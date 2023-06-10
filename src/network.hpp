#pragma once

#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <unordered_map>

namespace bg = boost::geometry;
typedef bg::model::point<double, 2, bg::cs::geographic<bg::degree>> point;

class Network {
   public:
    struct VertexProps {
        point lonlat;
        VertexProps(); // これないとコンパイル通らない
        VertexProps(double _lat, double _lon);
    };

    struct EdgeProps {
        int laneCount, maxSpeed;
        double flow, newflow, cost, freecost, capacity;
        EdgeProps(); // これないとコンパイル通らない
        EdgeProps(int _laneCount, int _maxSpeed);
    };

    typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS,
                                  VertexProps, EdgeProps>
        Graph;
    Graph graph;
    void add_vertex(const int vertexID, const VertexProps &vertex_props);
    void add_edge(const int edgeID, const int oVertexID, const int dVertexID,
                  const EdgeProps &edge_props);

   private:
    std::unordered_map<int, Graph::vertex_descriptor> v_desc;
    std::unordered_map<int, Graph::edge_descriptor> e_desc;
};