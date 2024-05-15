import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("UAV Route")

    Canvas {
        id: canvas
        anchors.fill: parent

        property real xMin: -10
        property real xMax: 10
        property real yMin: -10
        property real yMax: 10

        function scaleX(x) { return (x - xMin) / (xMax - xMin) * width; }
        function scaleY(y) { return height - (y - yMin) / (yMax - yMin) * height; }

        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);
            ctx.strokeStyle = "black";
            ctx.lineWidth = 2;

            ctx.beginPath();
            ctx.moveTo(scaleX(xMin), scaleY(0));
            ctx.lineTo(scaleX(xMax), scaleY(0));
            ctx.moveTo(scaleX(0), scaleY(yMin));
            ctx.lineTo(scaleX(0), scaleY(yMax));
            ctx.stroke();

            var start = graph.getStart();
            var finish = graph.getFinish();

            ctx.strokeStyle = "green";
            ctx.lineWidth = 5;
            ctx.beginPath();
            ctx.arc(scaleX(start.x), scaleY(start.y), 1, 0, 2 * Math.PI);
            ctx.stroke();

            ctx.strokeStyle = "purple";
            ctx.lineWidth = 5;
            ctx.beginPath();
            ctx.arc(scaleX(finish.x), scaleY(finish.y), 1, 0, 2 * Math.PI);
            ctx.stroke();

            ctx.strokeStyle = "red";
            ctx.lineWidth = 3;
            var unsafeZones = graph.getUnsafeZones();
            for (var i = 0; i < unsafeZones.length; i++) {
                var point = unsafeZones[i];
                ctx.beginPath();
                ctx.arc(scaleX(point.x), scaleY(point.y), 1, 0, 2 * Math.PI);
                ctx.stroke();
            }

            ctx.strokeStyle = "blue";
            ctx.lineWidth = 3;
            var intermediateVertices = graph.getIntermediateVertices();
            for (var i = 0; i < intermediateVertices.length; i++) {
                var point = intermediateVertices[i];
                ctx.beginPath();
                ctx.arc(scaleX(point.x), scaleY(point.y), 1, 0, 2 * Math.PI);
                ctx.stroke();
            }
        }
    }

    Component.onCompleted: {
        canvas.requestPaint();
    }
}
