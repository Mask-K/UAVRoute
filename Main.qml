import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("UAV Route")

    Component.onCompleted: {
        var start = graph.getStart();
        var finish = graph.getFinish();
        console.log("Start: (" + start.x + ", " + start.y + ")");
        console.log("Finish: (" + finish.x + ", " + finish.y + ")");
        console.log("Total size: " + graph.size());

    }
}
