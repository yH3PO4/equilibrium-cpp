#pragma once

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/graph/adjacency_list.hpp>

namespace bg = boost::geometry;
typedef bg::model::point<double, 2, bg::cs::geographic<bg::degree>> point;

struct Node {
    int id;
    point lonlat;
    Node(int _nodeID, double _lat, double _lon);
};

struct Link {
    int id, laneCount, maxSpeed;
    double flow, newflow, cost, freecost, capacity;
    Link(int _id, int _laneCount, int _maxSpeed);
};

typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, Node,
                              Link>
    Graph;