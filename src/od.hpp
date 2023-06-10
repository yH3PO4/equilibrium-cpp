#pragma once
#include <unordered_map>

struct OD {
    double olat;
    double olon;
    double dlat;
    double dlon;
    double flow;
    int oVertexID;
    int dVertexID;
    OD(double _olat, double _olon, double _dlat, double _dlon, double _flow);
};