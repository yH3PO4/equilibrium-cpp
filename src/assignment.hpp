#include "network.hpp"
#include "od.hpp"

namespace assignment {
NearestNode calc_nearest_node(const OD &od, const Graph &graph);
void assignment(Graph &road_graph, const std::vector<OD> &ods);
}  // namespace assignment