#ifndef HEAP_HPP
#define HEAP_HPP

#include <vector>

class DijkstraHeap {
  private:
    std::vector<std::pair<double, int>> heap;
    std::vector<int> position;

    // Neighbour related queries
    int parent(const int &index) const;
    int leftSon(const int &index) const;
    int rightSon(const int &index) const;
    bool hasLeftSon(const int &index) const;
    bool hasRightSon(const int &index) const;
    bool isLeaf(const int &index) const;

    // Own heap-node related queries (fail if index is outside heap size)
    double getKey(const int &index) const;
    int getNodeIndex(const int &index) const;

    // Methods that change the heap's structure
    void swapNodes(const int &index1, const int &index2);
    void sift(int index);
    void percolate(int index);

  public:
    DijkstraHeap();
    DijkstraHeap(int numNodes);

    // API

    // This function either adds the node to the heap if it didn't exist or changes its key if it does.
    // It is tuned for the Dijkstra algorithm, so only decrease in key is supported.
    void push(const double &key, const int &nodeIndex);

    int pop();

    bool empty() const;
};

#endif  // HEAP_HPP
