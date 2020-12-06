import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.3

import com.eykop.Yata 1.0

Window {

    property real trayX
    property real trayY

    Material.theme: Material.Dark
    Material.accent: Material.Purple

    x: (trayX - width/2)
    y: trayY
    width: 300
    height: 500

    flags: Qt.Drawer

    Component {
        id: taskDelegate

        Item {
            width: 300; height: 40
            RowLayout {

                CheckBox { checked: Status == "x" }
                Text {
                    text: Description
                    elide: Qt.ElideRight
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: list.currentIndex = index
            }
        }
    }

    ListView {
        id: list

        anchors.fill: parent
        focus: true
        clip: true
        model: tasksListModel ? tasksListModel: null
        delegate: taskDelegate
        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
    }
}
