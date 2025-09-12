import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    
    property var playerData
    
    height: 60
    color: playerData.name === "CurrentUser" ? "#e8f5e8" : "white"
    border.color: "#dee2e6"
    border.width: 1
    radius: 4
    
    RowLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 10
        
        // Avatar
        Rectangle {
            Layout.preferredWidth: 40
            Layout.preferredHeight: 40
            color: "#007bff"
            radius: 20
            
            Text {
                anchors.centerIn: parent
                text: playerData.name.charAt(0).toUpperCase()
                color: "white"
                font.bold: true
                font.pixelSize: 16
            }
        }
        
        // Player Info
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 2
            
            RowLayout {
                Layout.fillWidth: true
                
                Text {
                    text: playerData.name
                    font.bold: true
                    font.pixelSize: 12
                    Layout.fillWidth: true
                    elide: Text.ElideRight
                }
                
                // Host indicator
                Rectangle {
                    visible: playerData.isHost
                    width: 16
                    height: 16
                    color: "#ffc107"
                    radius: 8
                    
                    Text {
                        anchors.centerIn: parent
                        text: "H"
                        font.bold: true
                        font.pixelSize: 8
                        color: "white"
                    }
                    
                    ToolTip.text: "Host"
                    ToolTip.visible: hostMouseArea.containsMouse
                    
                    MouseArea {
                        id: hostMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                    }
                }
            }
            
            RowLayout {
                Layout.fillWidth: true
                spacing: 8
                
                // Status Indicator
                Rectangle {
                    width: 8
                    height: 8
                    radius: 4
                    color: getStatusColor(playerData.status)
                }
                
                Text {
                    text: playerData.status
                    font.pixelSize: 10
                    color: "#6c757d"
                }
                
                Item { Layout.fillWidth: true } // Spacer
                
                Text {
                    text: "?? " + playerData.life
                    font.pixelSize: 10
                    color: "#dc3545"
                    font.bold: true
                }
                
                Text {
                    text: "?? " + playerData.handSize
                    font.pixelSize: 10
                    color: "#495057"
                }
            }
        }
    }
    
    function getStatusColor(status) {
        switch(status) {
            case "Ready": return "#28a745"
            case "Playing": return "#007bff"
            case "Waiting": return "#ffc107"
            case "Disconnected": return "#dc3545"
            case "Conceded": return "#6c757d"
            default: return "#6c757d"
        }
    }
}