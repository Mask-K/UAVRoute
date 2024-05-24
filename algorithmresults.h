#ifndef ALGORITHMRESULTS_H
#define ALGORITHMRESULTS_H

#include <QObject>

class AlgorithmResults : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector<int> bestPath READ bestPath WRITE setBestPath NOTIFY bestPathChanged)
    Q_PROPERTY(double bestLength READ bestLength WRITE setBestLength NOTIFY bestLengthChanged)

public:
    explicit AlgorithmResults(QObject *parent = nullptr)
        : QObject(parent), m_bestLength(0.0) {}

    QVector<int> bestPath() const { return m_bestPath; }
    void setBestPath(const QVector<int> &path) {
        if (m_bestPath != path) {
            m_bestPath = path;
            emit bestPathChanged();
        }
    }

    double bestLength() const { return m_bestLength; }
    void setBestLength(double length) {
        if (m_bestLength != length) {
            m_bestLength = length;
            emit bestLengthChanged();
        }
    }

signals:
    void bestPathChanged();
    void bestLengthChanged();

private:
    QVector<int> m_bestPath;
    double m_bestLength;
};

#endif // ALGORITHMRESULTS_H
