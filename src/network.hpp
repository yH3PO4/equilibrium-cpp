#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/astar_search.hpp>
#include <unordered_map>

#include "geometry.hpp"

class Network {
   public:
    struct VertexProps {
        point_t lonlat;
        VertexProps();  // これないとコンパイル通らない
        VertexProps(double _lat, double _lon);
    };

    struct EdgeProps {
        int laneCount, maxSpeed;
        double flow, newflow, cost, freecost, capacity;
        EdgeProps();  // これないとコンパイル通らない
        EdgeProps(int _laneCount, int _maxSpeed);
        double bpr();
    };
    typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS,
                                  VertexProps, EdgeProps>
        graph_t;

    class distance_heuristic : public boost::astar_heuristic<graph_t, double> {
       public:
        distance_heuristic(graph_t::vertex_descriptor goal, graph_t& graph);
        double operator()(graph_t::vertex_descriptor u) const;

       private:
        graph_t::vertex_descriptor goal_;
        graph_t& graph_;
    };

    class astar_goal_visitor : public boost::default_astar_visitor {
       public:
        astar_goal_visitor(graph_t::vertex_descriptor goal);
        void examine_vertex(graph_t::vertex_descriptor u, const graph_t& g);

       private:
        graph_t::vertex_descriptor m_goal;
    };

    struct found_goal {};

    static const double alpha;
    static const double beta;

    void add_vertex(const size_t vertexID, const VertexProps& vertex_props);
    void add_edge(const size_t edgeID, const size_t oVertexID,
                  const size_t dVertexID, const EdgeProps& edge_props);
    bgi::rtree<std::pair<point_t, size_t>, bgi::quadratic<16>> generate_rtree()
        const;
    void update_flow(const size_t oVertexID, const size_t dVertexID,
                     const double flow);

   private:
    graph_t graph;
    std::unordered_map<size_t, graph_t::vertex_descriptor> v_desc;
    std::unordered_map<size_t, graph_t::edge_descriptor> e_desc;
    std::vector<graph_t::edge_descriptor> shortest_path(const size_t oVertexID,
                                                        const size_t dVertexID);
};