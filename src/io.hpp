#include "network.hpp"
#include "od.hpp"

namespace io {
std::vector<std::string> split(const std::string &row, const char delim);
void read_vertex(const std::string &in_vertex_path, Network &network);
void read_edge(const std::string &in_edge_path, Network &network);
Network read_network(const std::string &in_vertex_path,
                     const std::string &in_edge_path);
std::vector<OD> read_od(const std::string &od_path);
}  // namespace io
