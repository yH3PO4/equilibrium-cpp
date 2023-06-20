#include "network.hpp"
#include "od.hpp"

namespace assignment {
void assignment(Network &network, const std::vector<OD> &ods);
void set_nearest_vertex(const Network &network, std::vector<OD> &ods);
}  // namespace assignment