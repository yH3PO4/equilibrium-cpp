#pragma once

#include <unordered_map>

struct NearestNode {
    int oNode;
    int dNode;
};

struct OD {
    double olat;
    double olon;
    double dlat;
    double dlon;
    double flow;
    NearestNode road_nearest_node;
    OD(double _olat, double _olon, double _dlat, double _dlon, double _flow);
};