#ifndef NODE_HPP
#define NODE_HPP

class Node {
  private:
    double longitude, latitude;
    bool corrupt;
    int index;

    void init(int index);

  public:
    Node();
    explicit Node(int index);

    void reset(const double &corruptionChance);

    bool isCorrupt() const;
    void setCorrupt(const double &chance);

    double sphereDistance(const Node &other) const;
    double broadcastDuration(const Node &other) const;
};

#endif  // NODE_HPP
