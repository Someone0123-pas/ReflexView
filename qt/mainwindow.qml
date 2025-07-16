import QtQuick 2.0
import QtQuick.Controls.Material
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

ApplicationWindow {
    id: root
    visible: true
    title: qsTr("ReflexView: No ROM loaded")
    Material.theme: Material.Dark

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
        spacing: root.margin * 2

        Layout.preferredWidth: 800
        Layout.preferredHeight: 600

        Rectangle {
            Layout.alignment: Qt.AlignTop
            Layout.preferredWidth: 150
            Layout.fillWidth: false
            Layout.fillHeight: true
            radius: 10
            color: Material.background

            ColumnLayout {
                id: menue
                spacing: 30
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter

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
        }

        ColumnLayout {
            id: background_menue
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: menuestate.state === "BACKGROUNDS"
            spacing: 30

            readonly property int num_backgrounds: 0x1f
            property int pictureindex: 0

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                Layout.alignment: Qt.AlignCenter
                spacing: 50

                Button {
                    id: background_previous
                    icon.name: "go-previous"
                    onClicked: {
                        background_menue.pictureindex =
                        background_menue.pictureindex <= 0
                            ? background_menue.num_backgrounds - 1
                            : background_menue.pictureindex - 1
                    }
                }
                TextField {
                    id: background_indexfield
                    horizontalAlignment: TextField.AlignHCenter

                    text: qsTr(background_menue.pictureindex)
                    onTextChanged: {
                        if (text !== "") {
                            var nextindex = parseInt(text)
                            background_menue.pictureindex =
                                !isNaN(nextindex) && nextindex >= 0
                                ? nextindex % background_menue.num_backgrounds
                                : 0
                        }
                    }
                }
                Button {
                    id: background_next
                    icon.name: "go-next"
                    onClicked: {
                        background_menue.pictureindex = 
                        (background_menue.pictureindex + 1) % background_menue.num_backgrounds
                    }
                }
            }
            Image {
                id: mainimg
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: implicitWidth
                Layout.minimumHeight: implicitHeight
                fillMode: Image.PreserveAspectFit
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
                PropertyChanges { mainimg.source: "image://generated/background-%1".arg(background_menue.pictureindex) }
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
            romstate.state = "NONE"
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
