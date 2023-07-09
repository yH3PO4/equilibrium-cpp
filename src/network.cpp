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
    this->freecost = 1;  // TODO: 距離計算
    this->cost = bpr();
    this->capacity = 0;
}

Network::EdgeProps::EdgeProps(int _laneCount, int _maxSpeed) {
    this->laneCount = _laneCount;
    this->maxSpeed = _maxSpeed;
    this->flow = 0;
    this->newflow = 0;
    this->cost = 0;
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

bgi::rtree<std::pair<point_t, size_t>, bgi::quadratic<16>>
Network::generate_rtree() const {
    bgi::rtree<std::pair<point_t, size_t>, bgi::quadratic<16>> rtree;
    for (const auto &v : this->v_desc) {
        rtree.insert(std::make_pair(this->graph[v.second].lonlat, v.first));
    }
    return rtree;
}

void Network::update_flow(const size_t oVertexID, const size_t dVertexID) {
    auto res = shortest_path(oVertexID, dVertexID);
}

Network::distance_heuristic::distance_heuristic(graph_t::vertex_descriptor goal,
                                                graph_t &graph)
    : goal_(goal), graph_(graph){};

double Network::distance_heuristic::operator()(
    graph_t::vertex_descriptor u) const {
    // heuristic なコスト計算
    return bg::distance(graph_[goal_].lonlat, graph_[u].lonlat);
}

Network::astar_goal_visitor::astar_goal_visitor(graph_t::vertex_descriptor goal)
    : m_goal(goal){};

void Network::astar_goal_visitor::examine_vertex(graph_t::vertex_descriptor u,
                                                 const graph_t &g) {
    if (u == m_goal) throw found_goal();
}

std::deque<size_t> Network::shortest_path(const size_t oVertexID,
                                          const size_t dVertexID) {
    std::vector<graph_t::vertex_descriptor> parents(
        boost::num_vertices(graph));  // これにO(N)かかるか要調査

    graph_t::vertex_descriptor start = this->v_desc.at(oVertexID);
    graph_t::vertex_descriptor goal = this->v_desc.at(dVertexID);
    if (start == goal) return {};
    try {
        boost::astar_search(this->graph, start, distance_heuristic(goal, graph),
                            boost::predecessor_map(&parents[0])
                                .weight_map(boost::get(&EdgeProps::cost, graph))
                                .visitor(astar_goal_visitor(goal)));
    } catch (found_goal fg) {
        if (parents.at(goal) == goal) {
            std::cout << "no path" << std::endl;
            return {};
        }
        std::deque<graph_t::vertex_descriptor> route;
        for (graph_t::vertex_descriptor v = goal; v != start; v = parents[v]) {
            route.push_front(v);
        }
        route.push_front(start);
        return route;
    }
}