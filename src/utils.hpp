#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>

double Rand01();

int RandInt(int limit);

int PoissonNumber();

double LatencyDistribution();

std::pair<long long, std::pair<double, double>> RandomCity();

double DegToRad(double deg);

std::string GetLogFilePath();
