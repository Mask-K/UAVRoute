#include "graph.h"

#include <cmath>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

Graph::Graph(const std::string& jsonPath)
{
    std::ifstream f(jsonPath);


    nlohmann::json data = nlohmann::json::parse(f);

    start_ = {data["start"]["x"], data["start"]["y"]};
    finish_ = {data["finish"]["x"], data["finish"]["y"]};

    intermediateVertices_.reserve(data["nodes"].size());
    for(const auto& node : data["nodes"]){
        intermediateVertices_.push_back({node["x"], node["y"], node["is_safe"]});
    }
    f.close();
}

double Graph::calcDistance(const Vertex& left, const Vertex& right){
    int dx = right.x - left.x;
    int dy = right.y - left.y;
    return std::sqrt(dx * dx + dy * dy);
}
