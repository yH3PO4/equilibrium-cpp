#include <iostream>

#include "assignment.hpp"
#include "io.hpp"

int main() {
    const std::string in_vertex_path =
        "Input_data/Tokyo/TokyoNode.csv";
    const std::string in_edge_path =
        "Input_data/Tokyo/TokyoLink.csv";
    const std::string od_path =
        "Input_data/Tokyo/TokyoOD.csv";
    Network network = io::read_network(in_vertex_path, in_edge_path);
    std::vector<OD> ods = io::read_od(od_path);

    // assignment::assignment(road_graph, ods);
}
