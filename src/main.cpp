#include <iostream>

#include "assignment.hpp"
#include "io.hpp"

const double Network::alpha = 0.48;  // BPR関数のパラメータ
const double Network::beta = 2.52;   // BPR関数のパラメータ

int main() {
    const std::string in_vertex_path = "Input_data/Tokyo/TokyoNode.csv";
    const std::string in_edge_path = "Input_data/Tokyo/TokyoLink.csv";
    const std::string od_path = "Input_data/Tokyo/TokyoOD.csv";

    const double gamma = 16.0;  // 日換算係数
    const double c = 350.0;     // 1車線あたりの交通容量

    Network network = io::read_network(in_vertex_path, in_edge_path);
    std::vector<OD> ods = io::read_od(od_path);
    assignment::set_nearest_vertex(network, ods);
    assignment::assignment(network, ods);
}
