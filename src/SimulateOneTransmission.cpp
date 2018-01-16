#include <ctime>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include "Globals.hpp"
#include "Heap.hpp"
#include "Input.hpp"
#include "Node.hpp"
#include "Utils.hpp"

#include "third_party/cpp-base/src/cppbase/Random.hpp"
#include "third_party/cpp-base/src/cppbase/OS.hpp"

void chooseTargets(int node, std::vector<int> &targets, std::vector<bool> &used) {
    used[node] = true;
    for (size_t x, i = 0; i < gossipFactor; ++i) {
        do {
            x = base::RandUInt32(numNodes);
        } while (used[x]);
        targets[i] = x;
        used[x] = true;
    }
    for (size_t i = 0; i < gossipFactor; ++i) {
        used[targets[i]] = false;
    }
    used[node] = false;
}

void RunSimulation(std::vector<double> &dist, const std::vector<Node> &nodes) {
    std::vector<bool> used(numNodes, false);
    std::vector<int> targets(gossipFactor, 0);

    DijkstraHeap q(numNodes);

    int startNode = base::RandInt32(numNodes);
    q.push(0, startNode);
    dist[startNode] = 0;

    while (!q.empty()) {
        int node = q.pop();
        if (nodes[node].isCorrupt() && node != startNode) {
            continue;
        }

        chooseTargets(node, targets, used);
        for (int x : targets) {
            const auto d = nodes[x].broadcastDuration(nodes[node]);
            if (dist[x] > dist[node] + d) {
                dist[x] = dist[node] + d;
                q.push(dist[x], x);
            }
        }
    }
    std::sort(dist.begin(), dist.end());
}

int main(int argc, char *argv[]) {
    ReadData();

    base::MakeDir(GetLogFolderPath());
    std::string logFilePath = GetLogFilePath();
    base::Touch(logFilePath);

    std::ofstream logFile(logFilePath.c_str(), std::ofstream::app);

    std::vector<double> total(numNodes);
    std::vector<int> timesReached(numNodes);

    std::vector<std::pair<std::pair<int, std::string>, bool>> percents = {
        {{0.5 * numNodes - 1, "50%"}, false},    //
        {{0.66 * numNodes - 1, "66%"}, false},   //
        {{0.75 * numNodes - 1, "75%"}, false},   //
        {{0.90 * numNodes - 1, "90%"}, false},   //
        {{0.99 * numNodes - 1, "99"}, false},    //
        {{0.999 * numNodes - 1, "99.9%"}, false},//
        {{numNodes - 1, "100%"}, false}          //
    };

    double totalTime = 0;

    for (int steps = 1; steps <= 2e9; ++steps) {
        std::vector<double> current(numNodes, 1e20);
        for (size_t i = 0; i < numNodes; ++i) {
            nodes[i].reset(corruptionChance);
        }

        double startTime = clock();
        RunSimulation(current, nodes);
        double currentRoundTime = (clock() - startTime) / CLOCKS_PER_SEC;

        bool logToConsole = ((int)(totalTime / 20) != (int)((totalTime + currentRoundTime) / 20));

        totalTime += currentRoundTime;

        for (size_t i = 0; i < numNodes; ++i) {
            if (current[i] < 1e20) {
                total[i] += current[i];
                timesReached[i] += 1;
            }
        }

        if (logToConsole) {
            std::cout << "Number of nodes: " << numNodes << "\n";
            std::cout << "GossipFactor: " << gossipFactor << "\n";
            std::cout << "Chance for a node to be not transmit: " << corruptionChance << "\n";
            std::cout << "Computing time for a transmission: " << computingTime << "ms\n";
            std::cout << "Latency to travel 6,371km (Earth Radius): " << latency << "ms\n";
            std::cout << "Simulation runs: " << steps << "\n";
            std::cout << "Simulations total run time: " << totalTime << "s\n";
        }

        bool first = true;
        for (auto &percent : percents) {
            const int index = percent.first.first;
            if (logToConsole) {
                std::cout << percent.first.second << ": " << total[index] / timesReached[index] << "ms";
                std::cout << ", reached this " << 100.0 * timesReached[index] / steps << "% of the time\n";
            }
            if (first) {
                first = false;
            } else {
                logFile << ",";
            }
            if (current[index] < 1e20) {
                logFile << current[index];
            }
        }
        logFile << "\n";
        logFile.flush();
        if (logToConsole) {
            std::cout << "A full report can be found in " << logFilePath << std::endl;
            std::cout << std::endl;
        }
    }

    return 0;
}
