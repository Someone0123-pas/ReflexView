import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

ApplicationWindow {
    id: root
    visible: true
    title: qsTr("ReflexView: No ROM loaded")

    required property QmlBridge qmlbridge
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
                    rompicker.open()
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
                // New Menue-Buttons here
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
                PropertyChanges { mainimg.source: "image://generated/background-24" }
            }
        ]
    }

    StateGroup {
        id: romstate
        state: root.qmlbridge.is_filepath_loaded() ? "LOADED" : "NONE"

        states: [
            State {
                name: "NONE"
                PropertyChanges { root.title: qsTr("ReflexView: No ROM loaded") }
            },
            State {
                name: "LOADED"
                PropertyChanges { root.title: qsTr("ReflexView: ") + root.qmlbridge.get_rom_filepath() }
            }
        ]
    }


    FileDialog {
        id: rompicker
        title: qsTr("Select ROM")

        onAccepted: {
            root.qmlbridge.set_rom_filepath(selectedFile)
        }
    }

    Connections {
        target: root.qmlbridge
        function onError(errortitle, errormessage) {
            errorPopup.title = errortitle
            errorPopup.contentItem.text = errormessage
            errorPopup.open()
        }
        function onSuccess_set_rom_filepath() {
            romstate.state = "LOADED"
            menuestate.state = "NONE"
        }
    }

    Dialog {
        id: errorPopup
        anchors.centerIn: parent
        popupType: Popup.Window
        focus: true
        contentItem: Text {}
    }
}
