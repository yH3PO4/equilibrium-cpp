#include "network.hpp"
#include "od.hpp"

namespace assignment {
double toRadians(double degree);
double calculateDistance(double lat1, double lon1, double lat2, double lon2);
NearestNode findNearestNode(const OD &od, const Graph &graph);
void assignment(Graph &road_graph, const std::vector<OD> &ods);
}  // namespace assignment