#include <algorithm>
#include <fstream>
#include <iostream>

#include "utils.hpp"
#include "node.hpp"
#include "input.hpp"
#include "globals.hpp"


void run_simulation(std::vector<double> &dist, const std::vector<Node> &nodes) {
    std::vector<bool> used(num_nodes, false);
    std::vector<int> targets(gossip_factor, 0);
    std::vector<std::pair<double, int>> q;
    int start_node = rand_int(num_nodes);
    q.push_back({0, start_node});
    dist[start_node] = 0;
    while (!q.empty()) {
        int node = q.front().second;
        std::pop_heap(q.begin(), q.end(), std::greater<std::pair<double, int>>());
        q.pop_back();
        if (!nodes[node].is_corrupt() || node == start_node) {
            for (int x, j = 0; j < gossip_factor; ++j) {
                do {
                    x = rand_int(num_nodes);
                } while (used[x]);
                targets[j] = x;
                used[x] = true;
            }
            for (int j = 0; j < gossip_factor; ++j) {
                used[targets[j]] = false;
            }
            for (int x : targets) {
                const auto d = nodes[x].broadcast_duration(nodes[node]);
                if (dist[x] > dist[node] + d) {
                    dist[x] = dist[node] + d;
                    q.push_back({dist[x], x});
                    std::push_heap(q.begin(), q.end(), std::greater<std::pair<double, int>>());
                }
            }
        }
    }
    std::sort(dist.begin(), dist.end());
}

int main(int argc, char* argv[]) {
    read_data();

    std::ofstream log_file(get_log_file_path().c_str());
    std::cout << get_log_file_path() << std::endl;
    log_file << num_nodes << ","
             << gossip_factor << ","
             << corruption_chance * 100 << "%,"
             << computing_time << ","
             << latency
             << std::endl;

    std::vector<double> total(num_nodes);
    std::vector<int> times_reached(num_nodes);

    std::vector<std::pair<std::pair<int, std::string>, bool>> percents = {
            {{ 0.5 * num_nodes - 1,  "50%"}, false},
            {{0.66 * num_nodes - 1,  "66%"}, false},
            {{0.75 * num_nodes - 1,  "75%"}, false},
            {{0.90 * num_nodes - 1,  "90%"}, false},
            {{0.99 * num_nodes - 1,  "99%"}, false},
            {{       num_nodes - 1, "100%"}, false}
    };
    for (int steps = 1; steps <= 2e9; ++ steps) {
        std::vector<double> current(num_nodes, 1e20);
        for (int i = 0; i < num_nodes; ++ i) {
            nodes[i].reset(corruption_chance);
        }
        run_simulation(current, nodes);
        for (int i = 0; i < num_nodes; ++ i) {
            if (current[i] < 1e20) {
                total[i] += current[i];
                times_reached[i] += 1;
            }
        }

        if (steps % 20 == 0) {
            std::cout << "Number of nodes: " << num_nodes << "\n";
            std::cout << "GossipFactor: " << gossip_factor << "\n";
            std::cout << "Chance for a node to be not transmit: " << corruption_chance << "\n";
            std::cout << "Computing time for a transmission: " << computing_time << "ms\n";
            std::cout << "Latency to travel 6,371km (Earth Radius): " << latency << "ms\n";
            std::cout << "Simulation runs: " << steps << "\n";
        }
        bool first = true;
        for (auto &percent: percents) {
            const int index = percent.first.first;
            if (steps % 20 == 0) {
                std::cout << percent.first.second << ": " << total[index] / times_reached[index] << "ms";
                std::cout << ", reached this " << 100.0 * times_reached[index] / steps << "% of the time\n";
            }
            if (first) {
                first = false;
            } else {
                log_file << ",";
            }
            if (current[index] < 1e20) {
                log_file << current[index];
            }
        }
        log_file << "\n";
        log_file.flush();
        if (steps % 20 == 0) {
            std::cout << "A full report can be found in transmission.csv" << std::endl;
            std::cout << std::endl;
        }
    }

    return 0;
}
