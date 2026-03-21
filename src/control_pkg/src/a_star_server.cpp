#include "rclcpp/rclcpp.hpp"
#include <interfaces/srv/path_finding.hpp>
#include <memory>
#include <cmath>
#include <vector>
#include <set>
#include <algorithm> 

using namespace std;

// CONFIGURACIÓN (Debe coincidir con el cliente)
#define MAP_WIDTH 160
#define MAP_HEIGHT 90

struct NodeData {
    int x, y;
    double g, h, f;
    int parent_x, parent_y;
    // Sobrecarga para ordenar en el Set por menor costo F
    bool operator<(const NodeData& other) const {
        if (std::abs(f - other.f) > 1e-5) return f < other.f;
        if (std::abs(h - other.h) > 1e-5) return h < other.h;
        return (y != other.y) ? (y < other.y) : (x < other.x);
    }
};

class AStarServer : public rclcpp::Node
{
public:
    AStarServer() : Node("a_star_server")
    {
        this->declare_parameter<int>("robot_id", 0);
        int robot_id = this->get_parameter("robot_id").as_int();
        
        std::stringstream ss;
        ss << "/robot_0" << robot_id << "/path_finding_server";
        
        // Usamos Reentrant para permitir múltiples peticiones si fuera necesario
        callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
        
        service_ = this->create_service<interfaces::srv::PathFinding>(
            ss.str(), 
            std::bind(&AStarServer::solve_path, this, std::placeholders::_1, std::placeholders::_2),
            rmw_qos_profile_services_default,
            callback_group_);
            
        RCLCPP_INFO(this->get_logger(), "Servidor A* (Headless) Listo. Esperando peticiones...");
    }

private:
    rclcpp::Service<interfaces::srv::PathFinding>::SharedPtr service_;
    rclcpp::CallbackGroup::SharedPtr callback_group_;

    double calculateH(int x, int y, int destX, int destY) {
        // Distancia Euclidiana
        return std::sqrt(std::pow(x - destX, 2) + std::pow(y - destY, 2));
    }

    bool isValid(int x, int y) {
        return (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT);
    }

    void solve_path(const std::shared_ptr<interfaces::srv::PathFinding::Request> request,
                    std::shared_ptr<interfaces::srv::PathFinding::Response> response)
    {
        // 1. Reconstruir GRID desde el vector plano
        // Usamos vector para gestión automática de memoria
        std::vector<std::vector<int>> grid(MAP_HEIGHT, std::vector<int>(MAP_WIDTH));
        
        if (request->grid.size() != MAP_WIDTH * MAP_HEIGHT) {
            RCLCPP_ERROR(this->get_logger(), "Error: Tamaño del grid incorrecto (%zu)", request->grid.size());
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

        // Validaciones básicas
        if (!isValid(srcX, srcY) || !isValid(dstX, dstY)) {
            RCLCPP_WARN(this->get_logger(), "Coordenadas fuera de rango: (%d,%d)->(%d,%d)", srcX, srcY, dstX, dstY);
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
        // Usar un radio mayor para liberar inicio y meta (aprox. tamaño del robot)
        int safety_radius = 8; // Aproximadamente la mitad del robot más margen
        clear_radius(srcX, srcY, safety_radius);
        clear_radius(dstX, dstY, safety_radius);

        // 3. Manejo adicional de Destino Bloqueado (Backup)
        // Si después de limpiar la meta aún cae en obstáculo, buscar punto libre cercano
        if (grid[dstY][dstX] == 0) {
            bool found = false;
            for(int r=1; r<=3 && !found; r++) { 
                for(int dy=-r; dy<=r; dy++) {
                    for(int dx=-r; dx<=r; dx++) {
                        int nx = dstX + dx;
                        int ny = dstY + dy;
                        if(isValid(nx, ny) && grid[ny][nx] == 1) {
                            dstX = nx; dstY = ny;
                            found = true;
                            break;
                        }
                    }
                }
            }
            if(!found) {
                return; 
            }
        }

        // 4. Inicialización A*
        std::vector<std::vector<NodeData>> node_info(MAP_HEIGHT, std::vector<NodeData>(MAP_WIDTH));
        std::vector<std::vector<bool>> closed_list(MAP_HEIGHT, std::vector<bool>(MAP_WIDTH, false));

        // Resetear nodos
        for(int y=0; y<MAP_HEIGHT; y++) {
            for(int x=0; x<MAP_WIDTH; x++) {
                node_info[y][x] = {x, y, __DBL_MAX__, __DBL_MAX__, __DBL_MAX__, -1, -1};
            }
        }

        // Nodo inicial
        node_info[srcY][srcX] = {srcX, srcY, 0.0, 0.0, 0.0, srcX, srcY};
        
        std::set<std::pair<double, std::pair<int, int>>> open_list;
        open_list.insert({0.0, {srcY, srcX}});

        bool dest_found = false;
        int max_iter = 50000; // Timeout de seguridad
        int iter = 0;

        // 8 Vecinos
        int dx[] = {-1, 1, 0, 0, -1, -1, 1, 1};
        int dy[] = {0, 0, 1, -1, 1, -1, 1, -1};
        double cost[] = {1.0, 1.0, 1.0, 1.0, 1.414, 1.414, 1.414, 1.414};

        // 5. Bucle Principal
        while (!open_list.empty()) {
            iter++;
            if (iter > max_iter) break;

            auto p = *open_list.begin();
            open_list.erase(open_list.begin());

            int y = p.second.first;
            int x = p.second.second;

            closed_list[y][x] = true;

            if (x == dstX && y == dstY) {
                dest_found = true;
                break;
            }

            for (int i = 0; i < 8; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];

                if (isValid(nx, ny) && grid[ny][nx] == 1 && !closed_list[ny][nx]) {
                    double new_g = node_info[y][x].g + cost[i];
                    double new_h = calculateH(nx, ny, dstX, dstY);
                    double new_f = new_g + new_h;

                    if (node_info[ny][nx].f > new_f) {
                        // Si ya está en open_list, el set no permite actualizar la clave directamente.
                        // La estrategia simple es insertar de nuevo; el nodo se procesará con el menor costo primero.
                        // La versión vieja quedará en el set pero se descartará al revisar closed_list.
                        open_list.insert({new_f, {ny, nx}});
                        node_info[ny][nx] = {nx, ny, new_g, new_h, new_f, x, y};
                    }
                }
            }
        }

        // 6. Reconstrucción
        if (dest_found) {
            int cx = dstX;
            int cy = dstY;
            
            while (!(node_info[cy][cx].parent_x == cx && node_info[cy][cx].parent_y == cy)) {
                response->path_x.push_back(cx);
                response->path_y.push_back(cy);
                int px = node_info[cy][cx].parent_x;
                int py = node_info[cy][cx].parent_y;
                cx = px; cy = py;
            }
            response->path_x.push_back(cx);
            response->path_y.push_back(cy);

            std::reverse(response->path_x.begin(), response->path_x.end());
            std::reverse(response->path_y.begin(), response->path_y.end());
            
            response->path_size = response->path_x.size();
            RCLCPP_INFO(this->get_logger(), "Ruta calculada: %d pasos.", response->path_size);
        } else {
             // Solo Log si realmente falló (opcional, para no llenar la terminal si está bloqueado por obstáculos)
             // RCLCPP_WARN(this->get_logger(), "No se encontro camino.");
        }
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<AStarServer>();
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    executor.spin();
    rclcpp::shutdown();
    return 0;
}