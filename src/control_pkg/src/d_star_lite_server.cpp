#include "rclcpp/rclcpp.hpp"
#include <interfaces/srv/path_finding.hpp>
#include <memory>
#include <vector>
#include <cmath>
#include <queue>
#include <limits>
#include <algorithm>
#include <iostream>

using namespace std;

// --- CONFIGURACIÓN DE DIMENSIONES (Igual que tu Cliente) ---
#define MAP_WIDTH 160  // X (Columnas)
#define MAP_HEIGHT 90  // Y (Filas)

// Valor infinito para costos
const double INF = std::numeric_limits<double>::infinity();

struct GridNode {
    int x, y;
    bool operator==(const GridNode& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const GridNode& other) const {
        return !(*this == other);
    }
};

struct Key {
    double k1, k2;
    // Operadores para comparar claves (Lexicográfico)
    bool operator<(const Key& other) const {
        if (std::abs(k1 - other.k1) > 1e-5) return k1 < other.k1;
        return k2 < other.k2;
    }
    bool operator>(const Key& other) const {
        if (std::abs(k1 - other.k1) > 1e-5) return k1 > other.k1;
        return k2 > other.k2;
    }
    bool operator<=(const Key& other) const {
        return !(*this > other);
    }
};

struct QueueEntry {
    GridNode node;
    Key key;
};

// Comparador para Priority Queue (Min-Heap: el menor arriba)
struct CompareEntry {
    bool operator()(const QueueEntry& a, const QueueEntry& b) {
        return a.key > b.key; 
    }
};

class DStarLiteServer : public rclcpp::Node
{
public:
    DStarLiteServer() : Node("path_finding_server") // Nombre genérico para reemplazar A*
    {
        this->declare_parameter<int>("robot_id", 0);
        robot_id = this->get_parameter("robot_id").as_int();
        
        std::stringstream ss;
        ss << "/robot_0" << robot_id << "/path_finding_server";

        // Callback Group Reentrante para evitar Deadlocks
        callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

        service_ = this->create_service<interfaces::srv::PathFinding>(
            ss.str(), 
            std::bind(&DStarLiteServer::service_callback, this, std::placeholders::_1, std::placeholders::_2),
            rmw_qos_profile_services_default,
            callback_group_);

        RCLCPP_INFO(this->get_logger(), "D* Lite Server Listo para Robot %d (Grid %dx%d)", robot_id, MAP_WIDTH, MAP_HEIGHT);

        // Inicializar estructuras de datos
        grid.resize(MAP_HEIGHT, std::vector<int>(MAP_WIDTH, 1));
        rhs.resize(MAP_HEIGHT, std::vector<double>(MAP_WIDTH, INF));
        g.resize(MAP_HEIGHT, std::vector<double>(MAP_WIDTH, INF));
        
        km = 0;
        initialized = false;
    }

private:
    int robot_id;
    rclcpp::Service<interfaces::srv::PathFinding>::SharedPtr service_;
    rclcpp::CallbackGroup::SharedPtr callback_group_;
    
    // Estado D* Lite
    std::vector<std::vector<int>> grid;
    std::vector<std::vector<double>> rhs;
    std::vector<std::vector<double>> g;
    double km;
    
    GridNode start_node;
    GridNode goal_node;
    GridNode last_start_node;
    bool initialized;

    // Cola de Prioridad
    std::priority_queue<QueueEntry, std::vector<QueueEntry>, CompareEntry> U;

    // --- AYUDANTES ---
    bool isValid(int x, int y) {
        return x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT;
    }

    bool isBlocked(int x, int y) {
        if (!isValid(x, y)) return true;
        return grid[y][x] == 0; // 0 = Obstáculo
    }

    double heuristic(GridNode a, GridNode b) {
        // Distancia Euclídea (se puede cambiar a Manhattan/Chebyshev)
        return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
    }

    Key calculateKey(GridNode s) {
        double min_g_rhs = std::min(g[s.y][s.x], rhs[s.y][s.x]);
        return {min_g_rhs + heuristic(start_node, s) + km, min_g_rhs};
    }

    void updateVertex(GridNode u) {
        if (u != goal_node) {
            double min_rhs = INF;
            // Revisar 8 vecinos
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;
                    GridNode s_prime = {u.x + dx, u.y + dy};
                    
                    if (isValid(s_prime.x, s_prime.y)) {
                        // Costo de movimiento (1.0 recto, 1.414 diagonal)
                        double move_cost = std::sqrt(dx*dx + dy*dy);
                        
                        // Si alguno está bloqueado, costo infinito
                        if (isBlocked(u.x, u.y) || isBlocked(s_prime.x, s_prime.y)) {
                             move_cost = INF;
                        }

                        if (g[s_prime.y][s_prime.x] != INF && move_cost != INF) {
                            min_rhs = std::min(min_rhs, g[s_prime.y][s_prime.x] + move_cost);
                        }
                    }
                }
            }
            rhs[u.y][u.x] = min_rhs;
        }

        // Si es inconsistente, añadir a la cola
        if (std::abs(g[u.y][u.x] - rhs[u.y][u.x]) > 1e-5) {
            U.push({u, calculateKey(u)});
        }
    }

    void computeShortestPath() {
        int max_steps = 80000; // Seguridad para no colgar el proceso
        int steps = 0;
        
        while (!U.empty() && steps < max_steps) {
            // Condición de parada de D* Lite
            if (U.top().key <= calculateKey(start_node) || rhs[start_node.y][start_node.x] != g[start_node.y][start_node.x]) {
                
                QueueEntry top = U.top();
                GridNode u = top.node;
                Key k_old = top.key;
                Key k_new = calculateKey(u);

                if (k_old < k_new) {
                    U.pop();
                    U.push({u, k_new});
                } 
                else if (g[u.y][u.x] > rhs[u.y][u.x]) {
                    U.pop();
                    g[u.y][u.x] = rhs[u.y][u.x];
                    // Actualizar vecinos
                    for (int dx = -1; dx <= 1; dx++) {
                        for (int dy = -1; dy <= 1; dy++) {
                            if (dx == 0 && dy == 0) continue;
                            GridNode s = {u.x + dx, u.y + dy};
                            if (isValid(s.x, s.y)) updateVertex(s);
                        }
                    }
                } 
                else {
                    U.pop();
                    double g_old = g[u.y][u.x];
                    g[u.y][u.x] = INF;
                    updateVertex(u); // Pred(u)
                    for (int dx = -1; dx <= 1; dx++) {
                        for (int dy = -1; dy <= 1; dy++) {
                            if (dx == 0 && dy == 0) continue;
                            GridNode s = {u.x + dx, u.y + dy};
                            if (isValid(s.x, s.y)) updateVertex(s);
                        }
                    }
                }
                steps++;
            } else {
                break;
            }
        }
    }

    void service_callback(const std::shared_ptr<interfaces::srv::PathFinding::Request> request,
                          std::shared_ptr<interfaces::srv::PathFinding::Response> response)
    {
        GridNode req_start = {request->src_x, request->src_y};
        GridNode req_goal = {request->dst_x, request->dst_y};

        // Validar coordenadas
        if(!isValid(req_start.x, req_start.y) || !isValid(req_goal.x, req_goal.y)) {
             RCLCPP_ERROR(this->get_logger(), "Coordenadas invalidas");
             response->path_size = 0;
             return;
        }

        // 1. Detección de Cambios de Misión (Reinicio Completo)
        // Si cambia la meta, D* Lite pierde eficiencia, mejor reiniciar.
        if (!initialized || req_goal != goal_node) {
            // RCLCPP_INFO(this->get_logger(), "Nueva meta detectada. Reiniciando D*...");
            
            start_node = req_start;
            goal_node = req_goal;
            last_start_node = start_node;
            km = 0;
            
            // Resetear costmaps
            for(int y=0; y<MAP_HEIGHT; y++) {
                std::fill(rhs[y].begin(), rhs[y].end(), INF);
                std::fill(g[y].begin(), g[y].end(), INF);
            }
            rhs[goal_node.y][goal_node.x] = 0;
            
            // Vaciar cola
            U = std::priority_queue<QueueEntry, std::vector<QueueEntry>, CompareEntry>();
            U.push({goal_node, calculateKey(goal_node)});
            
            // Cargar Mapa Inicial
            int idx = 0;
            for(int y=0; y<MAP_HEIGHT; y++) {
                for(int x=0; x<MAP_WIDTH; x++) {
                    grid[y][x] = request->grid[idx++];
                }
            }
            // Forzar inicio/fin libres
            grid[req_start.y][req_start.x] = 1;
            grid[req_goal.y][req_goal.x] = 1;

            initialized = true;
            computeShortestPath();

        } else {
            // 2. Actualización Incremental (Mismo objetivo, mapa cambia o robot se mueve)
            start_node = req_start;
            km += heuristic(last_start_node, start_node);
            last_start_node = start_node;

            // Detectar cambios en el mapa
            bool map_changed = false;
            int idx = 0;
            for(int y=0; y<MAP_HEIGHT; y++) {
                for(int x=0; x<MAP_WIDTH; x++) {
                    int new_val = request->grid[idx++];
                    // Forzar libres
                    if ((x==req_start.x && y==req_start.y) || (x==req_goal.x && y==req_goal.y)) new_val = 1;

                    if (grid[y][x] != new_val) {
                        grid[y][x] = new_val; // Actualizar costo
                        map_changed = true;
                        
                        // Actualizar nodo afectado y sus vecinos
                        updateVertex({x, y});
                        for (int dx = -1; dx <= 1; dx++) {
                            for (int dy = -1; dy <= 1; dy++) {
                                if (dx == 0 && dy == 0) continue;
                                GridNode n = {x + dx, y + dy};
                                if (isValid(n.x, n.y)) updateVertex(n);
                            }
                        }
                    }
                }
            }
            
            if (map_changed) {
                // RCLCPP_INFO(this->get_logger(), "Mapa cambio. Recalculando ruta...");
                computeShortestPath();
            }
        }

        // 3. Extracción del Camino (Gradient Descent)
        std::vector<int> path_x, path_y;
        
        if (g[start_node.y][start_node.x] == INF) {
            RCLCPP_WARN(this->get_logger(), "No hay camino posible (Costo infinito en inicio).");
            response->path_size = 0;
            return;
        }

        GridNode current = start_node;
        path_x.push_back(current.x);
        path_y.push_back(current.y);

        int max_path_len = 2000;
        
        while (current != goal_node && path_x.size() < (size_t)max_path_len) {
            GridNode best_next = current;
            double min_cost = INF;

            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;
                    GridNode next = {current.x + dx, current.y + dy};
                    
                    if (isValid(next.x, next.y)) {
                        double move_cost = std::sqrt(dx*dx + dy*dy);
                        if (isBlocked(next.x, next.y)) move_cost = INF;
                        
                        if (move_cost != INF && g[next.y][next.x] != INF) {
                            double total = g[next.y][next.x] + move_cost;
                            // Preferencia por diagonales para suavidad
                            if (total < min_cost) {
                                min_cost = total;
                                best_next = next;
                            }
                        }
                    }
                }
            }

            if (best_next == current) {
                // Atrapado en mínimo local
                break; 
            }
            current = best_next;
            path_x.push_back(current.x);
            path_y.push_back(current.y);
        }

        // RCLCPP_INFO(this->get_logger(), "Ruta D* enviada: %zu puntos.", path_x.size());
        response->path_x = path_x;
        response->path_y = path_y;
        response->path_size = path_x.size();
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<DStarLiteServer>();
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    executor.spin();
    rclcpp::shutdown();
    return 0;
}