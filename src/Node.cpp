#include "Node.hpp"

#include "Globals.hpp"
#include "Utils.hpp"

#include "third_party/cpp-base/src/cppbase/Random.hpp"

Node::Node() {
    this->init(0);
}

Node::Node(int index) {
    this->init(index);
}

void Node::init(int index) {
    const auto city = RandomCity();
    this->longitude = DegToRad(city.second.first);
    this->latitude = DegToRad(city.second.second);
    this->setCorrupt(corruptionChance);
    this->index = index;
}

void Node::reset(const double &corruptionChance) {
    const auto city = RandomCity();
    this->longitude = DegToRad(city.second.first);
    this->latitude = DegToRad(city.second.second);
    this->setCorrupt(corruptionChance);
}

bool Node::isCorrupt() const {
    return this->corrupt;
}

void Node::setCorrupt(const double &chance) {
    this->corrupt = (base::RandFloat64() < chance);
}

double Node::sphereDistance(const Node &other) const {
    return acos(sin(longitude) * sin(other.longitude) + cos(longitude) * cos(other.longitude) * cos(fabs(latitude - other.latitude)));
}

double Node::broadcastDuration(const Node &other) const {
    const double dist = this->sphereDistance(other);
    return (computingTime + latency * dist) * LatencyDistribution();
}
