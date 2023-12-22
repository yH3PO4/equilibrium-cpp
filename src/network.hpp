#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/astar_search.hpp>
#include <unordered_map>

#include "geometry.hpp"

class Network {
   public:
    struct VertexProps {
        unsigned int outerID;
        point_t lonlat;
        VertexProps();  // これないとコンパイル通らない
        VertexProps(unsigned int vertexID, double _lon, double _lat);
    };
    struct EdgeProps {
        unsigned int outerID;
        int laneCount, maxSpeed;
        double length, capacity;
        double flow, newflow, cost, freecost;
        EdgeProps();  // これないとコンパイル通らない
        EdgeProps(unsigned int edgeID, int _laneCount, int _maxSpeed,
                  double _length);
        double bpr() const;
        double bpr(double _flow) const;
    };
    typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS,
                                  VertexProps, EdgeProps>
        graph_t;

    class distance_heuristic : public boost::astar_heuristic<graph_t, double> {
       public:
        distance_heuristic(const graph_t::vertex_descriptor goal,
                           const graph_t& graph);
        double operator()(graph_t::vertex_descriptor u) const;

       private:
        const graph_t::vertex_descriptor goal_;
        const graph_t& graph_;
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
    static const double gamma;
    static const double c;

    void add_vertex(const VertexProps& vertex_props);
    void add_edge(const unsigned int oVertexID, const unsigned int dVertexID,
                  const EdgeProps& edge_props);
    int num_vertices() const;
    int num_edges() const;
    double calc_length(int oVertexID, int dVertexID) const;
    bgi::rtree<std::pair<point_t, unsigned int>, bgi::quadratic<16>>
    generate_rtree() const;
    void all_or_nothing(const unsigned int oVertexID,
                        const unsigned int dVertexID, const double flow);
    void update_all_flow();
    double calc_z(double xi) const;
    double update_optimal_flow(double minxi);
    void set_result();
    std::vector<std::tuple<VertexProps, VertexProps, EdgeProps>> get_link_flow()
        const;

   private:
    graph_t graph;
    std::unordered_map<unsigned int, graph_t::vertex_descriptor> v_desc;
    std::vector<graph_t::edge_descriptor> shortest_path(
        unsigned int oVertexID, unsigned int dVertexID) const;
};