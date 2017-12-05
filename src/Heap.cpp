#include "Heap.hpp"


// Constructors
DijkstraHeap::DijkstraHeap() = default;

DijkstraHeap::DijkstraHeap(int numNodes) {
    this->heap.reserve((unsigned int)numNodes);
    this->position.assign(numNodes, -1);
}

// Neighbour query methods
int DijkstraHeap::parent(const int &index) const {
    return (index - 1) / 2;
}

int DijkstraHeap::leftSon(const int &index) const {
    return 2 * index + 1;
}

int DijkstraHeap::rightSon(const int &index) const {
    return 2 * index + 2;
}

bool DijkstraHeap::hasLeftSon(const int &index) const {
    return this->leftSon(index) < (int)this->heap.size();
}

bool DijkstraHeap::hasRightSon(const int &index) const {
    return this->rightSon(index) < (int)this->heap.size();
}

bool DijkstraHeap::isLeaf(const int &index) const {
    return !this->hasLeftSon(index);
}

// Own heap node queries
double DijkstraHeap::getKey(const int &index) const {
    return this->heap[index].first;
}

int DijkstraHeap::getNodeIndex(const int &index) const {
    return this->heap[index].second;
}

// Methods that modify the heap structure
void DijkstraHeap::swapNodes(const int &index1, const int &index2) {
    this->position[this->getNodeIndex(index1)] = index2;
    this->position[this->getNodeIndex(index2)] = index1;
    std::swap(this->heap[index1], this->heap[index2]);
}

void DijkstraHeap::sift(int index) {
    while (index > 0 && this->getKey(index) < this->getKey(this->parent(index))) {
        this->swapNodes(index, this->parent(index));
        index = this->parent(index);
    }
}

void DijkstraHeap::percolate(int index) {
    while (!this->isLeaf(index)) {
        int sonWithLowestKey = this->leftSon(index);
        double sonLowestKey = this->getKey(sonWithLowestKey);
        if (this->hasRightSon(index)) {
            double rightSonKey = this->getKey(this->rightSon(index));
            if (rightSonKey < sonLowestKey) {
                sonWithLowestKey = this->rightSon(index);
                sonLowestKey = rightSonKey;
            }
        }
        if (sonLowestKey < this->getKey(index)) {
            this->swapNodes(index, sonWithLowestKey);
            index = sonWithLowestKey;
        } else {
            break;
        }
    }
}


// API
void DijkstraHeap::push(const double &key, const int &nodeIndex) {
    if (this->position[nodeIndex] != -1) {
        // We already have it, change its key
        this->heap[this->position[nodeIndex]].first = key;
        // Now sift it up from where it is until the heap is valid again
        this->sift(this->position[nodeIndex]);

        return;
    }
    // We don't have this node, add it to the heap
    this->position[nodeIndex] = (int)this->heap.size();
    this->heap.push_back({key, nodeIndex});
    // sift it up
    this->sift(this->position[nodeIndex]);
}

int DijkstraHeap::pop() {
    int returnNodeIndex = this->heap[0].second;

    this->swapNodes(0, (int)this->heap.size() - 1);
    this->position[returnNodeIndex] = -1;
    this->heap.pop_back();

    // percolate down the node from the root
    this->percolate(0);

    return returnNodeIndex;
}

bool DijkstraHeap::empty() const {
    return this->heap.empty();
}
