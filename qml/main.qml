import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

Window {
    property bool running: !project.running
    width: 900
    height: 609
    minimumWidth: width
    minimumHeight: height
    maximumWidth: width
    maximumHeight: height
    visible: project.uiVisible
    title: project.windowTitle
    FontLoader {
        id: poppinsLite
        source: "qrc:/Poppins/Poppins-Light.ttf"
    }
    FontLoader {
        id: poppinsBold
        source: "qrc:/Poppins/Poppins-Bold.ttf"
    }
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
            topMargin: 10
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
            leftMargin: 10
            bottom: parent.bottom
            bottomMargin: 70
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
        font.family: poppinsLite.name
        font.pointSize: 12
    }
    RowLayout {
        anchors {
            top: logoImage.bottom
            topMargin: 5
            left: parent.left
            right: parent.right
            margins: 30
        }
        Repeater {
            model: [qsTr("Pause"), qsTr("Resume"), qsTr("Settings"), qsTr("About"), qsTr("Quit")]
            Button {
                id: button
                enabled: {
                    switch (index) {
                    case 0:
                        return running
                    case 1:
                        return !running
                    default:
                        return true
                    }
                }
                Layout.fillWidth: true
                background: Rectangle {
                    color: "transparent"
                }
                contentItem: Text {
                    id: txt
                    text: button.text
                    font {
                        family: poppinsBold.name
                        pointSize: 12
                        bold: true
                    }
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    opacity: enabled ? 1 : 0.3
                }
                flat: true
                implicitWidth: 120
                text: modelData
                hoverEnabled: true
                state: pressed ? "Pressed": ""
                states: [
                    State {
                        name: "Pressed"
                        PropertyChanges {
                            target: txt
                            color: "#356c64"
                        }
                    }
                ]
                transitions: [
                    Transition {
                        from: ""; to: "Pressed"
                        ColorAnimation { duration: 100 }
                    },
                    Transition {
                        from: "Pressed"; to: ""
                        ColorAnimation { duration: 300 }
                    }
                ]
                onClicked: {
                    project.buttonClicked(index)
                }
            }
        }
    }
    Text {
        anchors {
            bottom: parent.bottom
            bottomMargin: 5
            horizontalCenter: parent.horizontalCenter
        }
        font.family: poppinsLite.name
        font.pointSize: 12
        text: qsTr("EnviroSoft PC Power Reclamation Software")
    }
}
