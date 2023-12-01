#include "network.hpp"

#include <cmath>
#include <iostream>

Network::VertexProps::VertexProps() {}

Network::VertexProps::VertexProps(size_t vertexID, double _lat, double _lon) {
    this->outerID = vertexID;
    this->lonlat = point_t(_lat, _lon);
}

Network::EdgeProps::EdgeProps() {}

Network::EdgeProps::EdgeProps(size_t edgeID, int _laneCount, int _maxSpeed,
                              double _length) {
    this->outerID = edgeID;
    this->laneCount = _laneCount;
    this->maxSpeed = _maxSpeed;
    this->length = _length;
    this->capacity = _laneCount * gamma * c;
    this->flow = 0;
    this->newflow = 0;
    // リンクの長さと制限速度から自由旅行時間を計算して格納
    this->cost = (_length / (_maxSpeed / 3.6)) / 60;
    this->freecost = (_length / (_maxSpeed / 3.6)) / 60;
}

double Network::EdgeProps::bpr() {
    return this->freecost *
           (1.0 + alpha * pow((this->flow / this->capacity), beta));
}

double Network::EdgeProps::bpr(double _flow) {
    return this->freecost * (1.0 + alpha * pow((_flow / this->capacity), beta));
}

void Network::add_vertex(const VertexProps &vertex_props) {
    auto v = boost::add_vertex(vertex_props, this->graph);
    this->v_desc[vertex_props.outerID] = v;
}

void Network::add_edge(const size_t oVertexID, const size_t dVertexID,
                       const EdgeProps &edge_props) {
    auto [e, flag] =
        boost::add_edge(this->v_desc.at(oVertexID), this->v_desc.at(dVertexID),
                        edge_props, this->graph);
    if (flag) {
        this->e_desc[edge_props.outerID] = e;
    } else {
        std::cout << "Failed to add edge with edge_id " << edge_props.outerID
                  << std::endl;
    }
}

int Network::num_vertices() { return boost::num_vertices(this->graph); }

int Network::num_edges() { return boost::num_edges(this->graph); }

const double Network::calc_length(int oVertexID, int dVertexID) {
    graph_t::vertex_descriptor oVertex, dVertex;
    try {
        oVertex = this->v_desc.at(oVertexID);
    } catch (std::out_of_range &ex) {
        std::cout << oVertexID << " is not in graph" << std::endl;
        return 0;
    }
    try {
        dVertex = this->v_desc.at(dVertexID);
    } catch (std::out_of_range &ex) {
        std::cout << dVertexID << " is not in graph" << std::endl;
        return 0;
    }

    double length =
        bg::distance(this->graph[oVertex].lonlat, this->graph[dVertex].lonlat);
    return length;
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
    for (const auto &e : path) graph[e].newflow += flow;
}

void Network::update_all_flow() {
    auto edges = boost::edges(graph);
    for (auto eit = edges.first; eit != edges.second; ++eit) {
        // iterate all edges
        Network::EdgeProps &edge = graph[*eit];
        edge.flow = edge.newflow;
        edge.cost = edge.bpr();
        edge.newflow = 0.0;
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
    return delta;
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
    std::cout << "経路が存在しません" << std::endl;
    return {};
}

void Network::set_result() {
    auto edges = boost::edges(graph);
    for (auto eit = edges.first; eit != edges.second; ++eit) {
        // iterate all edges
        Network::EdgeProps &edge = graph[*eit];
        edge.flow = edge.newflow;
        edge.cost = edge.bpr();
    }
}

const std::vector<
    std::tuple<int, int, double, double, int, double, double, double>>
Network::get_link_flow() const {
    std::vector<
        std::tuple<int, int, double, double, int, double, double, double>>
        res;
    for (const auto &[edge_ID, edge] : this->e_desc) {
        const VertexProps &source = this->graph[edge.m_source];
        const VertexProps &target = this->graph[edge.m_target];
        const double oVertexID = source.outerID;
        const double oLat = source.lonlat.get<0>();
        const double oLon = source.lonlat.get<1>();
        const double dVertexID = target.outerID;
        const double dLat = target.lonlat.get<0>();
        const double dLon = target.lonlat.get<1>();
        const double flow = this->graph[edge].flow;
        res.emplace_back(edge_ID, oVertexID, oLat, oLon, dVertexID, dLat,
                         dLon, flow);
    }
    return res;
}
