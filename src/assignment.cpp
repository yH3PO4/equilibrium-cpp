#include "assignment.hpp"

#include <cmath>

double assignment::toRadians(double degree) { return degree * M_PI / 180.0; }

double assignment::calculateDistance(double lat1, double lon1, double lat2,
                                     double lon2) {
    const double earthRadius = 6371.0;  // 地球の半径（キロメートル）

    // 緯度と経度をラジアンに変換
    double lat1Rad = toRadians(lat1);
    double lon1Rad = toRadians(lon1);
    double lat2Rad = toRadians(lat2);
    double lon2Rad = toRadians(lon2);

    // ヒュベニの公式に基づいて距離を計算
    double deltaLat = lat2Rad - lat1Rad;
    double deltaLon = lon2Rad - lon1Rad;
    double a = std::sin(deltaLat / 2) * std::sin(deltaLat / 2) +
               std::cos(lat1Rad) * std::cos(lat2Rad) * std::sin(deltaLon / 2) *
                   std::sin(deltaLon / 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    double distance = earthRadius * c;
    return distance;  // km単位
}

NearestNode assignment::findNearestNode(const OD &od, const Graph &graph) {
    // TODO: kd-treeで高速化
    int oNodeID = 0, dNodeID = 0;
    double oDist = INFINITY, dDist = INFINITY;
    for (const auto &[nodeID, node] : graph.nodes) {
        {
            double tmp_oDist =
                calculateDistance(od.olat, od.olon, node.lat, node.lon);
            if (tmp_oDist < oDist) {
                oDist = tmp_oDist;
                oNodeID = nodeID;
            }
        }
        {
            double tmp_dDist =
                calculateDistance(od.dlat, od.dlon, node.lat, node.lon);
            if (tmp_dDist < dDist) {
                dDist = tmp_dDist;
                dNodeID = nodeID;
            }
        }
    }
    return NearestNode{oNodeID, dNodeID};
};

void assignment::assignment(Graph &road_graph, const std::vector<OD> &ods) {
    road_graph.initialize_flow();
    for (const auto &od : ods) {
        auto links = road_graph.shortest_path(od.road_nearest_node.oNode,
                                              od.road_nearest_node.oNode);
    }
    //     for (po = od; po != NULL; po = po->next) { //全ODについて
    //     linklist = dijkstra(network, po->onode, po->dnode);
    //     //Dijkstra法で最短経路を求めて

    //     for (pll = linklist; pll != NULL; pll = pll->next) {
    //         pll->link->newflow += po->flow;
    //         //その経路に全OD交通量を流し、それを各リンクの更新交通量に代入する
    //     }

    //     free_linklist(linklist); //linklistを削除
    // }
}