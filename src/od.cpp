#include "od.hpp"

OD::OD(double _olat, double _olon, double _dlat, double _dlon, double _flow) {
    this->oPoint = point_t(_olat, _olon);
    this->dPoint = point_t(_dlat, _dlon);
    this->flow = _flow;
}
