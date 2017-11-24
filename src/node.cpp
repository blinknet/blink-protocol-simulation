#include "node.hpp"

#include <map>
#include <queue>

#include "utils.hpp"
#include "globals.hpp"

Node::Node() {
    this->Init(0);
}

Node::Node(int index) {
    this->Init(index);
}

void Node::Init(int index) {
    const auto city = RandomCity();
    this->longitude = DegToRad(city.second.first);
    this->latitude = DegToRad(city.second.second);
    this->SetCorrupt(corruption_chance);
    this->index = index;
}

void Node::Reset(const double &corruption_chance) {
    const auto city = RandomCity();
    this->longitude = DegToRad(city.second.first);
    this->latitude = DegToRad(city.second.second);
    this->SetCorrupt(corruption_chance);
}

bool Node::IsCorrupt() const {
    return this->corrupt;
}

void Node::SetCorrupt(const double &chance) {
    this->corrupt = Rand01() <= chance;
}

double Node::SphereDistance(const Node &other) const {
    return acos(sin(longitude) * sin(other.longitude) + cos(longitude) * cos(other.longitude) * cos(fabs(latitude - other.latitude)));
}

double Node::BroadcastDuration(const Node &other) const {
    const double dist = this->SphereDistance(other);
    return (computing_time + latency * dist) * LatencyDistribution();
}
