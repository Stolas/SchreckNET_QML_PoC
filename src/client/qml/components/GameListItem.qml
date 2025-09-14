import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    height: 85
    
    property var gameData
    signal joinClicked()
    signal spectateClicked()
    
    Rectangle {
        anchors.fill: parent
        border.color: "#e0e0e0"
        border.width: 1
        color: mouseArea.containsMouse ? "#f5f5f5" : "white"
        
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
        }
        
        RowLayout {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 15
            
            // Game Info Column
            ColumnLayout {
                Layout.fillWidth: true
                Layout.minimumWidth: 300
                spacing: 6
                
                // Game Name and Indicators
                RowLayout {
                    spacing: 10
                    Layout.fillWidth: true
                    
                    Text {
                        text: gameData.name
                        font.bold: true
                        font.pixelSize: 14
                        Layout.fillWidth: true
                        Layout.minimumWidth: 150
                        elide: Text.ElideRight
                    }
                    
                    // Password Protected Indicator
                    Rectangle {
                        visible: gameData.passwordProtected
                        width: 18
                        height: 18
                        color: "#ffeb3b"
                        border.color: "#f57f17"
                        border.width: 1
                        radius: 3
                        
                        Text {
                            anchors.centerIn: parent
                            text: "🔒"
                            font.pixelSize: 10
                        }
                        
                        ToolTip.text: "Password Protected"
                        ToolTip.visible: passwordMouseArea.containsMouse
                        
                        MouseArea {
                            id: passwordMouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                        }
                    }
                    
                    // Buddies Only Indicator
                    Rectangle {
                        visible: gameData.buddiesOnly
                        width: 18
                        height: 18
                        color: "#4caf50"
                        border.color: "#2e7d32"
                        border.width: 1
                        radius: 3
                        
                        Text {
                            anchors.centerIn: parent
                            text: "👥"
                            font.pixelSize: 9
                        }
                        
                        ToolTip.text: "Buddies Only"
                        ToolTip.visible: buddiesMouseArea.containsMouse
                        
                        MouseArea {
                            id: buddiesMouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                        }
                    }
                }
                
                // Game Details
                RowLayout {
                    spacing: 20
                    Layout.fillWidth: true
                    
                    Text {
                        text: "Host: " + gameData.host
                        font.pixelSize: 12
                        color: "#666"
                        Layout.minimumWidth: 80
                        elide: Text.ElideRight
                    }
                    
                    Text {
                        text: "Format: " + gameData.format
                        font.pixelSize: 12
                        color: "#666"
                        Layout.minimumWidth: 70
                    }
                    
                    Text {
                        text: "Players: " + gameData.currentPlayers + "/" + gameData.maxPlayers
                        font.pixelSize: 12
                        color: gameData.currentPlayers >= gameData.maxPlayers ? "#f44336" : "#4caf50"
                        font.bold: true
                        Layout.minimumWidth: 60
                    }
                    
                    Text {
                        text: "Spectators: " + gameData.spectators
                        font.pixelSize: 12
                        color: "#666"
                        visible: gameData.spectators > 0
                        Layout.minimumWidth: 70
                    }
                }
            }
            
            // Action Buttons
            ColumnLayout {
                spacing: 6
                Layout.minimumWidth: 90
                
                Button {
                    text: "Join"
                    enabled: gameData.currentPlayers < gameData.maxPlayers
                    Layout.preferredWidth: 85
                    Layout.minimumWidth: 75
                    Layout.minimumHeight: 28
                    Layout.alignment: Qt.AlignRight
                    font.pixelSize: 12
                    onClicked: root.joinClicked()
                }
                
                Button {
                    text: "Spectate"
                    Layout.preferredWidth: 85
                    Layout.minimumWidth: 75
                    Layout.minimumHeight: 28
                    Layout.alignment: Qt.AlignRight
                    font.pixelSize: 12
                    onClicked: root.spectateClicked()
                }
            }
        }
    }
}
