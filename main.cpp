#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <graph.h>
#include <antcolonyoptimization.h>
#include <artificialbeecolony.h>
#include <algorithmresults.h>
#include <iostream>


QVector<QVector<double>> generateLargeAdjacencyMatrix(int numVertices, double density = 0.5, double maxWeight = 10.0) {

    QVector<QVector<double>> adjacencyMatrix(numVertices, QVector<double>(numVertices, std::numeric_limits<double>::infinity()));


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distWeight(1.0, maxWeight);
    std::uniform_real_distribution<> distProb(0.0, 1.0);


    for (int i = 0; i < numVertices; ++i) {
        adjacencyMatrix[i][i] = 0.0;

        for (int j = i + 1; j < numVertices; ++j) {
            if (distProb(gen) < density) {
                double weight = distWeight(gen);
                adjacencyMatrix[i][j] = weight;
                adjacencyMatrix[j][i] = weight;
            }
        }
    }

    return adjacencyMatrix;
}





int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);



    QQmlApplicationEngine engine;

    Graph* gr = new Graph("graph.json");
    engine.rootContext()->setContextProperty("graph", gr);



    auto matrix = gr->adjacencyMatrix();
    //auto matrix = generateLargeAdjacencyMatrix(100);
    for(int i = 0; i < matrix.size(); ++i){
        for(int j = 0; j < matrix[i].size(); ++j){
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    AntColonyOptimization aco(matrix);
    QVector<int> bestPath = aco.findBestPath();

    qDebug() << "Мурашиний алгоритм";
    qDebug() << "Найкращий маршрут:" << bestPath;
    qDebug() << "Довжина найкращого маршруту:" << aco.calculateRouteLength(bestPath);

    ArtificialBeeColony abc(matrix, 50);
    auto bestPath2 = abc.findBestPath();
    double bestLength = abc.getBestLength();
    qDebug() << "Бджолиний алгоритм";
    qDebug() << "Найкращий маршрут:" << bestPath2;
    qDebug() << "Довжина найкращого маршруту:" << bestLength;

    AlgorithmResults* algoResults = new AlgorithmResults();
    algoResults->setBestPath(bestPath);
    algoResults->setBestLength(aco.calculateRouteLength(bestPath));
    engine.rootContext()->setContextProperty("algorithmResults", algoResults);


    const QUrl url(u"qrc:/UAVRoute/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
