#include "assignment.hpp"

// void assignment::assignment(Graph &road_graph, const std::vector<OD> &ods) {
//     road_graph.initialize_flow();
//     for (const auto &od : ods) {
//         auto edges = road_graph.shortest_path(od.road_nearest_vertex.oVertex,
//                                               od.road_nearest_vertex.oVertex);
//     }
//     //     for (po = od; po != NULL; po = po->next) { //全ODについて
//     //     edgelist = dijkstra(network, po->oVertex, po->dVertex);
//     //     //Dijkstra法で最短経路を求めて

//     //     for (pll = edgelist; pll != NULL; pll = pll->next) {
//     //         pll->edge->newflow += po->flow;
//     //         //その経路に全OD交通量を流し、それを各リンクの更新交通量に代入する
//     //     }

//     //     free_edgelist(edgelist); //edgelistを削除
//     // }
// }