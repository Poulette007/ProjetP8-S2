import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 960
    height: 540
    title: "Ecran de demarrage"

    AnimatedImage {
        source: "qrc:/images/tumblr_o0mrvdzLsI1unbtf2o1_640.gif"
        smooth: true
        anchors.fill: parent
    }

    Text {
        x: 330
        y: 190
        font.pointSize: 40
        font.bold: true
        color: "#d6eaf8"
        text: "Commencer"      
    }

    Text {
        x: 430
        y: 250
        font.pointSize: 13
        font.italic: true
        font.bold: true
        color: "#FF0066"
        text: "Appuyer sur le bouton rouge!"
    }     
}