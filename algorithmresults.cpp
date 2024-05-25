#include "algorithmresults.h"

AlgorithmResults::AlgorithmResults(QObject *parent) : QObject(parent), bestLength_(0.0) {}

QVector<int> AlgorithmResults::bestPath() const {
    return bestPath_;
}

void AlgorithmResults::setBestPath(const QVector<int> &path) {
    if (bestPath_ != path) {
        bestPath_ = path;
        emit bestPathChanged();
    }
}

double AlgorithmResults::bestLength() const{
    return bestLength_;
}

void AlgorithmResults::setBestLength(double length) {
    if (bestLength_ != length) {
        bestLength_ = length;
        emit bestLengthChanged();
    }
}
