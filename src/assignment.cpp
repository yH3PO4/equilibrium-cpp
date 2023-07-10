#include "assignment.hpp"

#include <iostream>

void assignment::assignment(Network &network, const std::vector<OD> &ods) {
    constexpr double GR = 1.6180339887;
    constexpr double THRES_DELTA = 1.0;
    constexpr double THRES_XI = 0.0001;

    for (const auto &od : ods) {
        network.all_or_nothing(od.oVertexID, od.dVertexID, od.flow);
    }
    double delta = DBL_MAX;
    int n = 0;
    std::cout << delta << std::endl;
    while (delta > THRES_DELTA) {
        n++;
        network.update_all_flow();
        for (const auto &od : ods) {
            network.all_or_nothing(od.oVertexID, od.dVertexID, od.flow);
        }
        // 黄金分割法
        double xi1 = 0.0;
        double xi2 = 1.0;
        double xi3 = (xi1 * GR + xi2 * 1.0) / (1.0 + GR);
        double xi4 = (xi1 * 1.0 + xi2 * GR) / (1.0 + GR);
        while (abs(xi1 - xi2) > THRES_XI) {
            if (network.calc_z(xi3) < network.calc_z(xi4)) {
                xi2 = xi4;
                xi4 = xi3;
                xi3 = (xi1 * GR + xi2 * 1.0) / (1.0 + GR);
            } else {
                xi1 = xi3;
                xi3 = xi4;
                xi4 = (xi1 * 1.0 + xi2 * GR) / (1.0 + GR);
            }
        }
        double minxi = xi2; //最小になるときのxi
        double minz = network.calc_z(minxi); //最小になるときの目的関数
        delta = network.update_optimal_flow(minxi);
        std::cout << delta << " " << minz << std::endl;
    }
}

void assignment::set_nearest_vertex(const Network &network,
                                    std::vector<OD> &ods) {
    const auto rtree = network.generate_rtree();
    for (auto &od : ods) {
        std::vector<std::pair<point_t, size_t>> res{};
        rtree.query(bgi::nearest(od.oPoint, 1), std::back_inserter(res));
        od.oVertexID = res[0].second;
        res.clear();
        rtree.query(bgi::nearest(od.dPoint, 1), std::back_inserter(res));
        od.dVertexID = res[0].second;
    }
}