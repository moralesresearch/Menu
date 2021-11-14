import QtQuick 2.2
import QtQuick.Controls 2.15
import QtQuick.Window 2.0
import QtGraphicalEffects 1.0
import QtMultimedia 5.15


ApplicationWindow {

    id: window
    visible: true
    visibility: "FullScreen"
    color: "black"
    //opacity: 0.1
    modality: Qt.WindowStaysOnTopHint

    // Would like to fade directly to grey but performance is not good enough; why?
    // NumberAnimation on opacity { to: 1; duration: 2000 }

    SequentialAnimation on color {
        ColorAnimation { to: "black"; duration: 500 }
        ColorAnimation { to: "grey"; duration: 1000 }
    }

    Component.onDestruction: {
        console.log("Exiting")
    }

}
