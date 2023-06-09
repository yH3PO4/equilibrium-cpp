#include "od.hpp"

OD::OD(double _olat, double _olon, double _dlat, double _dlon, double _flow) {
    this->olat = _olat;
    this->olon = _olon;
    this->dlat = _dlat;
    this->dlon = _dlon;
    this->flow = _flow;
}
