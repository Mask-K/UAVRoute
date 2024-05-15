#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <graph.h>
#include <iostream>
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);



    QQmlApplicationEngine engine;

    Graph* gr = new Graph("graph.json");
    engine.rootContext()->setContextProperty("graph", gr);

    const QUrl url(u"qrc:/UAVRoute/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
