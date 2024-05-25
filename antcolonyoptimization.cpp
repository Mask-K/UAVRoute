#include "antcolonyoptimization.h"

#include <QSet>
#include <cmath>
#include <limits>
#include <numeric>

AntColonyOptimization::AntColonyOptimization(const QVector<QVector<double>>& adjacencyMatrix,
                      int numAnts, int numIterations, double alpha, double beta, double evaporationRate, double pheromoneConstant)
    : adjacencyMatrix_(adjacencyMatrix), numAnts_(numAnts), numIterations_(numIterations),
    alpha_(alpha), beta_(beta), evaporationRate_(evaporationRate), pheromoneConstant_(pheromoneConstant), rng_(std::random_device{}()) {}

QVector<int> AntColonyOptimization::findBestPath() {
    QVector<QVector<double>> pheromoneMatrix(adjacencyMatrix_.size(), QVector<double>(adjacencyMatrix_.size(), 1.0));

    QVector<int> bestSolution;
    double bestLength = std::numeric_limits<double>::infinity();

    for (int iteration = 0; iteration < numIterations_; ++iteration) {
        QVector<QVector<int>> solutions;
        for (int ant = 0; ant < numAnts_; ++ant) {
            QVector<int> solution = constructSolution(pheromoneMatrix);
            solutions.append(solution);
            double length = calculateRouteLength(solution);
            if (length < bestLength) {
                bestLength = length;
                bestSolution = solution;
            }
        }
        updatePheromones(pheromoneMatrix, solutions);
    }

    return bestSolution;
}

double AntColonyOptimization::calculateRouteLength(const QVector<int>& solution) {
    double length = 0.0;
    for (int i = 0; i < solution.size() - 1; ++i) {
        length += adjacencyMatrix_[solution[i]][solution[i + 1]];
    }
    return length;
}

int AntColonyOptimization::chooseNextVertex(const QVector<QVector<double>>& pheromoneMatrix,const QSet<int>& visited, int currentVertex) {
    int numVertices = pheromoneMatrix.size();
    std::vector<double> probabilities(numVertices, 0.0);

    double total = 0.0;
    for (int i = 0; i < numVertices; ++i) {
        if (!visited.contains(i) && adjacencyMatrix_[currentVertex][i] < std::numeric_limits<double>::infinity()) {
            double tau = std::pow(pheromoneMatrix[currentVertex][i], alpha_);
            double eta = std::pow(1.0 / adjacencyMatrix_[currentVertex][i], beta_);
            probabilities[i] = tau * eta;
            total += probabilities[i];
        }
    }

    if (total == 0.0) {
        std::vector<int> unvisitedVertices;
        for (int i = 0; i < numVertices; ++i) {
            if (!visited.contains(i)) {
                unvisitedVertices.push_back(i);
            }
        }
        std::uniform_int_distribution<int> dist(0, unvisitedVertices.size() - 1);
        return unvisitedVertices[dist(rng_)];
    }

    for (double& prob : probabilities) {
        prob /= total;
    }

    std::discrete_distribution<> dist(probabilities.begin(), probabilities.end());
    return dist(rng_);
}

QVector<int> AntColonyOptimization::constructSolution(QVector<QVector<double>>& pheromoneMatrix) {
    QSet<int> visited;
    int currentVertex = 0;
    QVector<int> solution = {currentVertex};
    visited.insert(currentVertex);

    while (currentVertex != adjacencyMatrix_.size() - 1) {
        int nextVertex = chooseNextVertex(pheromoneMatrix, visited, currentVertex);
        solution.append(nextVertex);
        visited.insert(nextVertex);
        currentVertex = nextVertex;
    }

    return solution;
}

void AntColonyOptimization::updatePheromones(QVector<QVector<double>>& pheromoneMatrix, const QVector<QVector<int>>& solutions) {
    for (auto& row : pheromoneMatrix) {
        for (double& pheromone : row) {
            pheromone *= (1.0 - evaporationRate_);
        }
    }

    for (const QVector<int>& solution : solutions) {
        double routeLength = calculateRouteLength(solution);
        for (int i = 0; i < solution.size() - 1; ++i) {
            int from = solution[i];
            int to = solution[i + 1];
            pheromoneMatrix[from][to] += pheromoneConstant_ / routeLength;
            pheromoneMatrix[to][from] += pheromoneConstant_ / routeLength;
        }
    }
}
