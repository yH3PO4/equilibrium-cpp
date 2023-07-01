#include "network.hpp"

#include <cmath>
#include <iostream>

Network::VertexProps::VertexProps() {}

Network::VertexProps::VertexProps(double _lat, double _lon) {
    this->lonlat = point_t(_lat, _lon);
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

void Network::add_vertex(const size_t vertexID,
                         const VertexProps &vertex_props) {
    auto v = boost::add_vertex(vertex_props, this->graph);
    this->v_desc[vertexID] = v;
}

void Network::add_edge(const size_t edgeID, const size_t oVertexID,
                       const size_t dVertexID, const EdgeProps &edge_props) {
    auto [e, flag] =
        boost::add_edge(this->v_desc.at(oVertexID), this->v_desc.at(dVertexID),
                        edge_props, this->graph);
    if (flag) {
        this->e_desc[edgeID] = e;
    } else {
        std::cout << "Failed to add edge with edge_id " << edgeID << std::endl;
    }
}

Network::distance_heuristic::distance_heuristic(graph_t::vertex_descriptor goal,
                                                graph_t &graph)
    : goal_(goal), graph_(graph){};
double Network::distance_heuristic::operator()(
    graph_t::vertex_descriptor u) const {
    // heuristic なコスト計算
    return get(boost::vertex_index, graph_, goal_) -
           get(boost::vertex_index, graph_, u);
}

std::vector<size_t> Network::shortest_path(const size_t oVertexID,
                                           const size_t dVertexID) {
    std::unordered_map<graph_t::vertex_descriptor, graph_t::vertex_descriptor>
        parents;
    std::unordered_map<graph_t::vertex_descriptor, double> distances;

    // try {
    //     boost::astar_search_tree(
    //         this->graph, this->v_desc.at(oVertexID),
    //         distance_heuristic<Graph, Cost>(goal, g),
    //         boost::predecessor_map(&parents[0])
    //             .distance_map(&distances[0])
    //             .visitor(astar_goal_visitor<Vertex>(goal)));
    // }
}

bgi::rtree<std::pair<point_t, size_t>, bgi::quadratic<16>>
Network::generate_rtree() const {
    bgi::rtree<std::pair<point_t, size_t>, bgi::quadratic<16>> rtree;
    for (const auto &v : this->v_desc) {
        rtree.insert(std::make_pair(this->graph[v.second].lonlat, v.first));
    }
    return rtree;
}