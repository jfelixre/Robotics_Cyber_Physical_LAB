#include "rclcpp/rclcpp.hpp"
#include <interfaces/srv/path_finding.hpp>
#include <memory>
#include <vector>
#include <cmath>
#include <queue>
#include <algorithm>
#include <set>

using namespace std;

#define MAP_WIDTH 160
#define MAP_HEIGHT 90

struct NodeData {
    int x, y;
    double h_cost; // Solo nos importa H
    int parent_x, parent_y;

    // GREEDY: Ordenar SOLO por la heurística (H).
    // El nodo que parece estar más cerca de la meta va primero.
    bool operator>(const NodeData& other) const {
        return h_cost > other.h_cost;
    }
};

class GreedyServer : public rclcpp::Node
{
public:
    GreedyServer() : Node("greedy_server")
    {
        this->declare_parameter<int>("robot_id", 0);
        int robot_id = this->get_parameter("robot_id").as_int();
        
        // Mismo nombre de servicio para ser compatible con tu cliente
        std::stringstream ss;
        ss << "/robot_0" << robot_id << "/path_finding_server";

        callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

        service_ = this->create_service<interfaces::srv::PathFinding>(
            ss.str(), 
            std::bind(&GreedyServer::service_callback, this, std::placeholders::_1, std::placeholders::_2),
            rmw_qos_profile_services_default,
            callback_group_);

        RCLCPP_INFO(this->get_logger(), "Greedy Best-First Server Listo.");
    }

private:
    rclcpp::Service<interfaces::srv::PathFinding>::SharedPtr service_;
    rclcpp::CallbackGroup::SharedPtr callback_group_;

    double calculateH(int x, int y, int destX, int destY) {
        // Distancia Manhattan suele funcionar mejor para Greedy en grids,
        // pero usaremos Euclídea para comparar justamente con tus otros algorimos.
        return std::sqrt(std::pow(x - destX, 2) + std::pow(y - destY, 2));
    }

    bool isValid(int x, int y) {
        return (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT);
    }

    void service_callback(const std::shared_ptr<interfaces::srv::PathFinding::Request> request,
                          std::shared_ptr<interfaces::srv::PathFinding::Response> response)
    {
        // 1. Cargar Mapa
        std::vector<std::vector<int>> grid(MAP_HEIGHT, std::vector<int>(MAP_WIDTH));
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

        if (!isValid(srcX, srcY) || !isValid(dstX, dstY)) {
            response->path_size = 0; return;
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

        // 3. Greedy Search
        std::vector<std::vector<bool>> visited(MAP_HEIGHT, std::vector<bool>(MAP_WIDTH, false));
        // Matriz para guardar padres y reconstruir
        std::vector<std::vector<std::pair<int, int>>> parents(MAP_HEIGHT, std::vector<std::pair<int, int>>(MAP_WIDTH, {-1, -1}));

        std::priority_queue<NodeData, std::vector<NodeData>, std::greater<NodeData>> open_list;

        // Nodo inicial
        open_list.push({srcX, srcY, calculateH(srcX, srcY, dstX, dstY), -1, -1});
        visited[srcY][srcX] = true;

        bool found = false;
        int max_iter = 30000;
        int iter = 0;

        int dx[] = {-1, 1, 0, 0, -1, -1, 1, 1};
        int dy[] = {0, 0, 1, -1, 1, -1, 1, -1};

        while(!open_list.empty()) {
            iter++;
            if(iter > max_iter) break;

            NodeData current = open_list.top();
            open_list.pop();

            int cx = current.x;
            int cy = current.y;

            if(cx == dstX && cy == dstY) {
                found = true;
                break;
            }

            for(int i=0; i<8; i++) {
                int nx = cx + dx[i];
                int ny = cy + dy[i];

                if(isValid(nx, ny) && !visited[ny][nx] && grid[ny][nx] == 1) {
                    visited[ny][nx] = true;
                    parents[ny][nx] = {cx, cy};
                    
                    // En Greedy solo importa cuánto falta para la meta (H)
                    double h = calculateH(nx, ny, dstX, dstY);
                    open_list.push({nx, ny, h, cx, cy});
                }
            }
        }

        // 4. Reconstruir
        if (found) {
            int cx = dstX;
            int cy = dstY;
            while(!(cx == srcX && cy == srcY)) {
                response->path_x.push_back(cx);
                response->path_y.push_back(cy);
                std::pair<int, int> p = parents[cy][cx];
                cx = p.first;
                cy = p.second;
            }
            response->path_x.push_back(srcX);
            response->path_y.push_back(srcY);

            std::reverse(response->path_x.begin(), response->path_x.end());
            std::reverse(response->path_y.begin(), response->path_y.end());
            response->path_size = response->path_x.size();
            
            RCLCPP_INFO(this->get_logger(), "Ruta Greedy calculada: %d pasos.", response->path_size);
        } else {
            response->path_size = 0;
        }
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<GreedyServer>();
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    executor.spin();
    rclcpp::shutdown();
    return 0;
}