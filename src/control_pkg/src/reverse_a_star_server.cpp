#include "rclcpp/rclcpp.hpp"
#include <interfaces/srv/path_finding.hpp>
#include <memory>
#include <vector>
#include <cmath>
#include <queue>
#include <limits>
#include <algorithm>
#include <set>

using namespace std;

// --- CONFIGURACIÓN DE DIMENSIONES ---
#define MAP_WIDTH 160
#define MAP_HEIGHT 90

struct NodeData {
    int x, y;
    double g_cost;
    double h_cost;
    double f_cost;
    int parent_x, parent_y;

    // Min-Heap para Priority Queue: Menor F arriba
    bool operator>(const NodeData& other) const {
        return f_cost > other.f_cost;
    }
};

class ReverseAStarServer : public rclcpp::Node
{
public:
    ReverseAStarServer() : Node("reverse_a_star_server")
    {
        this->declare_parameter<int>("robot_id", 0);
        int robot_id = this->get_parameter("robot_id").as_int();
        
        // Mantenemos el nombre genérico del servicio para compatibilidad con el cliente
        std::stringstream ss;
        ss << "/robot_0" << robot_id << "/path_finding_server";

        // Callback Group Reentrante (Multihilo)
        callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

        service_ = this->create_service<interfaces::srv::PathFinding>(
            ss.str(), 
            std::bind(&ReverseAStarServer::service_callback, this, std::placeholders::_1, std::placeholders::_2),
            rmw_qos_profile_services_default,
            callback_group_);

        RCLCPP_INFO(this->get_logger(), "Reverse A* Server (Robust) Listo. Grid %dx%d", MAP_WIDTH, MAP_HEIGHT);
    }

private:
    rclcpp::Service<interfaces::srv::PathFinding>::SharedPtr service_;
    rclcpp::CallbackGroup::SharedPtr callback_group_;

    // Heurística Euclidiana
    double calculateH(int x, int y, int targetX, int targetY) {
        return std::sqrt(std::pow(x - targetX, 2) + std::pow(y - targetY, 2));
    }

    bool isValid(int x, int y) {
        return (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT);
    }

    void service_callback(const std::shared_ptr<interfaces::srv::PathFinding::Request> request,
                          std::shared_ptr<interfaces::srv::PathFinding::Response> response)
    {
        // 1. Cargar Mapa
        std::vector<std::vector<int>> grid(MAP_HEIGHT, std::vector<int>(MAP_WIDTH));
        
        if (request->grid.size() != MAP_WIDTH * MAP_HEIGHT) {
            RCLCPP_ERROR(this->get_logger(), "Error: Tamaño del grid incorrecto.");
            return;
        }

        int idx = 0;
        for(int y = 0; y < MAP_HEIGHT; y++) {
            for(int x = 0; x < MAP_WIDTH; x++) {
                grid[y][x] = request->grid[idx++];
            }
        }

        int srcX = request->src_x;
        int srcY = request->src_y;
        int dstX = request->dst_x;
        int dstY = request->dst_y;

        // Validaciones
        if (!isValid(srcX, srcY) || !isValid(dstX, dstY)) {
            RCLCPP_WARN(this->get_logger(), "Coords fuera de rango: Inicio(%d,%d) Meta(%d,%d)", srcX, srcY, dstX, dstY);
            response->path_size = 0;
            return;
        }

        // 2. Liberar Inicio y Meta (Safety Radius)
        // Asegura que el robot no nazca atrapado
        auto clear_radius = [&](int cx, int cy, int r) {
            for(int dy=-r; dy<=r; dy++){
                for(int dx=-r; dx<=r; dx++){
                    if(isValid(cx+dx, cy+dy)) grid[cy+dy][cx+dx] = 1;
                }
            }
        };
        clear_radius(srcX, srcY, 1);
        clear_radius(dstX, dstY, 1);

        // 3. Algoritmo: A* Inverso (Búsqueda desde META hacia INICIO)
        // Esto crea un campo de atracción natural hacia la meta.
        
        std::vector<std::vector<bool>> closed_list(MAP_HEIGHT, std::vector<bool>(MAP_WIDTH, false));
        std::vector<std::vector<NodeData>> all_nodes(MAP_HEIGHT, std::vector<NodeData>(MAP_WIDTH));

        // Inicializar tabla
        for(int y=0; y<MAP_HEIGHT; y++) 
            for(int x=0; x<MAP_WIDTH; x++) 
                all_nodes[y][x] = {x, y, __DBL_MAX__, __DBL_MAX__, __DBL_MAX__, -1, -1};

        // ARRANCAMOS DESDE LA META (g=0)
        // La H se calcula hacia el Inicio
        all_nodes[dstY][dstX] = {dstX, dstY, 0.0, calculateH(dstX, dstY, srcX, srcY), 0.0, dstX, dstY};

        std::priority_queue<NodeData, std::vector<NodeData>, std::greater<NodeData>> open_list;
        open_list.push(all_nodes[dstY][dstX]);

        bool found = false;
        int max_iter = 60000;
        int iter = 0;

        // Movimientos (8-conectado)
        int dx[] = {-1, 1, 0, 0, -1, -1, 1, 1};
        int dy[] = {0, 0, 1, -1, 1, -1, 1, -1};
        double costs[] = {1.0, 1.0, 1.0, 1.0, 1.414, 1.414, 1.414, 1.414};

        while(!open_list.empty()) {
            iter++;
            if(iter > max_iter) break;

            NodeData current = open_list.top();
            open_list.pop();

            int cx = current.x;
            int cy = current.y;

            if(closed_list[cy][cx]) continue;
            closed_list[cy][cx] = true;

            // ¿Llegamos al INICIO del robot?
            if(cx == srcX && cy == srcY) {
                found = true;
                break;
            }

            // Expandir hacia vecinos
            for(int i=0; i<8; i++) {
                int nx = cx + dx[i];
                int ny = cy + dy[i];

                if(isValid(nx, ny) && !closed_list[ny][nx] && grid[ny][nx] == 1) {
                    double new_g = all_nodes[cy][cx].g_cost + costs[i];
                    double new_h = calculateH(nx, ny, srcX, srcY);
                    double new_f = new_g + new_h;

                    if(all_nodes[ny][nx].f_cost > new_f) {
                        // El padre apunta hacia donde venimos (La Meta)
                        all_nodes[ny][nx] = {nx, ny, new_g, new_h, new_f, cx, cy}; 
                        open_list.push(all_nodes[ny][nx]);
                    }
                }
            }
        }

        // 4. Reconstrucción del Camino
        if (found) {
            // Buscamos desde el Inicio siguiendo los padres hasta la Meta
            int cx = srcX;
            int cy = srcY;

            while(!(cx == dstX && cy == dstY)) {
                response->path_x.push_back(cx);
                response->path_y.push_back(cy);

                NodeData& n = all_nodes[cy][cx];
                if(n.parent_x == -1) break; // Error de puntero
                cx = n.parent_x;
                cy = n.parent_y;
            }
            // Agregar la meta final
            response->path_x.push_back(dstX);
            response->path_y.push_back(dstY);

            response->path_size = response->path_x.size();
            RCLCPP_INFO(this->get_logger(), "Ruta Reverse A* encontrada: %d pasos.", response->path_size);

        } else {
            RCLCPP_WARN(this->get_logger(), "No se encontro camino.");
            response->path_size = 0;
            // Debug opcional:
            // RCLCPP_INFO(this->get_logger(), "Grid Status: Start(%d,%d)=%d, Goal(%d,%d)=%d", srcX, srcY, grid[srcY][srcX], dstX, dstY, grid[dstY][dstX]);
        }
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ReverseAStarServer>();
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    executor.spin();
    rclcpp::shutdown();
    return 0;
}