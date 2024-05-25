#ifndef ALGORITHMRESULTS_H
#define ALGORITHMRESULTS_H

#include <QObject>

class AlgorithmResults : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector<int> bestPath READ bestPath WRITE setBestPath NOTIFY bestPathChanged)
    Q_PROPERTY(double bestLength READ bestLength WRITE setBestLength NOTIFY bestLengthChanged)

public:
    AlgorithmResults(QObject *parent = nullptr);

    QVector<int> bestPath() const;
    void setBestPath(const QVector<int> &path);

    double bestLength() const;
    void setBestLength(double length);

signals:
    void bestPathChanged();
    void bestLengthChanged();

private:
    QVector<int> bestPath_;
    double bestLength_;
};

#endif // ALGORITHMRESULTS_H
