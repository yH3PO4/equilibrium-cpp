#pragma once
#include <unordered_map>

#include "geometry.hpp"

struct OD {
    point_t oPoint;
    point_t dPoint;
    double flow;
    unsigned int oVertexID;
    unsigned int dVertexID;
    OD(double _olon, double _olat, double _dlon, double _dlat, double _flow);
};