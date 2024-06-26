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

    QVector<QVector<double>> distanceMatrix() const;

    Q_INVOKABLE QPointF getVertex(int index) const;
    Q_INVOKABLE QVector<double> getUnsafeZonesRadiuses() const;

signals:
    void dataLoaded();
private:
    void loadFromFile(const std::string& jsonPath);
    int size() const;
    double calcDistance(const QPointF& left, const QPointF& right) const;
    bool isSegmentIntersectingCircle(const QPointF& p1, const QPointF& p2, const QPointF& center, double radius) const;

    QPointF start_;
    QPointF finish_;
    QVector<QPointF> intermediateVertices_;
    QVector<QPointF> unsafeZones_;
    QVector<double> unsafeZonesRadiuses_;

};

#endif // GRAPH_H
