#ifndef ARTIFICIALBEECOLONY_H
#define ARTIFICIALBEECOLONY_H


#include <QVector>
#include <QSet>
#include <limits>
#include <random>

class ArtificialBeeColony {
public:
    ArtificialBeeColony(const QVector<QVector<double>>& adjacencyMatrix,
                        int numBees = 20,
                        int numIterations = 500,
                        int limit = 100)
        : adjacencyMatrix(adjacencyMatrix), numBees(numBees), numIterations(numIterations),
        limit(limit), rng(std::random_device{}()) {
        numEmployeeBees = numBees / 2;
        numOnlookerBees = numBees - numEmployeeBees;
    }

    QVector<int> findBestPath() {
        bestLength = std::numeric_limits<double>::infinity();
        QVector<QVector<int>> solutions(numBees);
        QVector<double> fitness(numBees);
        QVector<int> trial(numBees, 0);


        for (int i = 0; i < numBees; ++i) {
            solutions[i] = constructSolution();
            fitness[i] = 1.0 / calculateRouteLength(solutions[i]);
        }

        for (int iteration = 0; iteration < numIterations; ++iteration) {

            for (int i = 0; i < numEmployeeBees; ++i) {
                QVector<int> newSolution = constructSolution();
                double newLength = calculateRouteLength(newSolution);
                if (1.0 / newLength > fitness[i]) {
                    solutions[i] = newSolution;
                    fitness[i] = 1.0 / newLength;
                    trial[i] = 0;
                } else {
                    trial[i] += 1;
                }
                if (newLength < bestLength) {
                    bestLength = newLength;
                    bestSolution = newSolution;
                }
            }


            for (int i = 0; i < numOnlookerBees; ++i) {
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
                if (newLength < bestLength) {
                    bestLength = newLength;
                    bestSolution = newSolution;
                }
            }


            for (int i = 0; i < numBees; ++i) {
                if (trial[i] > limit) {
                    solutions[i] = constructSolution();
                    fitness[i] = 1.0 / calculateRouteLength(solutions[i]);
                    trial[i] = 0;
                }
            }
        }

        return bestSolution;
    }

    double getBestLength() const {
        return bestLength;
    }

private:
    const QVector<QVector<double>>& adjacencyMatrix;
    int numBees;
    int numIterations;
    int limit;
    int numEmployeeBees;
    int numOnlookerBees;
    double bestLength;
    QVector<int> bestSolution;
    std::mt19937 rng;

    int chooseNextVertex(const QSet<int>& visited, int currentVertex) {
        int numVertices = adjacencyMatrix.size();
        std::vector<double> probabilities(numVertices, 0.0);
        double total = 0.0;

        for (int i = 0; i < numVertices; ++i) {
            if (!visited.contains(i) && adjacencyMatrix[currentVertex][i] < std::numeric_limits<double>::infinity()) {
                double eta = 1.0 / adjacencyMatrix[currentVertex][i];
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
            return unvisitedVertices[dist(rng)];
        }

        for (double& prob : probabilities) {
            prob /= total;
        }

        std::discrete_distribution<> dist(probabilities.begin(), probabilities.end());
        return dist(rng);
    }

    QVector<int> constructSolution() {
        QSet<int> visited;
        int currentVertex = 0;
        QVector<int> solution = {currentVertex};
        visited.insert(currentVertex);

        while (currentVertex != adjacencyMatrix.size() - 1) {
            int nextVertex = chooseNextVertex(visited, currentVertex);
            solution.push_back(nextVertex);
            visited.insert(nextVertex);
            currentVertex = nextVertex;
        }

        return solution;
    }

    double calculateRouteLength(const QVector<int>& solution) {
        double length = 0.0;
        for (int i = 0; i < solution.size() - 1; ++i) {
            length += adjacencyMatrix[solution[i]][solution[i + 1]];
        }
        return length;
    }

    int selectOnlooker(const QVector<double>& fitness) {
        std::vector<double> stdFitness(fitness.begin(), fitness.end());
        std::discrete_distribution<> dist(stdFitness.begin(), stdFitness.end());
        return dist(rng);
    }
};

#endif // ARTIFICIALBEECOLONY_H
