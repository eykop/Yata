import QtQuick 2.12
import QtQuick.Controls.Material 2.3
import QtQuick.Controls 2.0
import Qt.labs.platform 1.1


SystemTrayIcon {

    id: tray
    Material.theme: Material.Dark
    Material.accent: Material.Purple
    icon.source: "qrc:/todo.png"
    visible: true


    menu: Menu {
        MenuItem {
            text: qsTr("Quit")
            onTriggered: Qt.quit()
        }
        MenuItem {
            text: qsTr("Show")
            onTriggered: {
                var component = Qt.createComponent("AppWindow.qml");
                var win = component.createObject(tray, {x: tray.geometry.x, y: tray.geometry.y});
                win.show();
            }
        }
    }



    onActivated: {
        if (reason == SystemTrayIcon.Trigger) {
            var component = Qt.createComponent("ListViewWindow.qml");
            var win = component.createObject(tray, {trayX: tray.geometry.x, trayY: tray.geometry.y});
            win.show();
            win.raise();
            win.requestActivate();
        }


    }

    Component.onCompleted: showMessage("Message title", "Something important came up. Click this to know more.")
}
