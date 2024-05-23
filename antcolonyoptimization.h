#ifndef ANTCOLONYOPTIMIZATION_H
#define ANTCOLONYOPTIMIZATION_H

#include <QVector>
#include <QSet>
#include <cmath>
#include <limits>
#include <random>
#include <numeric>

class AntColonyOptimization {
public:
    AntColonyOptimization(const QVector<QVector<double>>& adjacencyMatrix,
                          int numAnts = 50,
                          int numIterations = 500,
                          double alpha = 1.0,
                          double beta = 2.0,
                          double evaporationRate = 0.5,
                          double pheromoneConstant = 1.0)
        : adjacencyMatrix(adjacencyMatrix), numAnts(numAnts), numIterations(numIterations),
        alpha(alpha), beta(beta), evaporationRate(evaporationRate), pheromoneConstant(pheromoneConstant), rng(std::random_device{}()) {}

    QVector<int> findBestPath() {
        QVector<QVector<double>> pheromoneMatrix(adjacencyMatrix.size(), QVector<double>(adjacencyMatrix.size(), 1.0));

        QVector<int> bestSolution;
        double bestLength = std::numeric_limits<double>::infinity();

        for (int iteration = 0; iteration < numIterations; ++iteration) {
            QVector<QVector<int>> solutions;
            for (int ant = 0; ant < numAnts; ++ant) {
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

    double calculateRouteLength(const QVector<int>& solution) {
        double length = 0.0;
        for (int i = 0; i < solution.size() - 1; ++i) {
            length += adjacencyMatrix[solution[i]][solution[i + 1]];
        }
        return length;
    }

private:
    const QVector<QVector<double>>& adjacencyMatrix;
    const int numAnts;
    const int numIterations;
    const double alpha;
    const double beta;
    const double evaporationRate;
    const double pheromoneConstant;
    std::mt19937 rng;

    int chooseNextVertex(const QVector<QVector<double>>& pheromoneMatrix,const QSet<int>& visited, int currentVertex) {
        int numVertices = pheromoneMatrix.size();
        std::vector<double> probabilities(numVertices, 0.0);

        double total = 0.0;
        for (int i = 0; i < numVertices; ++i) {
            if (!visited.contains(i) && adjacencyMatrix[currentVertex][i] < std::numeric_limits<double>::infinity()) {
                double tau = std::pow(pheromoneMatrix[currentVertex][i], alpha);
                double eta = std::pow(1.0 / adjacencyMatrix[currentVertex][i], beta);
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
            return unvisitedVertices[dist(rng)];
        }

        for (double& prob : probabilities) {
            prob /= total;
        }

        std::discrete_distribution<> dist(probabilities.begin(), probabilities.end());
        return dist(rng);
    }

    QVector<int> constructSolution(QVector<QVector<double>>& pheromoneMatrix) {
        QSet<int> visited;
        int currentVertex = 0;
        QVector<int> solution = {currentVertex};
        visited.insert(currentVertex);

        while (currentVertex != adjacencyMatrix.size() - 1) {
            int nextVertex = chooseNextVertex(pheromoneMatrix, visited, currentVertex);
            solution.append(nextVertex);
            visited.insert(nextVertex);
            currentVertex = nextVertex;
        }

        return solution;
    }

    void updatePheromones(QVector<QVector<double>>& pheromoneMatrix, const QVector<QVector<int>>& solutions) {
        for (auto& row : pheromoneMatrix) {
            for (double& pheromone : row) {
                pheromone *= (1.0 - evaporationRate);
            }
        }

        for (const QVector<int>& solution : solutions) {
            double routeLength = calculateRouteLength(solution);
            for (int i = 0; i < solution.size() - 1; ++i) {
                int from = solution[i];
                int to = solution[i + 1];
                pheromoneMatrix[from][to] += pheromoneConstant / routeLength;
                pheromoneMatrix[to][from] += pheromoneConstant / routeLength;
            }
        }
    }


};


#endif // ANTCOLONYOPTIMIZATION_H
