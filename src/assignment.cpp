#include "assignment.hpp"

void assignment::assignment(Network &network, const std::vector<OD> &ods) {
    // for (const auto &od : ods) {
    //     auto edges = network.shortest_path(od.road_nearest_vertex.oVertex,
    //                                        od.road_nearest_vertex.oVertex);
    // }
    //     for (po = od; po != NULL; po = po->next) { //全ODについて
    //     edgelist = dijkstra(network, po->oVertex, po->dVertex);
    //     //Dijkstra法で最短経路を求めて

    //     for (pll = edgelist; pll != NULL; pll = pll->next) {
    //         pll->edge->newflow += po->flow;
    //         //その経路に全OD交通量を流し、それを各リンクの更新交通量に代入する
    //     }

    //     free_edgelist(edgelist); //edgelistを削除
    // }
}

void assignment::set_nearest_vertex(const Network &network, std::vector<OD> &ods) {
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