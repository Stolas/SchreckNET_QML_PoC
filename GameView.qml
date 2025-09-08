import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SchreckNET_QML_PoC

Item {
        id: root

        property string gameName: ""
        signal backToLobby()

        GameController {
                id: gameController
                gameName: root.gameName

                onGameLeft: {
                        root.backToLobby()
                }
        }

        ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 5

                // Top Bar - Game Info and Controls
                Rectangle {
                        Layout.fillWidth: true
                        Layout.minimumHeight: 50
                        color: "#2c3e50"
                        radius: 5

                        RowLayout {
                                anchors.fill: parent
                                anchors.margins: 10
                                spacing: 15

                                // Game Info
                                ColumnLayout {
                                        spacing: 2

                                        Text {
                                                text: gameController.gameName
                                                color: "white"
                                                font.bold: true
                                                font.pixelSize: 16
                                        }

                                        Text {
                                                text: "Current: " + gameController.currentPlayer + " | Phase: " + gameController.gamePhase
                                                color: "#bdc3c7"
                                                font.pixelSize: 12
                                        }
                                }

                                Item { Layout.fillWidth: true } // Spacer

                                // Game Controls
                                RowLayout {
                                        spacing: 10

                                        Button {
                                                text: "Load Deck"
                                                Layout.minimumWidth: 80
                                                Layout.minimumHeight: 30
                                                onClicked: gameController.loadDeckFromFile()
                                        }

                                        Button {
                                                text: "End Turn"
                                                enabled: gameController.currentPlayer === "CurrentUser"
                                                Layout.minimumWidth: 80
                                                Layout.minimumHeight: 30
                                                onClicked: gameController.endTurn()
                                        }

                                        Button {
                                                text: "Start Game"
                                                visible: gameController.isHost
                                                highlighted: true
                                                Layout.minimumWidth: 80
                                                Layout.minimumHeight: 30
                                                onClicked: gameController.startGame()
                                        }

                                        Button {
                                                text: "Leave Game"
                                                Layout.minimumWidth: 80
                                                Layout.minimumHeight: 30
                                                onClicked: gameController.leaveGame()
                                        }
                                }
                        }
                }

                // Main Game Area
                RowLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        spacing: 10

                        // Left Side - Card Display
                        ColumnLayout {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.preferredWidth: parent.width * 0.65
                                Layout.minimumWidth: 600
                                spacing: 5

                                // Deck Overview
                                Rectangle {
                                        Layout.fillWidth: true
                                        Layout.minimumHeight: 40
                                        color: "#ecf0f1"
                                        border.color: "#bdc3c7"
                                        border.width: 1
                                        radius: 3

                                        RowLayout {
                                                anchors.fill: parent
                                                anchors.margins: 10

                                                Text {
                                                        text: "Deck Overview"
                                                        font.bold: true
                                                        font.pixelSize: 14
                                                }

                                                Item { Layout.fillWidth: true }

                                                Text {
                                                        text: "Total Cards: " + gameController.deckModel.rowCount()
                                                        font.pixelSize: 12
                                                        color: "#7f8c8d"
                                                }
                                        }
                                }

                                // Card Display by Type
                                ScrollView {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        Layout.minimumHeight: 400

                                        ColumnLayout {
                                                width: parent.width
                                                spacing: 15

                                                // Crypt Section
                                                CardTypeSection {
                                                        Layout.fillWidth: true
                                                        title:  "Crypt"
                                                        cards: gameController.deckModel.getCardsByType("Crypt")
                                                }
                                                // Master Section
                                                CardTypeSection {
                                                        Layout.fillWidth: true
                                                        title:  "Master"
                                                        cards: gameController.deckModel.getCardsByType("Master")
                                                }
                                                // Action Section
                                                CardTypeSection {
                                                        Layout.fillWidth: true
                                                        title:  "Action"
                                                        cards: gameController.deckModel.getCardsByType("Action")
                                                }
                                                // Ally Section
                                                CardTypeSection {
                                                        Layout.fillWidth: true
                                                        title:  "Ally"
                                                        cards: gameController.deckModel.getCardsByType("Ally")
                                                }
                                                // Modifier Section
                                                CardTypeSection {
                                                        Layout.fillWidth: true
                                                        title:  "Modifier"
                                                        cards: gameController.deckModel.getCardsByType("Modifier")
                                                }
                                                // Mixed Section
                                                CardTypeSection {
                                                        Layout.fillWidth: true
                                                        title:  "Mixed"
                                                        cards: gameController.deckModel.getCardsByType("Mixed")
                                                }
                                                // Reaction Section
                                                CardTypeSection {
                                                        Layout.fillWidth: true
                                                        title:  "Reaction"
                                                        cards: gameController.deckModel.getCardsByType("Reaction")
                                                }
                                                // Combat Section
                                                CardTypeSection {
                                                        Layout.fillWidth: true
                                                        title:  "Combat"
                                                        cards: gameController.deckModel.getCardsByType("Combat")
                                                }
                                                // Event Section
                                                CardTypeSection {
                                                        Layout.fillWidth: true
                                                        title:  "Event"
                                                        cards: gameController.deckModel.getCardsByType("Event")
                                                }
                                        }
                                }
                        }

                        // Right Side - Players and Chat
                        ColumnLayout {
                                Layout.fillHeight: true
                                Layout.preferredWidth: parent.width * 0.35
                                Layout.minimumWidth: 300
                                Layout.maximumWidth: 400
                                spacing: 10

                                // Players List
                                GroupBox {
                                        title: "Players"
                                        Layout.fillWidth: true
                                        Layout.minimumHeight: 200
                                        Layout.maximumHeight: 250

                                        ScrollView {
                                                anchors.fill: parent
                                                anchors.margins: 5

                                                ListView {
                                                        id: playersListView
                                                        model: gameController.playersModel
                                                        spacing: 2

                                                        delegate: PlayerListItem {
                                                                width: playersListView.width
                                                                playerData: model
                                                        }
                                                }
                                        }
                                }

                                // Chat Section
                                GroupBox {
                                        title: "Game Chat"
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        Layout.minimumHeight: 250

                                        ColumnLayout {
                                                anchors.fill: parent
                                                anchors.margins: 5
                                                spacing: 5

                                                // Chat History
                                                ScrollView {
                                                        Layout.fillWidth: true
                                                        Layout.fillHeight: true
                                                        Layout.minimumHeight: 150

                                                        ListView {
                                                                id: chatListView
                                                                model: gameController.chatHistory
                                                                spacing: 1

                                                                delegate: Text {
                                                                        width: chatListView.width
                                                                        text: modelData
                                                                        wrapMode: Text.Wrap
                                                                        padding: 4
                                                                        font.pixelSize: 12

                                                                        Rectangle {
                                                                                anchors.fill: parent
                                                                                color: index % 2 === 0 ? "transparent" : "#f8f9fa"
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
                                                                Layout.minimumWidth: 150
                                                                Layout.minimumHeight: 25
                                                                placeholderText: "Type your message..."
                                                                text: gameController.chatMessage
                                                                onTextChanged: gameController.chatMessage = text
                                                                selectByMouse: true

                                                                Keys.onReturnPressed: {
                                                                        gameController.sendChatMessage()
                                                                        chatInput.focus = true
                                                                }
                                                        }

                                                        Button {
                                                                text: "Send"
                                                                enabled: chatInput.text.trim().length > 0
                                                                Layout.minimumWidth: 50
                                                                Layout.minimumHeight: 25
                                                                onClicked: {
                                                                        gameController.sendChatMessage()
                                                                        chatInput.focus = true
                                                                }
                                                        }
                                                }
                                        }
                                }
                        }
                }
        }
}
