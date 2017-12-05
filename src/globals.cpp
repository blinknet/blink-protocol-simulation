#include "globals.hpp"

double computingTime;
double latency;
double corruptionChance;

size_t numNodes;
size_t gossipFactor;

std::vector<std::pair<long long, std::pair<double, double>>> cities;
std::vector<Node> nodes;

std::string citiesFilePath;
