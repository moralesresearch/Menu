import QtQuick 2.2
import QtQuick.Controls 2.15
import QtQuick.Window 2.0
import QtGraphicalEffects 1.0
import QtMultimedia 5.15
import QtQml 2.15

ApplicationWindow {

    id: window
    visible: true
    visibility: "FullScreen"
    color: Qt.rgba(0, 0, 0, 0)
    modality: Qt.WindowStaysOnTopHint

    // Would like to fade directly to grey but performance is not good enough; why?
    // NumberAnimation on opacity { to: 1; duration: 2000 }

    SequentialAnimation on color {
        PropertyAnimation { to: Qt.rgba(0.800, 0.800, 0.800, 1); duration: 2000 ; easing.type: Easing.InCubic }
    }

    Component.onDestruction: {
        console.log("Exiting")
    }

}
