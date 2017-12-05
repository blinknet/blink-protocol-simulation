#pragma once

#include <cmath>
#include <cstdlib>

#include <algorithm>
#include <string>
#include <utility>

double Rand01();

int RandInt(int limit);

int PoissonNumber();

double LatencyDistribution();

std::pair<long long, std::pair<double, double>> RandomCity();

double DegToRad(double deg);

std::string GetLogFolderPath();
std::string GetLogFilePath();
