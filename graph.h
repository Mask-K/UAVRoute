#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>

class Graph
{
private:
    struct Vertex {
        double x;
        double y;
    };

    Vertex start_;
    Vertex finish_;
    std::vector<Vertex> intermediateVertices_;
    std::vector<Vertex> unsafeZones_;

public:
    Graph(const std::string& jsonPath);

    double calcDistance(const Vertex& left, const Vertex& right);
};

#endif // GRAPH_H
