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
        intermediateVertices_.push_back({node["x"], node["y"]});
    }

    for(const auto& node: data["unsafeZones"]){
        unsafeZones_.push_back({node["x"], node["y"]});
        unsafeZonesRadiuses_.push_back(node["radius"]);
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

QVector<QVector<double>> Graph::distanceMatrix() const{
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

                for(int l = 0; l < unsafeZones_.size(); ++l){
                    if(isSegmentIntersectingCircle(left, right, unsafeZones_[l], unsafeZonesRadiuses_[l])){
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


QPointF Graph::getVertex(int index) const{
    if (index < 0 || index >= size()) {
        throw std::out_of_range("Index out of range");
    }
    if(index == 0){
        return start_;
    }
    if(index == size() - 1){
        return finish_;
    }
    if(index <= unsafeZones_.size()){
        return unsafeZones_[index - 1];
    }

    return intermediateVertices_[index - unsafeZones_.size() - 1];
}

bool Graph::isSegmentIntersectingCircle(const QPointF& p1, const QPointF& p2, const QPointF& center, double radius) const {
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    double fx = p1.x() - center.x();
    double fy = p1.y() - center.y();

    double a = dx * dx + dy * dy;
    double b = 2 * (fx * dx + fy * dy);
    double c = (fx * fx + fy * fy) - radius * radius;

    double discriminant = b * b - 4 * a * c;

    if (discriminant >= 0) {
        discriminant = std::sqrt(discriminant);

        double t1 = (-b - discriminant) / (2 * a);
        double t2 = (-b + discriminant) / (2 * a);

        if ((t1 >= 0 && t1 <= 1) || (t2 >= 0 && t2 <= 1)) {
            return true;
        }
    }


    double dist1 = std::sqrt(fx * fx + fy * fy);
    double dist2 = std::sqrt((p2.x() - center.x()) * (p2.x() - center.x()) + (p2.y() - center.y()) * (p2.y() - center.y()));

    return dist1 <= radius && dist2 <= radius;

}

QVector<double> Graph::getUnsafeZonesRadiuses() const{
    return    unsafeZonesRadiuses_;
}

