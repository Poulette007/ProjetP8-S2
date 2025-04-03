import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 960
    height: 540
    title: "Ecran de demarrage"
    Image {
        source: "qrc:/images/ciel.png"
        anchors.fill: parent
    }
    //Image Avion commercial
    Image {
        source: "qrc:/images/AvionCommercial.png"
        x:160
        y:153
    }
    //Image Chasseur brun
    Image {
        source: "qrc:/images/ChasseurBrun.png"
        x:390
        y:227
    }
    //Image Helico noir
    Image {
        source: "qrc:/images/HelicoNoir.png"
        x:590
        y:212
    }
    //Text Avion commercial
    Text {
        x:195
        y:300
        font.pointSize: 10
        font.bold: true
        font.italic: true
        color: "red"
        text: "Appuyer sur rouge "      
    }
    //Text Chasseur brun
    Text {
        x:425
        y:300
        font.pointSize: 10
        font.bold: true
        font.italic: true
        color: "green"
        text: "Appuyer sur vert "      
    }
    //Text Helico noir
    Text {
        x:645
        y:300
        font.pointSize: 10
        font.italic: true
        font.bold: true
        color: "black"
        text: "Appuyer sur noir"      
    }
    Text {
        x: 225
        y: 135
        font.pointSize: 20
        font.bold: true
        color: "black"
        text: "Entrez votre nom: "      
    }
    
    Rectangle{
        x: 225
        y: 170
        width: 500
        height: 30
        color: "#fdfefe"
        opacity: 0.5
        border.color: "black"
        border.width: 2
    }

         
}