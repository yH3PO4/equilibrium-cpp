#pragma once

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
    static const double alpha;
    static const double beta;

    void add_vertex(const size_t vertexID, const VertexProps &vertex_props);
    void add_edge(const size_t edgeID, const size_t oVertexID, const size_t dVertexID,
                  const EdgeProps &edge_props);
    bgi::rtree<std::pair<point_t, size_t>, bgi::quadratic<16>>
     generate_rtree() const;

   private:
    graph_t graph;
    std::unordered_map<size_t, graph_t::vertex_descriptor> v_desc;
    std::unordered_map<size_t, graph_t::edge_descriptor> e_desc;
};