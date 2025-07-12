import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    visible: true

    readonly property int margin: 20

    minimumWidth: mainwin.Layout.minimumWidth + 2 * margin
    minimumHeight: mainwin.Layout.minimumHeight + 2 * margin
    width: mainwin.Layout.preferredWidth + 2 * margin
    height: mainwin.Layout.preferredHeight + 2 * margin


    RowLayout {
        id: mainwin
        anchors.fill: parent
        anchors.margins: root.margin
        spacing: root.margin

        Layout.preferredWidth: 800
        Layout.preferredHeight: 600

        ColumnLayout {
            id: menue
            spacing: 30
            Layout.alignment: Qt.AlignTop
            Layout.preferredWidth: 100
            Layout.fillWidth: false
            Layout.fillHeight: true

            Button {
                text: qsTr("Load ROM")
                Layout.fillWidth: true
                onClicked: {
                    romstate.state = "LOADED"
                }
            }

            ColumnLayout {
                spacing: 10
                enabled: romstate.state !== "NONE"
                Button {
                    Layout.fillWidth: true
                    text: qsTr("Backgrounds")
                    onClicked: {
                        menuestate.state = "BACKGROUNDS"
                    }
                }
                // Insert Buttons here
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 50
            }
            Image {
                id: mainimg
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: implicitWidth
                Layout.minimumHeight: implicitHeight
                fillMode: Image.PreserveAspectFit

                visible: menuestate.state !== "NONE"
                source: "image://generated/background-20"
            }
        }
        
    }

    StateGroup {
        id: menuestate
        state: "NONE"

        states: [
            State {
                name: "NONE"
            },
            State {
                name: "BACKGROUNDS"
            }
        ]
    }

    StateGroup {
        id: romstate
        state: "NONE"

        states: [
            State {
                name: "NONE"
                PropertyChanges { root.title: qsTr("ReflexView: No ROM loaded") }
            },
            State {
                name: "LOADED"
                PropertyChanges { root.title: qsTr("ReflexView: Loaded ROM") }
            }
        ]
    }

}
