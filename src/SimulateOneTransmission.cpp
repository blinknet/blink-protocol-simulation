#include <ctime>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <atomic>
#include <mutex>
#include <thread>

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


std::ofstream logFile;
std::mutex logFileMutex;
std::vector<std::pair<std::pair<int, std::string>, bool>> percents;
std::vector<std::atomic<long long>> total;
std::vector<std::atomic<int>> timesReached;
std::atomic<int> numSimulations(0);


void WriteToLogFile(const std::vector<double> &currentRun) {
    std::lock_guard<std::mutex> lockGuard(logFileMutex);
    bool first = true;
    for (auto &percent : percents) {
        const int index = percent.first.first;
        if (first) {
            first = false;
        } else {
            logFile << ",";
        }
        if (currentRun[index] < 1e20) {
            logFile << currentRun[index];
        }
    }
    logFile << "\n";
    logFile.flush();
}


void UpdateStatistics(const std::vector<double> &currentRun) {
    for (auto &percent : percents) {
        const int index = percent.first.first;
        if (currentRun[index] < 1e20) {
            total[index] += (long long)(currentRun[index] * 1e9);
            timesReached[index] += 1;
        }
    }
    numSimulations += 1;
}


void StartThreadSafeSimulation() {
    std::vector<Node> localNodes(nodes.begin(), nodes.end());
    for (auto &localNode: localNodes) {
        localNode.reset(corruptionChance);
    }
    std::vector<double> currentRun(nodes.size(), 1e20);

    RunSimulation(currentRun, localNodes);

    UpdateStatistics(currentRun);
    WriteToLogFile(currentRun);
}

void ThreadWorker() {
    while (true) {
        StartThreadSafeSimulation();
    }
}


void LogStatisticsToConsole(const std::string &logFilePath) {
    std::cout << "Total number of simulations: " << numSimulations << "\n";
    std::cout << "Number of nodes: " << numNodes << "\n";
    std::cout << "GossipFactor: " << gossipFactor << "\n";
    std::cout << "Chance for a node to be not transmit: " << corruptionChance << "\n";
    std::cout << "Computing time for a transmission: " << computingTime << "ms\n";
    std::cout << "Latency to travel 6,371km (Earth Radius): " << latency << "ms\n";

    for (auto &percent : percents) {
        const int index = percent.first.first;
        std::cout << percent.first.second << ": " << total[index] / timesReached[index] / 1e9 << "ms";
        std::cout << ", reached this " << 100.0 * timesReached[index] / numSimulations << "% of the time\n";
    }
    std::cout << "A full report can be found in " << logFilePath << std::endl;
    std::cout << std::endl;
}


int main(int argc, char *argv[]) {
    ReadData();

    percents = {
        {{0.5 * numNodes - 1, "50%"}, false},    //
        {{0.66 * numNodes - 1, "66%"}, false},   //
        {{0.75 * numNodes - 1, "75%"}, false},   //
        {{0.90 * numNodes - 1, "90%"}, false},   //
        {{0.99 * numNodes - 1, "99"}, false},    //
        {{0.999 * numNodes - 1, "99.9%"}, false},//
        {{numNodes - 1, "100%"}, false}          //
    };

    base::MakeDir(GetLogFolderPath());
    std::string logFilePath = GetLogFilePath();
    base::Touch(logFilePath);

    logFile = std::ofstream(logFilePath.c_str(), std::ofstream::app);
    total = std::vector<std::atomic<long long>>(numNodes);
    timesReached = std::vector<std::atomic<int>>(numNodes);
    for (int i = 0; i < numNodes; ++ i) {
        std::atomic_init(&total[i], 0LL);
        std::atomic_init(&timesReached[i], 0);
    }

    std::cout << "Number of threads to start: " << numThreads << "\n";


    std::vector<std::thread*> threads;

    for (int i = 0; i < numThreads; ++ i) {
        std::thread *thread = new std::thread(ThreadWorker);
        threads.push_back(thread);
    }

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(15));

        LogStatisticsToConsole(logFilePath);
    }

    return 0;
}
