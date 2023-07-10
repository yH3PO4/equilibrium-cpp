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
    return this->freecost *
           (1.0 + alpha * pow((this->flow / this->capacity), beta));
}

double Network::EdgeProps::bpr(double _flow) {
    return this->freecost * (1.0 + alpha * pow((_flow / this->capacity), beta));
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

void Network::all_or_nothing(const size_t oVertexID, const size_t dVertexID,
                             const double flow) {
    std::vector<graph_t::edge_descriptor> path =
        shortest_path(oVertexID, dVertexID);
    for (const auto e : path) graph[e].newflow += flow;
}

void Network::update_all_flow() {
    auto edges = boost::edges(graph);
    for (auto eit = edges.first; eit != edges.second; ++eit) {
        // iterate all edges
        Network::EdgeProps &edge = graph[*eit];
        edge.flow = edge.newflow;
        edge.cost = edge.bpr();
    }
}

double Network::calc_z(double xi) {
    // z を計算する
    double z = 0.0;
    auto edges = boost::edges(graph);
    for (auto eit = edges.first; eit != edges.second; ++eit) {
        // iterate all edges
        Network::EdgeProps &edge = graph[*eit];
        double cost = edge.bpr(edge.flow * xi + edge.newflow * (1 - xi));
        z += cost * (edge.flow * xi + edge.newflow * (1 - xi));
    }
    return z;
}

double Network::update_optimal_flow(double minxi) {
    // リンクの交通量とコストを更新、収束判定値delta計算
    double delta = 0.0;
    auto edges = boost::edges(graph);
    for (auto eit = edges.first; eit != edges.second; ++eit) {
        // iterate all edges
        Network::EdgeProps &edge = graph[*eit];
        delta += abs(
            edge.flow * minxi + (edge.newflow) * (1 - minxi) -
            edge.flow);  // 最適な交通量から最初の交通量を引いた値の絶対値を足す
        edge.cost = edge.bpr(
            edge.flow * minxi +
            edge.newflow *
                (1 - minxi));  // 最適な交通量のときのリンクのコストを計算
        edge.newflow =
            edge.flow * minxi +
            edge.newflow * (1 - minxi);  // 最適な交通量を更新交通量に代入
    }
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

std::vector<Network::graph_t::edge_descriptor> Network::shortest_path(
    const size_t oVertexID, const size_t dVertexID) {
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
            std::cout << "経路が存在しません" << std::endl;
            return {};
        }
        std::vector<graph_t::edge_descriptor> route;
        graph_t::vertex_descriptor v = goal;
        while (v != start) {
            graph_t::vertex_descriptor p = parents.at(v);
            auto [e, flag] = boost::edge(p, v, graph);
            if (!flag) {
                std::cout << "経路復元に失敗しました" << std::endl;
                return {};
            }
            route.push_back(e);
            v = p;
        }
        std::reverse(route.begin(), route.end());  // 起点->終点ｎ
        return route;
    }
}