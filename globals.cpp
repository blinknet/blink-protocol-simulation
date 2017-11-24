#include "globals.hpp"

double computing_time;
double latency;
double corruption_chance;

size_t num_nodes;
size_t gossip_factor;

std::vector<std::pair<long long, std::pair<double, double>>> cities;
std::vector<Node> nodes;
//EventQueue time_line;
