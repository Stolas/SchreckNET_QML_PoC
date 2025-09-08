import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SchreckNET_QML_PoC

ApplicationWindow {
    id: root
    width: 800
    height: 700
    minimumWidth: 700
    minimumHeight: 600
    visible: true
    title: "SchreckNET - Connect to Server"
    
    property alias loginController: loginController
    
    LoginController {
        id: loginController
        
        onConnectionSucceeded: {
            root.title = "SchreckNET - Game Lobby"
            root.minimumWidth = 900
            root.minimumHeight = 650
            root.width = Math.max(root.width, 1000)
            root.height = Math.max(root.height, 700)
            stackView.push(gameLobbyComponent)
        }
        
        onConnectionFailed: function(error) {
            errorDialog.text = error
            errorDialog.open()
        }
    }
    
    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: loginScreenComponent
    }
    
    Component {
        id: loginScreenComponent
        LoginScreen {
            controller: loginController
        }
    }
    
    Component {
        id: gameLobbyComponent
        GameLobby {
            onBackToLogin: {
                loginController.disconnect()
                root.title = "SchreckNET - Connect to Server"
                root.minimumWidth = 700
                root.minimumHeight = 600
                root.width = Math.max(root.width, 800)
                root.height = Math.max(root.height, 700)
                stackView.pop()
            }
            
            onGameJoined: function(gameName) {
                // Open game in new tab view
                root.title = "SchreckNET - " + gameName
                root.minimumWidth = 1200
                root.minimumHeight = 800
                root.width = Math.max(root.width, 1400)
                root.height = Math.max(root.height, 900)
                stackView.push(gameTabViewComponent, {"initialGameName": gameName})
            }
        }
    }
    
    Component {
        id: gameTabViewComponent
        GameTabView {
            property string initialGameName: ""
            
            Component.onCompleted: {
                if (initialGameName) {
                    openGame(initialGameName)
                }
            }
            
            onBackToLobby: {
                root.title = "SchreckNET - Game Lobby"
                root.minimumWidth = 900
                root.minimumHeight = 650
                root.width = Math.max(root.width, 1000)
                root.height = Math.max(root.height, 700)
                stackView.pop()
            }
        }
    }
    
    Dialog {
        id: errorDialog
        title: "Connection Error"
        property alias text: errorText.text
        anchors.centerIn: parent
        
        contentItem: Text {
            id: errorText
            wrapMode: Text.WordWrap
            minimumPixelSize: 12
        }
        
        standardButtons: Dialog.Ok
    }
}
