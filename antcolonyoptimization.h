#ifndef ANTCOLONYOPTIMIZATION_H
#define ANTCOLONYOPTIMIZATION_H

#include <QVector>

#include <random>


class AntColonyOptimization {
public:
    AntColonyOptimization(const QVector<QVector<double>>& adjacencyMatrix,
                          int numAnts = 50,
                          int numIterations = 500,
                          double alpha = 1.0,
                          double beta = 2.0,
                          double evaporationRate = 0.5,
                          double pheromoneConstant = 1.0);
    QVector<int> findBestPath();

    double calculateRouteLength(const QVector<int>& solution);

private:
    const QVector<QVector<double>>& adjacencyMatrix_;
    const int numAnts_;
    const int numIterations_;
    const double alpha_;
    const double beta_;
    const double evaporationRate_;
    const double pheromoneConstant_;
    std::mt19937 rng_;

    int chooseNextVertex(const QVector<QVector<double>>& pheromoneMatrix,const QSet<int>& visited, int currentVertex);

    QVector<int> constructSolution(QVector<QVector<double>>& pheromoneMatrix);

    void updatePheromones(QVector<QVector<double>>& pheromoneMatrix, const QVector<QVector<int>>& solutions);


};


#endif // ANTCOLONYOPTIMIZATION_H
