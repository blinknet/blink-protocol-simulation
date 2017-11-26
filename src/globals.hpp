#pragma once

#include <cstdlib>
#include <vector>
#include <string>

#include "node.hpp"

extern double computing_time;
extern double latency;
extern double corruption_chance;

extern size_t num_nodes;
extern size_t gossip_factor;

extern std::vector<std::pair<long long, std::pair<double, double>>> cities;
extern std::vector<Node> nodes;

extern std::string cities_file_path;
