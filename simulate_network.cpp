#include <vector>
#include <iostream>
#include <fstream>

#include "utils.hpp"
#include "node.hpp"
#include "globals.hpp"
#include "input.hpp"


void run_simulation(int transmissions=1) {
    for (int i = 1; i <= transmissions; ++ i) {
        int start_node_index;
        do {
            start_node_index = rand_int(num_nodes);
        } while (nodes[start_node_index].is_corrupt());

        time_line.push(
                Event(
                        start_node_index,
                        start_node_index,
                        i,
                        (i - 1) * 1000.0,
                        Event::EventType::BROADCAST
                )
        );
    }

    while (!time_line.empty()) {
        time_line.apply_first();
    }
}


int main(int argc, char* argv[]) {
    read_data();

    const int TRANSMISSIONS = 1;

    std::ofstream log_file("../transmission.csv");
    log_file << "50%,66%,75%,90%,99%,100%" << std::endl;

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
        for (int i = 0; i < num_nodes; ++ i) {
            nodes[i].reset(corruption_chance);
        }
        run_simulation(TRANSMISSIONS);

        std::vector<double> current(num_nodes);
        for(int i = 0; i < num_nodes; i += 1) {
            current[i] = nodes[i].received_version(TRANSMISSIONS) ? nodes[i].get_version_receive_time(TRANSMISSIONS) : 1e20;
        }

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
