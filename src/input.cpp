#include "input.hpp"

#include <fstream>
#include <iostream>
#include <vector>

#include "third_party/autojson/src/lib/json.hpp"

#include "globals.hpp"

const std::string kDefaultCitiesFilePath = "data/cities.txt";
const int kDefautNumNodes = 10000;
const int kDefaultGossipFactor = 16;
const double kDefaultCorruptionChance = 0.1;
const double kComputingTime = 20;
const double kLatency = 100;

void ReadData() {
    /// read JSON from file
    AutoJson::Json config = AutoJson::Json::ReadFromFile("data/config.json");

    cities_file_path = (config["citiesFilePath"] | kDefaultCitiesFilePath).operator std::string();
    num_nodes = config["numNodes"] | kDefautNumNodes;
    gossip_factor = config["gossipFactor"] | kDefaultGossipFactor;
    corruption_chance = config["corruptionChance"] | kDefaultCorruptionChance;
    computing_time = config["computingTime"] | kComputingTime;
    latency = config["latency"] | kLatency;

    std::cout << "Reading city list..." << std::endl;
    std::ifstream cities_file(cities_file_path.c_str());
    int num_empty_cities = 0;
    int city_population;
    double city_longitude;
    double city_latitude;

    while (cities_file >> city_population >> city_latitude >> city_longitude) {
        cities.push_back({static_cast<long long>(city_population), {city_longitude, city_latitude}});
        if (city_population == 0) {
            num_empty_cities += 1;
        }
    }

    if (num_empty_cities > 0) {
        auto extra_population_to_distribute = static_cast<int>(1e9); // ONE BILLION PEOPLE
        int extra_population_for_city = extra_population_to_distribute / num_empty_cities;
        for (auto &city: cities) {
            if (city.first == 0) {
                city.first += extra_population_for_city;
            }
        }
    }
    for (int i = 1; i < cities.size(); ++ i) {
        cities[i].first += cities[i - 1].first;
    }
    std::cout << "Finished reading cities..." << std::endl;

    for (int i = 0; i < num_nodes; ++ i) {
        nodes.push_back(Node(i));
    }
}
