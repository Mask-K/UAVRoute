#ifndef ARTIFICIALBEECOLONY_H
#define ARTIFICIALBEECOLONY_H


#include <QVector>

#include <random>

class ArtificialBeeColony {
public:
    ArtificialBeeColony(const QVector<QVector<double>>& adjacencyMatrix,
                        int numBees = 20,
                        int numIterations = 500,
                        int limit = 100);

    QVector<int> findBestPath();

    double getBestLength() const;

private:
    const QVector<QVector<double>>& adjacencyMatrix_;
    int numBees_;
    int numIterations_;
    int limit_;
    int numEmployeeBees_;
    int numOnlookerBees_;
    double bestLength_;
    QVector<int> bestSolution_;
    std::mt19937 rng_;

    int chooseNextVertex(const QSet<int>& visited, int currentVertex);

    QVector<int> constructSolution();

    double calculateRouteLength(const QVector<int>& solution);

    int selectOnlooker(const QVector<double>& fitness);
};

#endif // ARTIFICIALBEECOLONY_H
