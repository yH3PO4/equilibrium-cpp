#pragma once

#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/graph/adjacency_list.hpp>
// https://stackoverflow.com/questions/46156995/boost-1-65-1-geometry-distance-strategy-compile-error-with-visual-studio-2017
#include <boost/geometry/algorithms/comparable_distance.hpp>
#include <boost/geometry/strategies/strategies.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;
typedef bg::model::point<double, 2, bg::cs::geographic<bg::degree>>
    point_t;  // (lat, lon)
