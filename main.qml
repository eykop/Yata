import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3

import com.eykop.Yata 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Yata: Yet Another Todo.txt Application!")

    ScrollView {
        anchors.fill: parent

        TasksTableView
        {
            id: listTabel
            Layout.margins: 5
            anchors.fill: parent
            model:  tasksListModel ? tasksListModel: null
            onRowClicked: {

            }
        }
    }

}

