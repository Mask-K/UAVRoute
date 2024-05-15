#include "graph.h"

#include <cmath>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

Graph::Graph(const std::string& jsonPath, QObject* parent) : QObject(parent)
{
    qRegisterMetaType<Graph::Vertex>();
    loadFromFile(jsonPath);
}

double Graph::calcDistance(const Vertex& left, const Vertex& right){
    int dx = right.x - left.x;
    int dy = right.y - left.y;
    return std::sqrt(dx * dx + dy * dy);
}

int Graph::size() const{
    return intermediateVertices_.size() + unsafeZones_.size() + 2;
}

void Graph::loadFromFile(const std::string& jsonPath){
    std::ifstream f(jsonPath);

    nlohmann::json data = nlohmann::json::parse(f);

    start_ = {data["start"]["x"], data["start"]["y"]};
    finish_ = {data["finish"]["x"], data["finish"]["y"]};

    intermediateVertices_.reserve(data["nodes"].size());
    unsafeZones_.reserve(data["nodes"].size());
    for(const auto& node : data["nodes"]){
        if(node["is_safe"]){
            intermediateVertices_.push_back({node["x"], node["y"]});
        }
        else{
            unsafeZones_.push_back({node["x"], node["y"]});
        }
    }

    f.close();

    emit dataLoaded();
}

QPointF Graph::getStart() const{
    return start_;
}

QPointF Graph::getFinish() const{
    return finish_;
}



