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
        function scaleRadius(r) {
            var scaleXFactor = width / (xMax - xMin);
            var scaleYFactor = height / (yMax - yMin);
            var scaleFactor = (scaleXFactor + scaleYFactor) / 2;
            return r * scaleFactor;
        }

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
            ctx.arc(scaleX(start.x), scaleY(start.y), 5, 0, 2 * Math.PI);
            ctx.stroke();


            ctx.strokeStyle = "purple";
            ctx.lineWidth = 5;
            ctx.beginPath();
            ctx.arc(scaleX(finish.x), scaleY(finish.y), 5, 0, 2 * Math.PI);
            ctx.stroke();

            // Draw unsafe zones
            ctx.strokeStyle = "red";
            ctx.lineWidth = 3;
            var unsafeZones = graph.getUnsafeZones();
            var radiuses = graph.getUnsafeZonesRadiuses();
            for (var i = 0; i < unsafeZones.length; i++) {
                var point = unsafeZones[i];
                var radius = radiuses[i];

                var scaledRadius = scaleRadius(radius);
                if (radius === 0) {
                    scaledRadius = 1;
                }
                console.log("Drawing circle at: ", point.x, point.y, "with radius:", scaledRadius);

                ctx.beginPath();
                ctx.arc(scaleX(point.x), scaleY(point.y), scaledRadius, 0, 2 * Math.PI);
                ctx.stroke();


                ctx.beginPath();
                ctx.arc(scaleX(point.x), scaleY(point.y), 2, 0, 2 * Math.PI);
                ctx.fillStyle = 'red';
                ctx.fill();
            }


            ctx.strokeStyle = "blue";
            ctx.lineWidth = 3;
            var intermediateVertices = graph.getIntermediateVertices();
            for (var i = 0; i < intermediateVertices.length; i++) {
                var point = intermediateVertices[i];
                ctx.beginPath();
                ctx.arc(scaleX(point.x), scaleY(point.y), 3, 0, 2 * Math.PI);
                ctx.stroke();
            }


            ctx.strokeStyle = "orange";
            ctx.lineWidth = 2;
            ctx.beginPath();
            var path = algorithmResults.bestPath;
            if (path.length > 0) {
                var first = graph.getVertex(path[0]);
                ctx.moveTo(scaleX(first.x), scaleY(first.y));
                for (var i = 1; i < path.length; ++i) {
                    var point = graph.getVertex(path[i]);
                    ctx.lineTo(scaleX(point.x), scaleY(point.y));
                }
                ctx.stroke();
            }


            ctx.fillStyle = "white";
            ctx.font = "14px Arial";
            ctx.fillText("Best Length: " + algorithmResults.bestLength, 10, 20);
        }
    }
    Component.onCompleted: {
        canvas.requestPaint();
    }
}
