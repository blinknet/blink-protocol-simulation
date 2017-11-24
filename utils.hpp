#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>

double rand01();

int rand_int(int limit);

int poisson_number();

double latency_distribution();

std::pair<long long, std::pair<double, double>> random_city();

double deg_to_rad(double deg);

std::string get_log_file_path();
