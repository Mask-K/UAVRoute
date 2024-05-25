#include "artificialbeecolony.h"

#include <QSet>
#include <limits>

ArtificialBeeColony::ArtificialBeeColony(const QVector<QVector<double>>& adjacencyMatrix,
                                         int numBees, int numIterations, int limit) : adjacencyMatrix_(adjacencyMatrix), numBees_(numBees),
    numIterations_(numIterations), limit_(limit), rng_(std::random_device{}()) {
    numEmployeeBees_ = numBees / 2;
    numOnlookerBees_ = numBees - numEmployeeBees_;
}

QVector<int> ArtificialBeeColony::findBestPath() {
    bestLength_ = std::numeric_limits<double>::infinity();
    QVector<QVector<int>> solutions(numBees_);
    QVector<double> fitness(numBees_);
    QVector<int> trial(numBees_, 0);


    for (int i = 0; i < numBees_; ++i) {
        solutions[i] = constructSolution();
        fitness[i] = 1.0 / calculateRouteLength(solutions[i]);
    }

    for (int iteration = 0; iteration < numIterations_; ++iteration) {

        for (int i = 0; i < numEmployeeBees_; ++i) {
            QVector<int> newSolution = constructSolution();
            double newLength = calculateRouteLength(newSolution);
            if (1.0 / newLength > fitness[i]) {
                solutions[i] = newSolution;
                fitness[i] = 1.0 / newLength;
                trial[i] = 0;
            } else {
                trial[i] += 1;
            }
            if (newLength < bestLength_) {
                bestLength_ = newLength;
                bestSolution_ = newSolution;
            }
        }


        for (int i = 0; i < numOnlookerBees_; ++i) {
            int k = selectOnlooker(fitness);
            QVector<int> newSolution = constructSolution();
            double newLength = calculateRouteLength(newSolution);
            if (1.0 / newLength > fitness[k]) {
                solutions[k] = newSolution;
                fitness[k] = 1.0 / newLength;
                trial[k] = 0;
            } else {
                trial[k] += 1;
            }
            if (newLength < bestLength_) {
                bestLength_ = newLength;
                bestSolution_ = newSolution;
            }
        }


        for (int i = 0; i < numBees_; ++i) {
            if (trial[i] > limit_) {
                solutions[i] = constructSolution();
                fitness[i] = 1.0 / calculateRouteLength(solutions[i]);
                trial[i] = 0;
            }
        }
    }

    return bestSolution_;
}

double ArtificialBeeColony::getBestLength() const {
    return bestLength_;
}

int ArtificialBeeColony::chooseNextVertex(const QSet<int>& visited, int currentVertex) {
    int numVertices = adjacencyMatrix_.size();
    std::vector<double> probabilities(numVertices, 0.0);
    double total = 0.0;

    for (int i = 0; i < numVertices; ++i) {
        if (!visited.contains(i) && adjacencyMatrix_[currentVertex][i] < std::numeric_limits<double>::infinity()) {
            double eta = 1.0 / adjacencyMatrix_[currentVertex][i];
            probabilities[i] = eta;
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

QVector<int> ArtificialBeeColony::constructSolution() {
    QSet<int> visited;
    int currentVertex = 0;
    QVector<int> solution = {currentVertex};
    visited.insert(currentVertex);

    while (currentVertex != adjacencyMatrix_.size() - 1) {
        int nextVertex = chooseNextVertex(visited, currentVertex);
        solution.push_back(nextVertex);
        visited.insert(nextVertex);
        currentVertex = nextVertex;
    }

    return solution;
}

double ArtificialBeeColony::calculateRouteLength(const QVector<int>& solution) {
    double length = 0.0;
    for (int i = 0; i < solution.size() - 1; ++i) {
        length += adjacencyMatrix_[solution[i]][solution[i + 1]];
    }
    return length;
}

int ArtificialBeeColony::selectOnlooker(const QVector<double>& fitness) {
    std::vector<double> stdFitness(fitness.begin(), fitness.end());
    std::discrete_distribution<> dist(stdFitness.begin(), stdFitness.end());
    return dist(rng_);
}
