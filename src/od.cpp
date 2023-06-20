#include "od.hpp"

OD::OD(double _olat, double _olon, double _dlat, double _dlon, double _flow) {
    this->oPoint = point_t(_olon, _olat);
    this->dPoint = point_t(_dlon, _dlat);
    this->flow = _flow;
}
