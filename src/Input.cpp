#include "Input.hpp"

#include <fstream>
#include <iostream>
#include <vector>

#include "Globals.hpp"

#include "third_party/autojson/src/lib/json.hpp"

const std::string kDefaultCitiesFilePath = "data/cities.txt";
const std::string kDefaultLogsRootFolderPath = "logs/";
const int kDefautNumNodes = 10000;
const int kDefaultGossipFactor = 16;
const double kDefaultCorruptionChance = 0.1;
const double kComputingTime = 20;
const double kLatency = 100;

void ReadData() {
    /// read JSON from file
    AutoJson::Json config = AutoJson::Json::ReadFromFile("../data/config.json");

    citiesFilePath = (config["citiesFilePath"] | kDefaultCitiesFilePath).operator std::string();
    logsRootFolderPath = (config["logsRootFolder"] | kDefaultLogsRootFolderPath).operator std::string();
    numNodes = config["numNodes"] | kDefautNumNodes;
    gossipFactor = config["gossipFactor"] | kDefaultGossipFactor;
    corruptionChance = config["corruptionChance"] | kDefaultCorruptionChance;
    computingTime = config["computingTime"] | kComputingTime;
    latency = config["latency"] | kLatency;

    std::cout << "Reading city list..." << std::endl;
    std::ifstream citiesFile(citiesFilePath.c_str());
    int numEmptyCities = 0;
    int cityPopulation;
    double cityLongitude;
    double cityLatitude;

    while (citiesFile >> cityPopulation >> cityLatitude >> cityLongitude) {
        cities.push_back({static_cast<long long>(cityPopulation), {cityLongitude, cityLatitude}});
        if (cityPopulation == 0) {
            numEmptyCities += 1;
        }
    }

    if (numEmptyCities > 0) {
        auto extraPopulationToDistribute = static_cast<int>(1e9);  // ONE BILLION PEOPLE
        int extraPopulationForCity = extraPopulationToDistribute / numEmptyCities;
        for (auto &city : cities) {
            if (city.first == 0) {
                city.first += extraPopulationForCity;
            }
        }
    }
    for (int i = 1; i < (int)cities.size(); ++i) {
        cities[i].first += cities[i - 1].first;
    }
    std::cout << "Finished reading cities..." << std::endl;

    for (size_t i = 0; i < numNodes; ++i) {
        nodes.emplace_back(Node((int)i));
    }
}
