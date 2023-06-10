#include "network.hpp"

#include <cmath>
#include <iostream>

Network::VertexProps::VertexProps() {}

Network::VertexProps::VertexProps(double _lat, double _lon) {
    this->lonlat = point(_lon, _lat);
}

Network::EdgeProps::EdgeProps() {
    this->flow = 0;
    this->newflow = 0;
    this->cost = bpr();
    this->freecost = 0;
    this->capacity = 0;
}

Network::EdgeProps::EdgeProps(int _laneCount, int _maxSpeed) {
    this->laneCount = _laneCount;
    this->maxSpeed = _maxSpeed;
    this->flow = 0;
    this->newflow = 0;
    this->cost = bpr();
    this->freecost = 0;
    this->capacity = 0;
}

double Network::EdgeProps::bpr() {
    return this->freecost * (1.0 + alpha * pow((flow / capacity), beta));
}

void Network::add_vertex(const int vertexID, const VertexProps &vertex_props) {
    auto v = boost::add_vertex(vertex_props, this->graph);
    this->v_desc[vertexID] = v;
}

void Network::add_edge(const int edgeID, const int oVertexID,
                       const int dVertexID, const EdgeProps &edge_props) {
    auto [e, flag] =
        boost::add_edge(this->v_desc.at(oVertexID), this->v_desc.at(dVertexID),
                        edge_props, this->graph);
    if (flag) {
        this->e_desc[edgeID] = e;
    } else {
        std::cout << "Failed to add edge with edge_id " << edgeID << std::endl;
    }
}