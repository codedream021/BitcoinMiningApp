import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

Window {
    width: 900
    height: 609
    minimumWidth: width
    minimumHeight: height
    maximumWidth: width
    maximumHeight: height
    visible: project.uiVisible
    title: project.windowTitle
    Image {
        id: backgroundImage
        anchors {
            fill: parent
        }
        source: "qrc:/images/background.png"
    }
    Image {
        id: logoImage
        anchors {
            top: parent.top
            topMargin: 20
            horizontalCenter: parent.horizontalCenter
        }
        source: "qrc:/images/logo.png"
        width: backgroundImage.width * 0.38
        height: backgroundImage.height * 0.1
        fillMode: Image.PreserveAspectFit
    }
    Image {
        id: circleImage
        anchors {
            left: parent.left
            leftMargin: 20
            bottom: parent.bottom
            bottomMargin: 80
        }
        source: "qrc:/images/circle.png"
        width: backgroundImage.width * 0.44
        height: backgroundImage.height * 0.64
        fillMode: Image.PreserveAspectFit
    }
    Text {
        anchors.centerIn: circleImage
        clip: true
        text: project.statsText
        font.family: "Helvetica light"
        font.pointSize: 15
    }
    RowLayout {
        anchors {
            top: logoImage.bottom
            topMargin: 10
            left: parent.left
            right: parent.right
            margins: 30
        }
        Repeater {
            model: [qsTr("Pause"), qsTr("Resume"), qsTr("Settings"), qsTr("About"), qsTr("Quit")]
            Button {
                flat: true
                text: modelData
                font.family: "Helvetica Bold"
                font.pointSize: 15
                onClicked: {
                    project.buttonClicked(index)
                }
            }
        }
    }
    Text {
        anchors {
            bottom: parent.bottom
            bottomMargin: 20
            horizontalCenter: parent.horizontalCenter
        }
        font.family: "Helvetica light"
        font.pointSize: 15
        text: qsTr("EnviroSoft PC Power Reclamation Software")
    }
}
