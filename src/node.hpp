#pragma once

class Node {
private:
    double longitude, latitude;
    bool corrupt;
    int index;

    void Init(int index);

public:
    Node();
    Node(int index);

    void Reset(const double &corruption_chance);

    bool IsCorrupt() const;
    void SetCorrupt(const double &chance);

    double SphereDistance(const Node &other) const;
    double BroadcastDuration(const Node &other) const;
};
