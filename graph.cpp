#include "graph.h"

#include <cmath>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

Graph::Graph(const std::string& jsonPath, QObject* parent) : QObject(parent)
{
    loadFromFile(jsonPath);
}

double Graph::calcDistance(const QPointF& left, const QPointF& right) const{
    double dx = right.x() - left.x();
    double dy = right.y() - left.y();
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

QVector<QPointF> Graph::getIntermediateVertices() const{
    return intermediateVertices_;
}

QVector<QPointF> Graph::getUnsafeZones() const{
    return unsafeZones_;
}

QVector<QVector<double>> Graph::adjacencyMatrix() const{
    int n = size();
    int unsafeSize = unsafeZones_.size();

    QVector<QVector<double>> matrix(n, QVector<double>(n, std::numeric_limits<double>::infinity()));

    for(int i = 0; i < n; ++i){
        for(int j = i; j < n; ++j){
            if(i == j){
                matrix[i][j] = 0;
            }
            else if(i > 0 && i <= unsafeSize || j > 0 && j <= unsafeSize){ //if either of vertices is unsafe zone
                continue;
            }
            else{
                QPointF left = (i == 0) ? start_ : intermediateVertices_[i - 1 - unsafeSize];
                QPointF right = (j == n - 1) ? finish_ : intermediateVertices_[j - 1 - unsafeSize];

                bool safe = true;
                for(const auto& p : unsafeZones_){
                    if(isPointOnSegment(p, left, right)){
                        safe = false;
                        break;
                    }
                }

                if(safe){
                    matrix[i][j] = matrix[j][i] = calcDistance(left, right);
                }
            }
        }
    }

    return matrix;
}

bool Graph::isPointOnSegment(const QPointF& p, const QPointF& a, const QPointF& b) const{
    // A margin of error to account for floating-point constraints
    const double epsilon = 1e-9;

    double crossProduct = (a.x() - b.x()) * (p.y() - b.y()) - (p.x() - b.x()) * (a.y() - b.y());
    if (std::abs(crossProduct) > epsilon) {
        return false;
    }

    if (p.x() < std::min(a.x(), b.x()) - epsilon || p.x() > std::max(a.x(), b.x()) + epsilon) {
        return false;
    }
    if (p.y() < std::min(a.y(), b.y()) - epsilon || p.y() > std::max(a.y(), b.y()) + epsilon) {
        return false;
    }

    return true;
}



