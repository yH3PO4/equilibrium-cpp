#include "assignment.hpp"

NearestNode assignment::calc_nearest_node(const OD &od, const Graph &graph){

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