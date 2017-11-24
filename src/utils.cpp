#include <string>

#include "utils.hpp"
#include "globals.hpp"

long long RandLong(long long limit) {
    return (static_cast<long long>(rand()) * RAND_MAX + rand()) % limit;
}

double Rand01() {
    return ((double) RandLong(static_cast<long long>(RAND_MAX) * RAND_MAX) / (static_cast<long long>(RAND_MAX) * RAND_MAX));
}

int RandInt(int limit) {
    if (limit > RAND_MAX) {
        return static_cast<int>(RandLong(limit));
    }
    return rand() % limit;
}

const double kPoissonConstant = exp(-5.0);

int PoissonNumber() {
    double p = 1; int k = 0;
    do {
        p *= Rand01();
        k += 1;
    } while (p > kPoissonConstant);
    return k - 1;
}

double LatencyDistribution() {
    return 1.0 + 0.15 * (PoissonNumber() - 4);
}

std::pair<long long, std::pair<double, double>> RandomCity() {
    long long population = RandLong(cities.back().first);
    // A binary search here
    int left = 0, right = cities.size() - 1, city_index = -1;
    while (left <= right) {
        int middle = (left + right) / 2;
        if (cities[middle].first > population) {
            city_index = middle;
            right = middle - 1;
        } else {
            left = middle + 1;
        }
    }
    return cities[city_index];
}

double DegToRad(double deg) {
    return deg / 180.0 * M_PI;
}

std::string GetLogFolderName() {
    std::string folder_name;
    folder_name += "n=" + std::to_string(num_nodes) + ",";
    folder_name += "k=" + std::to_string(gossip_factor) + ",";
    folder_name += "c=" + std::to_string(static_cast<int>(100 * corruption_chance)) + "%,";
    folder_name += "t=" + std::to_string(static_cast<int>(computing_time)) + ",";
    folder_name += "l=" + std::to_string(static_cast<int>(latency));
    return folder_name;
}

// Log file path, relative to project root
std::string GetLogFilePath() {
    std::vector<std::string> folders = {
            "logs",
            GetLogFolderName()
    };

    std::string path;
    for (const auto &folder: folders) {
        path += folder + "/";
    }
    path += "data.csv";
    return path;
}
