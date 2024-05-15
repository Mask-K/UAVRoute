#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <graph.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Graph gr("graph.json");

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/UAVRoute/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
