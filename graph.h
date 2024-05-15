#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>

#include <QObject>
#include <QPointF>

class Graph : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(Vertex start READ getStart CONSTANT)
//    Q_PROPERTY(Vertex finish READ getFinish CONSTANT)

public:
    Graph(const std::string& jsonPath, QObject* parent = nullptr);

    struct Vertex {
        double x;
        double y;
    };

    double calcDistance(const Vertex& left, const Vertex& right);

    Q_INVOKABLE int size() const;
    Q_INVOKABLE QPointF getStart() const;
    Q_INVOKABLE QPointF getFinish() const;

signals:
    void dataLoaded();
private:
    void loadFromFile(const std::string& jsonPath);

    QPointF start_;
    QPointF finish_;
    std::vector<Vertex> intermediateVertices_;
    std::vector<Vertex> unsafeZones_;
};

#endif // GRAPH_H
