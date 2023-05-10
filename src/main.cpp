#include <iostream>

#include "network.hpp"
#include "io.hpp"

int main()
{
    const std::string in_node_path = "/home/hayashi/programs/equilibrium-cpp/Input_data/Tokyo/TokyoNode.csv";
    const std::string in_link_path = "/home/hayashi/programs/equilibrium-cpp/Input_data/Tokyo/TokyoLink.csv";
    Graph graph = io::read_network(in_node_path, in_link_path);
}
