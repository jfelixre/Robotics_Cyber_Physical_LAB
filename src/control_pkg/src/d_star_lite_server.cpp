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

#define ROW 90
#define COL 160

// Infinity value
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
    bool operator<(const Key& other) const {
        if (k1 != other.k1) return k1 < other.k1;
        return k2 < other.k2;
    }
    bool operator>(const Key& other) const {
        if (k1 != other.k1) return k1 > other.k1;
        return k2 > other.k2;
    }
    bool operator<=(const Key& other) const {
        return !(*this > other);
    }
};

struct QueueEntry {
    GridNode node;
    Key key;
    
    // Priority queue orders by largest element, so we need to reverse logic for min-heap
    bool operator>(const QueueEntry& other) const {
        return key > other.key; // Standard > for min-heap in std::priority_queue (which is max-heap) needs <. Wait.
        // std::priority_queue<T, vector<T>, greater<T>> is min-heap.
        // So we implement operator> for greater<T> to use.
        // greater<T> uses operator>.
        // So if this > other, it goes to bottom.
        // Correct.
    }
};

// Custom comparator for priority queue to act as Min-Heap
struct CompareEntry {
    bool operator()(const QueueEntry& a, const QueueEntry& b) {
        return a.key > b.key; // Returns true if a > b, so a comes AFTER b. Top is smallest.
    }
};

class DStarLiteServer : public rclcpp::Node
{
public:
    DStarLiteServer() : Node("d_star_lite_server")
    {
        this->declare_parameter<int>("robot_id", 0);
        robot_id = this->get_parameter("robot_id").as_int();
        RCLCPP_INFO(this->get_logger(), "D* Lite Server Started for Robot_ID: %d", robot_id);

        std::stringstream ss_service_name;
        ss_service_name << "/robot_0" << robot_id << "/path_finding_server"; // Keeping same name to be modular replacement
        // Note: If running alongside A*, this will conflict. User asked for "modular with A*", implying replacement or alternative.
        // If they want to run BOTH, they need different names. 
        // Assuming user will run ONE of them. Or I should name it d_star_server and let them remap.
        // But user said "usando los mismos topicos... para que pueda ser modular".
        // I will use a parameter to change service name or just use the same name pattern.
        // Let's use "d_star_server" and user can remap or change launch file.
        // Actually, to be "modular" usually means "plug and play". If I use the same service name, it conflicts if both nodes run.
        // I'll use "d_star_server" but print a warning.
        // Wait, if I use "a_star_server" name, I can't run both.
        // I'll use "d_star_server" and let the user decide.
        
        std::string service_name = ss_service_name.str();
        // Overwriting the name to be d_star_lite_server for safety, user can remap.
        // Actually, if I want it to be a drop-in replacement, I should probably use the same name if the user launches THIS node instead of the other.
        // I will use the same name pattern as A* server so it can replace it.
        
        service_ = this->create_service<interfaces::srv::PathFinding>(
            service_name, std::bind(&DStarLiteServer::service_callback, this,
            std::placeholders::_1, std::placeholders::_2));

        // Initialize grids
        for(int i=0; i<ROW; i++) {
            for(int j=0; j<COL; j++) {
                grid[i][j] = 1; // 1 is free
                rhs[i][j] = INF;
                g[i][j] = INF;
            }
        }
        km = 0;
        initialized = false;
    }

private:
    int robot_id;
    rclcpp::Service<interfaces::srv::PathFinding>::SharedPtr service_;
    
    // State
    int grid[ROW][COL];
    double rhs[ROW][COL];
    double g[ROW][COL];
    double km;
    GridNode start_node;
    GridNode goal_node;
    GridNode last_start_node;
    bool initialized;

    // Priority Queue
    // Using std::priority_queue with lazy deletion
    std::priority_queue<QueueEntry, std::vector<QueueEntry>, CompareEntry> U;

    // Helpers
    bool isValid(int x, int y) {
        return x >= 0 && x < COL && y >= 0 && y < ROW;
    }

    bool isBlocked(int x, int y) {
        // In A* server: 1 is free, 0 is blocked.
        if (!isValid(x, y)) return true;
        return grid[y][x] == 0;
    }

    double heuristic(GridNode a, GridNode b) {
        // Euclidean distance
        return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
    }

    Key calculateKey(GridNode s) {
        double min_g_rhs = std::min(g[s.y][s.x], rhs[s.y][s.x]);
        return {min_g_rhs + heuristic(start_node, s) + km, min_g_rhs};
    }

    void updateVertex(GridNode u) {
        if (u != goal_node) {
            double min_rhs = INF;
            // Check all 8 neighbors
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;
                    GridNode s_prime = {u.x + dx, u.y + dy};
                    if (isValid(s_prime.x, s_prime.y)) {
                        double cost = (isBlocked(u.x, u.y) || isBlocked(s_prime.x, s_prime.y)) ? INF : std::sqrt(dx*dx + dy*dy);
                        if (cost != INF) {
                            min_rhs = std::min(min_rhs, g[s_prime.y][s_prime.x] + cost);
                        }
                    }
                }
            }
            rhs[u.y][u.x] = min_rhs;
        }

        // Update queue
        // Since we can't easily remove, we just push. 
        // When popping, we check if key matches.
        // But wait, D* Lite checks if u is in U.
        // Lazy deletion makes "in U" check hard.
        // We will just push. If we pop a node with old key, we ignore it?
        // D* Lite logic:
        // if (g != rhs) insert(u, calculateKey(u))
        // else if (u in U) remove(u)
        
        // Simplified for lazy deletion:
        // Always push if g != rhs.
        // When popping u, if g == rhs, it's consistent, ignore.
        // If we pop u and it's not the "latest" version?
        // We can't easily know.
        // Standard trick: keep a separate "in_queue" set or map? No, just push.
        // If we pop u, we re-check calculateKey(u). If the popped key < calculateKey(u), then this entry is stale (we updated u to a higher key later? or lower?).
        // Actually, if g != rhs, we need it in queue.
        
        if (g[u.y][u.x] != rhs[u.y][u.x]) {
            U.push({u, calculateKey(u)});
        }
    }

    void computeShortestPath() {
        int max_steps = 100000; // Increased safety break
        int steps = 0;
        
        while (!U.empty() && steps < max_steps) {
            QueueEntry top = U.top();
            Key k_old = top.key;
            GridNode u = top.node;
            
            // Debug print every 5000 steps
            // if (steps % 5000 == 0) {
            //    RCLCPP_INFO(this->get_logger(), "Step %d: Processing (%d, %d), k_old=(%.2f, %.2f)", steps, u.x, u.y, k_old.k1, k_old.k2);
            // }

            Key k_new = calculateKey(u);

            if (k_old < k_new) {
                // Stale or needs update
                U.pop();
                // If it was stale, k_new might be different.
                // But wait, if k_old < k_new, it means the node's priority increased (worse).
                // D* Lite says: if k_old < calculateKey(u), put it back with new key.
                U.push({u, k_new});
            } else if (g[u.y][u.x] > rhs[u.y][u.x]) {
                U.pop();
                g[u.y][u.x] = rhs[u.y][u.x];
                // Update neighbors
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (dx == 0 && dy == 0) continue;
                        GridNode s = {u.x + dx, u.y + dy};
                        if (isValid(s.x, s.y)) {
                            updateVertex(s);
                        }
                    }
                }
            } else {
                U.pop();
                g[u.y][u.x] = INF;
                updateVertex(u); // Pred(u) includes u itself in logic sometimes? No.
                // Update neighbors
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (dx == 0 && dy == 0) continue;
                        GridNode s = {u.x + dx, u.y + dy};
                        if (isValid(s.x, s.y)) {
                            updateVertex(s);
                        }
                    }
                }
            }
            
            // Termination condition
            if (U.empty()) break;
            if (U.top().key > calculateKey(start_node) && rhs[start_node.y][start_node.x] == g[start_node.y][start_node.x]) {
                break;
            }
            steps++;
        }
        RCLCPP_INFO(this->get_logger(), "ComputeShortestPath finished after %d steps. U.empty? %d. rhs[start]=%.2f, g[start]=%.2f", 
            steps, U.empty(), rhs[start_node.y][start_node.x], g[start_node.y][start_node.x]);
    }

    void service_callback(const std::shared_ptr<interfaces::srv::PathFinding::Request> request,
                          std::shared_ptr<interfaces::srv::PathFinding::Response> response)
    {
        GridNode req_start = {request->src_x, request->src_y};
        GridNode req_goal = {request->dst_x, request->dst_y};

        RCLCPP_INFO(this->get_logger(), "Request: Start(%d, %d), Goal(%d, %d)", req_start.x, req_start.y, req_goal.x, req_goal.y);

        // Check if we need full reset
        if (!initialized || req_goal != goal_node) {
            // Full Reset
            start_node = req_start;
            goal_node = req_goal;
            last_start_node = start_node;
            km = 0;
            
            // Reset G and RHS
            for(int i=0; i<ROW; i++) {
                for(int j=0; j<COL; j++) {
                    rhs[i][j] = INF;
                    g[i][j] = INF;
                }
            }
            rhs[goal_node.y][goal_node.x] = 0;
            
            // Clear Queue
            U = std::priority_queue<QueueEntry, std::vector<QueueEntry>, CompareEntry>();
            U.push({goal_node, calculateKey(goal_node)});
            
            // Load Grid
            int grid_index = 0;
            for (int i=0; i<ROW; i++){
                for (int j=0; j<COL; j++){
                    int val = request->grid[grid_index++];
                    // Force start and goal to be free
                    if (i == req_start.y && j == req_start.x) val = 1;
                    if (i == req_goal.y && j == req_goal.x) val = 1;
                    grid[i][j] = val;
                }
            }
            
            RCLCPP_INFO(this->get_logger(), "Grid loaded. Start val: %d, Goal val: %d", grid[req_start.y][req_start.x], grid[req_goal.y][req_goal.x]);

            // Debug: Check neighbors of start
            bool start_trapped = true;
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;
                    if (isValid(req_start.x + dx, req_start.y + dy)) {
                        if (grid[req_start.y + dy][req_start.x + dx] == 1) {
                            start_trapped = false;
                        }
                    }
                }
            }
            if (start_trapped) {
                RCLCPP_WARN(this->get_logger(), "START NODE IS TRAPPED! All neighbors are blocked.");
            } else {
                RCLCPP_INFO(this->get_logger(), "Start node has at least one free neighbor.");
            }

            initialized = true;
            computeShortestPath();
        } else {
            // Incremental Update
            start_node = req_start;
            km += heuristic(last_start_node, start_node);
            last_start_node = start_node;

            // Detect Map Changes
            int grid_index = 0;
            bool map_changed = false;
            for (int i=0; i<ROW; i++){
                for (int j=0; j<COL; j++){
                    int new_val = request->grid[grid_index++];
                    // Force start and goal to be free
                    if (i == req_start.y && j == req_start.x) new_val = 1;
                    if (i == req_goal.y && j == req_goal.x) new_val = 1;

                    if (grid[i][j] != new_val) {
                        // Cost changed
                        grid[i][j] = new_val;
                        map_changed = true;
                        
                        // Update this node and neighbors
                        GridNode u = {j, i}; // x=j, y=i
                        updateVertex(u);
                        for (int dx = -1; dx <= 1; dx++) {
                            for (int dy = -1; dy <= 1; dy++) {
                                if (dx == 0 && dy == 0) continue;
                                GridNode s = {u.x + dx, u.y + dy};
                                if (isValid(s.x, s.y)) {
                                    updateVertex(s);
                                }
                            }
                        }
                    }
                }
            }
            
            if (map_changed) {
                computeShortestPath();
            }
        }

        // Extract Path
        std::vector<int> path_x, path_y;
        
        if (g[start_node.y][start_node.x] == INF) {
            // No path
            RCLCPP_WARN(this->get_logger(), "No path found! g[start] is INF.");
            response->path_size = 0;
            return;
        }

        GridNode current = start_node;
        path_x.push_back(current.x);
        path_y.push_back(current.y);

        int max_path = 5000;
        while (current != goal_node && path_x.size() < max_path) {
            GridNode best_next = current;
            double min_cost = INF;

            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;
                    GridNode next = {current.x + dx, current.y + dy};
                    if (isValid(next.x, next.y)) {
                        double move_cost = (isBlocked(current.x, current.y) || isBlocked(next.x, next.y)) ? INF : std::sqrt(dx*dx + dy*dy);
                        if (move_cost != INF && g[next.y][next.x] != INF) {
                            double total = g[next.y][next.x] + move_cost;
                            if (total < min_cost) {
                                min_cost = total;
                                best_next = next;
                            }
                        }
                    }
                }
            }

            if (best_next == current) {
                RCLCPP_WARN(this->get_logger(), "Stuck during path extraction!");
                break; // Stuck
            }
            current = best_next;
            path_x.push_back(current.x);
            path_y.push_back(current.y);
        }

        RCLCPP_INFO(this->get_logger(), "Path found with size: %ld", path_x.size());
        response->path_x = path_x;
        response->path_y = path_y;
        response->path_size = path_x.size();
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<DStarLiteServer>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
