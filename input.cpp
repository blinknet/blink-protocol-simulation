#include "input.hpp"

#include <fstream>
#include <iostream>

#include "globals.hpp"

// TODO: CHANGE THIS TO CONFIG FILE PARSING

void read_data() {
    // TODO: Change for compatibility with Linux / VIM
    const std::string CITIES_FILE_PATH = "../cities.txt";

    std::cout << "Use default values? (nodes=10,000; gossip=16; corrupt=10%; computing time=20ms; latency=100ms) [Y/n]";
    char answer;
    std::cin.get(answer);
    if (answer == 'N' || answer == 'n') {
        std::cout << "Number of nodes: ";
        std::cin >> num_nodes;
        std::cout << "GossipFactor: ";
        std::cin >> gossip_factor;
        std::cout << "Chance for a node to be not transmit: ";
        std::cin >> corruption_chance;
        std::cout << "Computing time for a transmission: ";
        std::cin >> computing_time;
        std::cout << "Latency to travel 6,371km (Earth Radius): ";
        std::cin >> latency;
    } else {
        num_nodes = 10000;
        gossip_factor = 16;
        corruption_chance = 0.1;
        computing_time = 20;
        latency = 100;
    }

    std::cout << "Reading city list..." << std::endl;
    std::ifstream cities_file(CITIES_FILE_PATH.c_str());
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
