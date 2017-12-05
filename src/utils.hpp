#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <cmath>
#include <cstdlib>

int PoissonNumber();

double LatencyDistribution();

std::pair<long long, std::pair<double, double>> RandomCity();

double DegToRad(double deg);

std::string GetLogFilePath();

#endif  // UTILS_HPP
