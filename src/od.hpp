#pragma once

#include <unordered_map>

struct NearestVertex {
    int oVertex;
    int dVertex;
};

struct OD {
    double olat;
    double olon;
    double dlat;
    double dlon;
    double flow;
    NearestVertex road_nearest_vertex;
    OD(double _olat, double _olon, double _dlat, double _dlon, double _flow);
};