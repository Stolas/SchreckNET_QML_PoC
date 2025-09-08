import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SchreckNET_QML_PoC

Item {
    id: root
    
    signal backToLogin()
    
    GameLobbyController {
        id: lobbyController
        
        onGameJoined: function(gameName) {
            // Handle game joining - could navigate to game screen
            console.log("Joined game:", gameName)
        }
        
        onGameCreated: {
            createGameDialog.open()
        }
        
        onSettingsRequested: {
            settingsDialog.open()
        }
    }
    
    RowLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 15
        
        // Left Panel - Game List
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width * 0.6
            Layout.minimumWidth: 500
            spacing: 10
            
            // Header with controls
            RowLayout {
                Layout.fillWidth: true
                Layout.minimumHeight: 40
                spacing: 10
                
                Label {
                    text: "Open Games"
                    font.pixelSize: 16
                    font.bold: true
                    Layout.minimumWidth: 100
                }
                
                Item { Layout.fillWidth: true } // Spacer
                
                Button {
                    text: "Refresh"
                    Layout.minimumWidth: 80
                    Layout.minimumHeight: 30
                    onClicked: lobbyController.refreshGames()
                }
                
                Button {
                    text: "Create Game"
                    highlighted: true
                    Layout.minimumWidth: 100
                    Layout.minimumHeight: 30
                    onClicked: lobbyController.createGame()
                }
            }
            
            // Game List
            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: 300
                
                ListView {
                    id: gameListView
                    model: lobbyController.gameModel
                    spacing: 2
                    
                    delegate: GameListItem {
                        width: gameListView.width
                        gameData: model
                        
                        onJoinClicked: lobbyController.joinGame(index)
                        onSpectateClicked: lobbyController.spectateGame(index)
                    }
                }
            }
        }
        
        // Right Panel - Chat and Controls
        ColumnLayout {
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width * 0.4
            Layout.minimumWidth: 350
            Layout.maximumWidth: 450
            spacing: 10
            
            // Player info and controls
            RowLayout {
                Layout.fillWidth: true
                Layout.minimumHeight: 35
                spacing: 10
                
                Label {
                    text: "Player: " + lobbyController.playerName
                    font.bold: true
                    Layout.fillWidth: true
                    Layout.minimumWidth: 100
                }
                
                Button {
                    text: "Settings"
                    Layout.minimumWidth: 70
                    Layout.minimumHeight: 25
                    onClicked: lobbyController.openSettings()
                }
                
                Button {
                    text: "Disconnect"
                    Layout.minimumWidth: 80
                    Layout.minimumHeight: 25
                    onClicked: root.backToLogin()
                }
            }
            
            // Chat Section
            GroupBox {
                title: "Chat"
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: 300
                
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 5
                    spacing: 5
                    
                    // Chat History
                    ScrollView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.minimumHeight: 200
                        
                        ListView {
                            id: chatListView
                            model: lobbyController.chatHistory
                            spacing: 1
                            
                            delegate: Text {
                                width: chatListView.width
                                text: modelData
                                wrapMode: Text.Wrap
                                padding: 4
                                font.pixelSize: 12
                                
                                Rectangle {
                                    anchors.fill: parent
                                    color: index % 2 === 0 ? "transparent" : "#f0f0f0"
                                    z: -1
                                }
                            }
                            
                            // Auto-scroll to bottom
                            onCountChanged: positionViewAtEnd()
                        }
                    }
                    
                    // Chat Input
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.minimumHeight: 35
                        spacing: 5
                        
                        TextField {
                            id: chatInput
                            Layout.fillWidth: true
                            Layout.minimumWidth: 200
                            Layout.minimumHeight: 25
                            placeholderText: "Type your message..."
                            text: lobbyController.chatMessage
                            onTextChanged: lobbyController.chatMessage = text
                            selectByMouse: true
                            
                            Keys.onReturnPressed: {
                                lobbyController.sendChatMessage()
                                chatInput.focus = true
                            }
                        }
                        
                        Button {
                            text: "Send"
                            enabled: chatInput.text.trim().length > 0
                            Layout.minimumWidth: 60
                            Layout.minimumHeight: 25
                            onClicked: {
                                lobbyController.sendChatMessage()
                                chatInput.focus = true
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Create Game Dialog
    Dialog {
        id: createGameDialog
        title: "Create New Game"
        anchors.centerIn: parent
        modal: true
        width: 400
        height: 350
        
        contentItem: ColumnLayout {
            spacing: 15
            anchors.margins: 20
            
            GridLayout {
                columns: 2
                columnSpacing: 10
                rowSpacing: 10
                Layout.fillWidth: true
                
                Label { 
                    text: "Game Name:" 
                    Layout.minimumWidth: 80
                }
                TextField {
                    id: gameNameField
                    placeholderText: "Enter game name"
                    Layout.fillWidth: true
                    Layout.minimumWidth: 200
                    Layout.minimumHeight: 25
                }
                
                Label { 
                    text: "Format:" 
                    Layout.minimumWidth: 80
                }
                ComboBox {
                    id: formatCombo
                    model: ["Standard", "Modern", "Legacy", "Pauper", "Commander"]
                    Layout.fillWidth: true
                    Layout.minimumWidth: 200
                    Layout.minimumHeight: 25
                }
                
                Label { 
                    text: "Max Players:" 
                    Layout.minimumWidth: 80
                }
                SpinBox {
                    id: maxPlayersSpinner
                    from: 2
                    to: 8
                    value: 4
                    Layout.minimumWidth: 100
                    Layout.minimumHeight: 25
                }
                
                Label { 
                    text: "Password:" 
                    Layout.minimumWidth: 80
                }
                TextField {
                    id: gamePasswordField
                    placeholderText: "Optional"
                    echoMode: TextInput.Password
                    Layout.fillWidth: true
                    Layout.minimumWidth: 200
                    Layout.minimumHeight: 25
                }
            }
            
            CheckBox {
                id: buddiesOnlyCheck
                text: "Buddies Only"
                Layout.minimumHeight: 25
            }
        }
        
        standardButtons: Dialog.Ok | Dialog.Cancel
        
        onAccepted: {
            lobbyController.gameModel.addGame(
                gameNameField.text || "New Game",
                lobbyController.playerName,
                formatCombo.currentText,
                1, // Current players (host)
                maxPlayersSpinner.value,
                0, // Spectators
                gamePasswordField.text.length > 0,
                buddiesOnlyCheck.checked
            )
            
            // Reset fields
            gameNameField.text = ""
            formatCombo.currentIndex = 0
            maxPlayersSpinner.value = 4
            gamePasswordField.text = ""
            buddiesOnlyCheck.checked = false
        }
    }
    
    // Settings Dialog
    Dialog {
        id: settingsDialog
        title: "Settings"
        anchors.centerIn: parent
        modal: true
        width: 500
        height: 400
        
        contentItem: ScrollView {
            anchors.margins: 20
            
            Text {
                text: "Settings panel would go here.\n\nThis is where you would configure:\n• Audio/Video settings\n• Game preferences\n• Network settings\n• Account settings\n• Display options\n• Keyboard shortcuts\n• Privacy settings"
                padding: 20
                wrapMode: Text.WordWrap
                font.pixelSize: 14
            }
        }
        
        standardButtons: Dialog.Close
    }
}