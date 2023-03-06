﻿import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import QtGraphicalEffects 1.15
import FluentUI 1.0

Item {

    ColumnLayout{
        spacing: 5

        FluText{
            text:"Theme"
            fontStyle: FluText.TitleLarge
        }
        RowLayout{
            Layout.topMargin: 20


            Repeater{
                model: [FluColors._Yellow,FluColors._Orange,FluColors._Red,FluColors._Magenta,FluColors._Purple,FluColors._Blue,FluColors._Teal,FluColors._Green]
                delegate:  Rectangle{
                    width: 42
                    height: 42
                    radius: 4
                    color: mouse_item.containsMouse ? Qt.lighter(modelData.normal,1.1) : modelData.normal
                    FluIcon {
                        anchors.centerIn: parent
                        icon: FluentIcons.FA_check
                        iconSize: 15
                        visible: modelData === FluTheme.primaryColor
                        color: FluApp.isDark ? Qt.rgba(0,0,0,1) : Qt.rgba(1,1,1,1)
                    }
                    MouseArea{
                        id:mouse_item
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            FluTheme.primaryColor = modelData
                        }
                    }
                }
            }
        }
    }
}