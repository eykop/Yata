import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.5
import QtQml.Models 2.13
import QtQuick.Controls.Styles 1.4

Item {

    property alias model: table.model
    signal rowClicked(int row);

    function fillColumns() {
        for(var i=0; i<table.model.columnCount(); i++)
        {
            var role = tasksListModel.headerData(i, Qt.Horizontal)
            table.addColumn(columnComponent.createObject(table, { "role": role, "title": role}));
        }

    }

    Component
    {
        id: columnComponent
        TableViewColumn{ }
    }

    TableView {
        id: table

        property int selectedRow: -1

        clip: true
        anchors.fill: parent
        Layout.margins: 10

        headerDelegate: Rectangle {
            id: delegateHeader
            height: headerText.implicitHeight
            color: styleData.column % 2 == 0 ? "gray": "lightgray"

            Label {
                id: headerText
                anchors.fill: parent
                anchors.verticalCenter: parent.verticalCenter
                text: styleData.value === undefined ? "" : styleData.value
            }

            Connections {
                target: styleData
                onPressedChanged: {console.debug("Header Clicked..")
                }
            }
        }



        itemDelegate: Component {
            Rectangle {
                height: 20
                implicitWidth: cellText.implicitWidth + cellText.padding * 4
                color: styleData.row === table.selectedRow ?
                           "yellow" :
                           table.selection.contains(styleData.row) ?
                               "red" :
                               ( styleData.row % 2 == 0 ) ?
                                   "lightblue" :
                                   "white"
                Text {
                    id: cellText
                    padding: 2
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width
                    text: styleData.value === undefined ? "" : styleData.value
                    elide: Text.ElideRight
                }

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton| Qt.RightButton
                    onClicked: {
                        if(mouse.button === Qt.LeftButton){
                            table.selectedRow = styleData.row;
                            if (mouse.modifiers & Qt.ShiftModifier) {
                                if(table.selection.contains(styleData.row)) {
                                    table.selection.deselect(styleData.row);
                                } else {
                                    table.selection.select(styleData.row);
                                }
                            }
                            else{
                                table.selection.clear();
                                table.selection.select(styleData.row);
                            }
                            rowClicked(styleData.row);
                        }
                        if(mouse.button === Qt.RightButton){
                            contextMenu.popup();
                        }
                    }
                }
            }
        }

        Menu { id: contextMenu
            MenuItem {
                text: qsTr('Delete')
                onTriggered: {
                    table.selection.forEach(
                                function(rowIndex) {
                                    table.selection.deselect(rowIndex);
                                }
                                )
                }
            }
            MenuItem {
                text: qsTr('Export')
                onTriggered: {
                    table.selection.forEach(
                                function(rowIndex) {
                                    table.selection.deselect(rowIndex);
                                }
                                )
                }
            }
        }

        onClicked: {
            rowClicked(row);
            selectedRow = row;
        }

        Component.onCompleted: {
            fillColumns();
        }

        Rectangle{
            border.color: "red"
            color: "transparent"
            anchors.fill: parent
        }
    }
}
