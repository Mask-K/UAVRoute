#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QElapsedTimer>
#include <QDebug>

#include <graph.h>
#include <antcolonyoptimization.h>
#include <artificialbeecolony.h>
#include <algorithmresults.h>
#include <iostream>




int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    Graph* gr = new Graph("graph.json");
    engine.rootContext()->setContextProperty("graph", gr);

    auto matrix = gr->distanceMatrix();

    QElapsedTimer timer;

    // Засікаємо час для мурашиного алгоритму
    timer.start();
    AntColonyOptimization aco(matrix);
    QVector<int> bestPath = aco.findBestPath();
    qint64 acoTime = timer.elapsed();

    qDebug() << "Мурашиний алгоритм";
    qDebug() << "Найкращий маршрут:" << bestPath;
    qDebug() << "Довжина найкращого маршруту:" << aco.calculateRouteLength(bestPath);
                                                  qDebug() << "У вигляді точок:";
    for(int i = 0; i < bestPath.length(); ++i){
        auto point = gr->getVertex(bestPath[i]);
        std::cout << "(" << point.x() << "; " << point.y() << "), ";
    }
    std::cout << std::endl;
    qDebug() << "Час виконання мурашиного алгоритму:" << acoTime << "мс";

    // Засікаємо час для бджолиного алгоритму
    timer.restart();
    ArtificialBeeColony abc(matrix, 20);
    auto bestPath2 = abc.findBestPath();
    double bestLength = abc.getBestLength();
    qint64 abcTime = timer.elapsed();

    qDebug() << "Бджолиний алгоритм";
    qDebug() << "Найкращий маршрут:" << bestPath2;
    qDebug() << "Довжина найкращого маршруту:" << bestLength;
    qDebug() << "У вигляді точок:";
    for(int i = 0; i < bestPath2.length(); ++i){
        auto point = gr->getVertex(bestPath2[i]);
        std::cout << "(" << point.x() << "; " << point.y() << "), ";
    }
    std::cout << std::endl;
    qDebug() << "Час виконання бджолиного алгоритму:" << abcTime << "мс";

    AlgorithmResults* algoResults = new AlgorithmResults();
    algoResults->setBestPath(bestPath2);
    algoResults->setBestLength(bestLength);
    engine.rootContext()->setContextProperty("algorithmResults", algoResults);

    const QUrl url(u"qrc:/UAVRoute/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
