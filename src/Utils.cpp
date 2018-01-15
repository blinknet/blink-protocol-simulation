#include <string>

#include "Globals.hpp"
#include "Utils.hpp"

#include "third_party/cpp-base/src/cppbase/Random.hpp"

const double kPoissonConstant = exp(-5.0);

int PoissonNumber() {
    double p = 1;
    int k = 0;
    do {
        p *= base::RandFloat64();
        k += 1;
    } while (p > kPoissonConstant);
    return k - 1;
}

double LatencyDistribution() {
    return 1.0 + 0.15 * (PoissonNumber() - 4);
}

std::pair<long long, std::pair<double, double>> RandomCity() {
    long long population = base::RandInt64(cities.back().first);
    // A binary search here
    int left = 0, right = cities.size() - 1, cityIndex = -1;
    while (left <= right) {
        int middle = (left + right) / 2;
        if (cities[middle].first > population) {
            cityIndex = middle;
            right = middle - 1;
        } else {
            left = middle + 1;
        }
    }
    return cities[cityIndex];
}

double DegToRad(double deg) {
    return deg / 180.0 * M_PI;
}

std::string GetLogFolderName() {
    std::string folderName;
    folderName += "n=" + std::to_string(numNodes) + ",";
    folderName += "k=" + std::to_string(gossipFactor) + ",";
    folderName += "c=" + std::to_string(static_cast<int>(100 * corruptionChance)) + "%,";
    folderName += "t=" + std::to_string(static_cast<int>(computingTime)) + ",";
    folderName += "l=" + std::to_string(static_cast<int>(latency));
    return folderName;
}


std::string GetLogFolderPath() {
    std::string folderPath = logsRootFolderPath;
    if (folderPath[folderPath.size() - 1] != '/') {
        folderPath += "/";
    }
    return folderPath + GetLogFolderName() + "/";
}


// Log file path, relative to project root
std::string GetLogFilePath() {
    return GetLogFolderPath() + "data.csv";
}
