#include "heap.hpp"


// Constructors
DijkstraHeap::DijkstraHeap() = default;

DijkstraHeap::DijkstraHeap(int num_nodes) {
    this->heap.reserve(num_nodes);
    this->position.assign(num_nodes, -1);
}


// Neighbour query methods
int DijkstraHeap::Parent(const int &index) const {
    return (index - 1) / 2;
}

int DijkstraHeap::LeftSon(const int &index) const {
    return 2 * index + 1;
}

int DijkstraHeap::RightSon(const int &index) const {
    return 2 * index + 2;
}

bool DijkstraHeap::HasLeftSon(const int &index) const {
    return this->LeftSon(index) < this->heap.size();
}

bool DijkstraHeap::HasRightSon(const int &index) const {
    return this->RightSon(index) < this->heap.size();
}

bool DijkstraHeap::IsLeaf(const int &index) const {
    return !this->HasLeftSon(index);
}


// Own heap node queries
double DijkstraHeap::GetKey(const int &index) const {
    return this->heap[index].first;
}

int DijkstraHeap::GetNodeIndex(const int &index) const {
    return this->heap[index].second;
}


// Methods that modify the heap structure
void DijkstraHeap::SwapNodes(const int &index1, const int &index2) {
    this->position[this->GetNodeIndex(index1)] = index2;
    this->position[this->GetNodeIndex(index2)] = index1;
    std::swap(this->heap[index1], this->heap[index2]);
}

void DijkstraHeap::Sift(int index) {
    while (index > 0 && this->GetKey(index) < this->GetKey(this->Parent(index))) {
        this->SwapNodes(index, this->Parent(index));
        index = this->Parent(index);
    }
}

void DijkstraHeap::Percolate(int index) {
    while (!this->IsLeaf(index)) {
        int son_with_lowest_key = this->LeftSon(index);
        double son_lowest_key = this->GetKey(son_with_lowest_key);
        if (this->HasRightSon(index)) {
            double right_son_key = this->GetKey(this->RightSon(index));
            if (right_son_key < son_lowest_key) {
                son_with_lowest_key = this->RightSon(index);
                son_lowest_key = right_son_key;
            }
        }
        if (son_lowest_key < this->GetKey(index)) {
            this->SwapNodes(index, son_with_lowest_key);
            index = son_with_lowest_key;
        } else {
            break;
        }
    }
}


// API
void DijkstraHeap::Push(const double &key, const int &node_index) {
    if (this->position[node_index] != -1) {
        // We already have it, change its key
        this->heap[this->position[node_index]].first = key;
        // Now Sift it up from where it is until the heap is valid again
        this->Sift(this->position[node_index]);

        return;
    }
    // We don't have this node, add it to the heap
    this->position[node_index] = this->heap.size();
    this->heap.push_back({key, node_index});
    // Sift it up
    this->Sift(this->position[node_index]);
}

int DijkstraHeap::Pop() {
    int return_node_index = this->heap[0].second;

    this->SwapNodes(0, this->heap.size() - 1);
    this->position[return_node_index] = -1;
    this->heap.pop_back();

    // Percolate down the node from the root
    this->Percolate(0);

    return return_node_index;
}

bool DijkstraHeap::Empty() const {
    return this->heap.empty();
}
