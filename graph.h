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

public:
    Graph(const std::string& jsonPath, QObject* parent = nullptr);

    Q_INVOKABLE QPointF getStart() const;
    Q_INVOKABLE QPointF getFinish() const;
    Q_INVOKABLE QVector<QPointF> getIntermediateVertices() const;
    Q_INVOKABLE QVector<QPointF> getUnsafeZones() const;

    QVector<QVector<double>> adjacencyMatrix() const;

    Q_INVOKABLE QPointF getVertex(int index) const;

signals:
    void dataLoaded();
private:
    void loadFromFile(const std::string& jsonPath);
    int size() const;
    bool isPointOnSegment(const QPointF& p, const QPointF& a, const QPointF& b) const;
    double calcDistance(const QPointF& left, const QPointF& right) const;

    QPointF start_;
    QPointF finish_;
    QVector<QPointF> intermediateVertices_;
    QVector<QPointF> unsafeZones_;

};

#endif // GRAPH_H
