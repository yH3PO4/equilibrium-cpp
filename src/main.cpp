#include <iostream>

#include "assignment.hpp"
#include "io.hpp"

int main() {
    const std::string in_node_path =
        "/home/hayashi/programs/equilibrium-cpp/Input_data/Tokyo/TokyoNode.csv";
    const std::string in_link_path =
        "/home/hayashi/programs/equilibrium-cpp/Input_data/Tokyo/TokyoLink.csv";
    const std::string od_path =
        "/home/hayashi/programs/equilibrium-cpp/Input_data/Tokyo/TokyoOD.csv";
    Graph road_graph = io::read_network(in_node_path, in_link_path);
    std::vector<OD> ods = io::read_od(od_path);

    for (auto& od : ods) {
        od.road_nearest_node = assignment::findNearestNode(od, road_graph);
    }
    // assignment::assignment(road_graph, ods);
}
