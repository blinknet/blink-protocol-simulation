#include <string>

#include "utils.hpp"
#include "globals.hpp"

double rand01() {
    return ((double) rand() / (RAND_MAX));
}

int rand_int(int limit) {
    return rand() % limit;
}

long long rand_long(long long limit) {
    return (static_cast<long long>(rand()) * RAND_MAX + rand()) % limit;
}

const double poisson_constant = exp(-5.0);

int poisson_number() {
    double p = 1; int k = 0;
    do {
        p *= rand01();
        k += 1;
    } while (p > poisson_constant);
    return k - 1;
}

double latency_distribution() {
    return 1.0 + 0.15 * (poisson_number() - 4);
}

std::pair<long long, std::pair<double, double>> random_city() {
    long long population = rand_long(cities.back().first);
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

double deg_to_rad(double deg) {
    return deg / 180.0 * M_PI;
}

std::string get_log_folder_name() {
    std::string folder_name;
    folder_name += "n=" + std::to_string(num_nodes) + ",";
    folder_name += "k=" + std::to_string(gossip_factor) + ",";
    folder_name += "c=" + std::to_string(static_cast<int>(100 * corruption_chance)) + "%,";
    folder_name += "t=" + std::to_string(static_cast<int>(computing_time)) + ",";
    folder_name += "l=" + std::to_string(static_cast<int>(latency));
    return folder_name;
}

// Log file path, relative to project root
std::string get_log_file_path() {
    std::vector<std::string> folders = {
            "logs",
            get_log_folder_name()
    };

    std::string path;
    for (const auto &folder: folders) {
        path += folder + "/";
    }
    path += "data.csv";
    return path;
}