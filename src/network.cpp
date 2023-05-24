#include "network.hpp"

#include <cmath>

Node::Node(int _nodeID, double _lat, double _lon) {
    this->id = _nodeID;
    this->lat = _lat;
    this->lon = _lon;
}

Link::Link(int _id, int _oNodeID, int _dNodeID, int _laneCount, int _maxSpeed) {
    this->id = _id;
    this->oNodeID = _oNodeID;
    this->dNodeID = _dNodeID;
    this->laneCount = _laneCount;
    this->maxSpeed = _maxSpeed;
    this->type = LinkType::Road;  // TODO; Railwayも追加
    this->flow = 0;
    this->newflow = 0;
    this->cost = 0;
    this->freecost = 0;
    this->capacity = 0;
}

Graph::Graph(std::vector<Node> nodes, std::vector<Link> links) {
    for (const auto &node : nodes) {
        this->nodes.emplace(node.id, node);
    }
    for (const auto &link : links) {
        adj_list[link.oNodeID].emplace_back(link);
    }
}

void Graph::assignment(double &alpha, double &beta) {
    for (auto &[s, links] : this->adj_list) {
        for (auto &link : links) {
            link.flow = 0.0;
            link.newflow = 0.0;  // 全リンクの交通量・更新交通量を0にする
            link.cost = bpr(link.flow, link.freecost, link.capacity, alpha,
                            beta);  // BPR関数でコストを計算
        }
    }
}

double bpr(const double &flow, const double &freecost, const double &capacity,
           const double &alpha, const double &beta) {
    return freecost * (1.0 + alpha * std::pow((flow / capacity), beta));
}
