import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import FluentUI 1.0

Button{
    id:control
    width: 36
    height: 36
    implicitWidth: width
    implicitHeight: height
    background:
        Rectangle{
        id:layout_color
        radius: 5
        color: container.colorValue
        border.color: {
            if(hovered)
                return FluTheme.primaryColor.light
            return FluTheme.dark ? Qt.rgba(45/255,45/255,45/255,1) : Qt.rgba(226/255,229/255,234/255,1)
        }
        border.width: 1
    }
    contentItem: Item{}
    onClicked: {
        popup.showPopup()
    }
    Menu{
        id:popup
        modal: true
        dim:false
        height: container.height
        width: container.width
        contentItem: Item{
            clip: true
            FluColorView{
                id:container
            }
        }
        background:Item{
            FluShadow{
                radius: 5
            }
        }
        enter: Transition {
            reversible: true
            NumberAnimation {
                property: "opacity"
                from:0
                to:1
                duration: FluTheme.enableAnimation ? 83 : 0
            }
        }

        exit:Transition {
            NumberAnimation {
                property: "opacity"
                from:1
                to:0
                duration: FluTheme.enableAnimation ? 83 : 0
            }
        }
        function showPopup() {
            var pos = control.mapToItem(null, 0, 0)
            if(window.height>pos.y+control.height+container.height){
                popup.y = control.height
            } else if(pos.y>container.height){
                popup.y = -container.height
            } else {
                popup.y = window.height-(pos.y+container.height)
            }
            popup.x = -(popup.width-control.width)/2
            popup.open()
        }
    }
}
