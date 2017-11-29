#include <ctime>

#include <algorithm>
#include <fstream>
#include <iostream>

#include "utils.hpp"
#include "node.hpp"
#include "input.hpp"
#include "globals.hpp"
#include "heap.hpp"


void choose_targets(int node, std::vector<int> &targets, std::vector<bool> &used) {
    used[node] = true;
    for (int x, i = 0; i < gossip_factor; ++i) {
        do {
            x = RandInt(num_nodes);
        } while (used[x]);
        targets[i] = x;
        used[x] = true;
    }
    for (int i = 0; i < gossip_factor; ++ i) {
        used[targets[i]] = false;
    }
    used[node] = false;
}

void RunSimulation(std::vector<double> &dist, const std::vector<Node> &nodes) {
    std::vector<bool> used(num_nodes, false);
    std::vector<int> targets(gossip_factor, 0);

    DijkstraHeap q(num_nodes);

    int start_node = RandInt(num_nodes);
    q.Push(0, start_node);
    dist[start_node] = 0;

    while (!q.Empty()) {
        int node = q.Pop();
        if (nodes[node].IsCorrupt() && node != start_node) {
            continue;
        }
        choose_targets(node, targets, used);
        for (int x : targets) {
            const auto d = nodes[x].BroadcastDuration(nodes[node]);
            if (dist[x] > dist[node] + d) {
                dist[x] = dist[node] + d;
                q.Push(dist[x], x);
            }
        }
    }
    std::sort(dist.begin(), dist.end());
}

int main(int argc, char* argv[]) {
    ReadData();

    std::ofstream log_file(GetLogFilePath().c_str());

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

    double total_time = 0;

    for (int steps = 1; steps <= 2e9; ++ steps) {
        std::vector<double> current(num_nodes, 1e20);
        for (int i = 0; i < num_nodes; ++ i) {
            nodes[i].Reset(corruption_chance);
        }

        double start_time = clock();
        RunSimulation(current, nodes);
        total_time += (clock() - start_time) / CLOCKS_PER_SEC;

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
            std::cout << "Simulations total run time: " << total_time << "s\n";
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
