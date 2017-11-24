#pragma once

#include <vector>

class DijkstraHeap {
private:
    std::vector<std::pair<double, int>> heap;
    std::vector<int> position;

    // Neighbour related queries
    inline int Parent(const int &index) const;
    inline int LeftSon(const int &index) const;
    inline int RightSon(const int &index) const;
    inline bool HasLeftSon(const int &index) const;
    inline bool HasRightSon(const int &index) const;
    inline bool IsLeaf(const int &index) const;

    // Own heap-node related queries (fail if index is outside heap size)
    inline double GetKey(const int &index) const;
    inline int GetNodeIndex(const int &index) const;

    // Methods that change the heap's structure
    inline void SwapNodes(const int &index1, const int &index2);
    void Sift(int index);
    void Percolate(int index);

public:
    DijkstraHeap();
    DijkstraHeap(int num_nodes);

    // API

    // This function either adds the node to the heap if it didn't exist or changes its key if it does.
    // It is tuned for the Dijkstra algorithm, so only decrease in key is supported.
    void Push(const double &key, const int &node_index);

    int Pop();

    bool Empty() const;
};
