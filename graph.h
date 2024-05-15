#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>

#include <QObject>
#include <QPointF>
#include <QVector>

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
    Q_INVOKABLE QVector<QPointF> getIntermediateVertices() const;
    Q_INVOKABLE QVector<QPointF> getUnsafeZones() const;

signals:
    void dataLoaded();
private:
    void loadFromFile(const std::string& jsonPath);

    QPointF start_;
    QPointF finish_;
    QVector<QPointF> intermediateVertices_;
    QVector<QPointF> unsafeZones_;

};

#endif // GRAPH_H
