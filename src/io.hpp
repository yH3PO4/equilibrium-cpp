#include "network.hpp"

namespace io
{
    std::vector<std::string> split(const std::string &row, const char delim);
    std::vector<Node> read_node(const std::string &in_node_path);
    std::vector<Link> read_link(const std::string &in_link_path);
    Graph read_network(const std::string &in_node_path, const std::string &in_link_path);
}
