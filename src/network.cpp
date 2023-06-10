#include "network.hpp"

#include <cmath>

Node::Node(int _nodeID, double _lat, double _lon) {
    this->id = _nodeID;
    this->lonlat = point(_lon, _lat);
}

Link::Link(int _id, int _laneCount, int _maxSpeed) {
    this->id = _id;
    this->laneCount = _laneCount;
    this->maxSpeed = _maxSpeed;
    this->flow = 0;
    this->newflow = 0;
    this->cost = 0;
    this->freecost = 0;
    this->capacity = 0;
}


std::vector<Link> Graph::shortest_path(const int &oNodeID, const int &dNodeID) {
    return std::vector<Link>{};
}

void Graph::initialize_flow() {
    for (auto &[s, links] : this->adj_list) {
        for (auto &link : links) {
            link.flow = 0.0;
            link.newflow = 0.0;  // 全リンクの交通量・更新交通量を0にする
            link.cost = this->bpr(link.flow, link.freecost,
                                  link.capacity);  // BPR関数でコストを計算
        }
    }
}

double Graph::bpr(const double &flow, const double &freecost,
                  const double &capacity) {
    return freecost *
           (1.0 + this->alpha * std::pow((flow / capacity), this->beta));
}
