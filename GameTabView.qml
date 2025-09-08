import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    
    signal backToLobby()
    
    property alias currentTabIndex: tabBar.currentIndex
    
    function openGame(gameName) {
        // Create new tab for the game
        var newTab = gameTabComponent.createObject(null, {"gameName": gameName})
        tabView.addTab(gameName, newTab)
        tabBar.currentIndex = tabView.count - 1
    }
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        
        // Tab Bar
        Rectangle {
            Layout.fillWidth: true
            Layout.minimumHeight: 50
            color: "#34495e"
            
            RowLayout {
                anchors.fill: parent
                spacing: 0
                
                // Back to Lobby Button
                Button {
                    text: "? Back to Lobby"
                    Layout.margins: 5
                    Layout.minimumWidth: 120
                    Layout.minimumHeight: 30
                    onClicked: root.backToLobby()
                }
                
                // Tab Bar
                TabBar {
                    id: tabBar
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    
                    background: Rectangle {
                        color: "transparent"
                    }
                    
                    Repeater {
                        id: tabRepeater
                        model: tabView.count
                        
                        TabButton {
                            id: tabButton
                            text: tabView.getTabName(index)
                            
                            background: Rectangle {
                                color: tabButton.checked ? "#2c3e50" : "#34495e"
                                border.color: "#7f8c8d"
                                border.width: tabButton.checked ? 0 : 1
                            }
                            
                            contentItem: RowLayout {
                                spacing: 8
                                
                                Text {
                                    text: tabButton.text
                                    color: "white"
                                    font.pixelSize: 12
                                    Layout.fillWidth: true
                                    elide: Text.ElideRight
                                }
                                
                                // Close button
                                Button {
                                    text: "?"
                                    Layout.preferredWidth: 20
                                    Layout.preferredHeight: 20
                                    background: Rectangle {
                                        color: parent.hovered ? "#e74c3c" : "transparent"
                                        radius: 10
                                    }
                                    contentItem: Text {
                                        text: parent.text
                                        color: "white"
                                        font.pixelSize: 10
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                    
                                    onClicked: {
                                        tabView.removeTab(index)
                                        if (tabView.count === 0) {
                                            root.backToLobby()
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                
                // New Game Button
                Button {
                    text: "+ Join Game"
                    Layout.margins: 5
                    Layout.minimumWidth: 100
                    Layout.minimumHeight: 30
                    onClicked: joinGameDialog.open()
                }
            }
        }
        
        // Tab Content
        StackLayout {
            id: tabView
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabBar.currentIndex
            
            property var tabNames: []
            property var tabItems: []
            
            function addTab(name, item) {
                tabNames.push(name)
                tabItems.push(item)
                item.parent = tabView
                tabNamesChanged()
            }
            
            function removeTab(index) {
                if (index >= 0 && index < tabItems.length) {
                    var item = tabItems[index]
                    tabNames.splice(index, 1)
                    tabItems.splice(index, 1)
                    item.destroy()
                    tabNamesChanged()
                    
                    // Adjust current index if necessary
                    if (currentIndex >= tabItems.length && tabItems.length > 0) {
                        currentIndex = tabItems.length - 1
                    }
                }
            }
            
            function getTabName(index) {
                return index < tabNames.length ? tabNames[index] : ""
            }
            
            Repeater {
                model: tabView.tabItems.length
                delegate: Loader {
                    sourceComponent: index < tabView.tabItems.length ? tabView.tabItems[index] : null
                }
            }
        }
    }
    
    // Component for game tabs
    Component {
        id: gameTabComponent
        
        GameView {
            property string gameName: ""
            
            Component.onCompleted: {
                if (gameName) {
                    gameController.gameName = gameName
                }
            }
            
            onBackToLobby: {
                // Find and remove this tab
                for (var i = 0; i < tabView.tabItems.length; i++) {
                    if (tabView.tabItems[i] === this) {
                        tabView.removeTab(i)
                        break
                    }
                }
                
                if (tabView.count === 0) {
                    root.backToLobby()
                }
            }
        }
    }
    
    // Join Game Dialog
    Dialog {
        id: joinGameDialog
        title: "Join Game"
        anchors.centerIn: parent
        modal: true
        width: 350
        height: 200
        
        contentItem: ColumnLayout {
            spacing: 15
            anchors.margins: 20
            
            Label {
                text: "Enter game name or ID:"
                font.pixelSize: 14
            }
            
            TextField {
                id: gameNameInput
                Layout.fillWidth: true
                placeholderText: "Game name..."
                selectByMouse: true
                
                Keys.onReturnPressed: {
                    if (text.trim().length > 0) {
                        joinGameDialog.accept()
                    }
                }
            }
            
            Text {
                text: "You can also join games from the lobby by clicking 'Join' on any available game."
                wrapMode: Text.WordWrap
                font.pixelSize: 12
                color: "#666"
                Layout.fillWidth: true
            }
        }
        
        standardButtons: Dialog.Ok | Dialog.Cancel
        
        onAccepted: {
            if (gameNameInput.text.trim().length > 0) {
                openGame(gameNameInput.text.trim())
                gameNameInput.text = ""
            }
        }
        
        onRejected: {
            gameNameInput.text = ""
        }
    }
}