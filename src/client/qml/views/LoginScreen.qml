import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ScrollView {
    id: root
    
    property var controller
    
    contentWidth: availableWidth
    contentHeight: Math.max(mainColumn.implicitHeight + 40, 600)
    
    ColumnLayout {
        id: mainColumn
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15
        
        // Header
        Text {
            text: "SchreckNET - Connect to Server"
            font.pixelSize: 18
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
            Layout.minimumHeight: 25
        }
        
        // Server Selection Group
        GroupBox {
            title: "Server"
            Layout.fillWidth: true
            Layout.minimumWidth: 400
            Layout.minimumHeight: 200
            
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10
                
                // Previous Hosts Radio Button
                RadioButton {
                    id: previousHostRadio
                    text: "Previous Hosts"
                    checked: true
                    Layout.fillWidth: true
                    Layout.minimumHeight: 25
                }
                
                // Previous Hosts ComboBox with Refresh Button
                RowLayout {
                    enabled: previousHostRadio.checked
                    Layout.fillWidth: true
                    Layout.minimumHeight: 35
                    
                    ComboBox {
                        id: hostsCombo
                        model: controller.previousHosts
                        currentIndex: 0
                        Layout.fillWidth: true
                        Layout.minimumWidth: 200
                        Layout.minimumHeight: 30
                        
                        onCurrentTextChanged: {
                            if (currentText && currentText.length > 0) {
                                controller.selectedHost = currentText
                                controller.loadServerInfo(currentText)
                            }
                        }
                    }
                    
                    Button {
                        text: "??"
                        ToolTip.text: "Refresh server list"
                        Layout.minimumWidth: 40
                        Layout.minimumHeight: 30
                        onClicked: controller.refreshServers()
                    }
                }
                
                // Custom Host Radio Button
                RadioButton {
                    id: customHostRadio
                    text: "Custom Host"
                    Layout.fillWidth: true
                    Layout.minimumHeight: 25
                }
                
                // Custom Host Fields
                GridLayout {
                    columns: 2
                    enabled: customHostRadio.checked
                    Layout.fillWidth: true
                    Layout.minimumHeight: 120
                    columnSpacing: 10
                    rowSpacing: 8
                    
                    Label {
                        text: "Name:"
                        Layout.minimumWidth: 80
                    }
                    TextField {
                        id: saveNameField
                        placeholderText: "Unique Server Name"
                        Layout.fillWidth: true
                        Layout.minimumWidth: 200
                        Layout.minimumHeight: 25
                        text: controller.saveName
                        onTextChanged: controller.saveName = text
                    }
                    
                    Label {
                        text: "Host:"
                        Layout.minimumWidth: 80
                    }
                    TextField {
                        id: hostField
                        placeholderText: "Server URL"
                        Layout.fillWidth: true
                        Layout.minimumWidth: 200
                        Layout.minimumHeight: 25
                        text: controller.hostUrl
                        onTextChanged: controller.hostUrl = text
                    }
                    
                    Label {
                        text: "Port:"
                        Layout.minimumWidth: 80
                    }
                    TextField {
                        id: portField
                        placeholderText: "Communication Port"
                        validator: IntValidator { bottom: 0; top: 65535 }
                        Layout.fillWidth: true
                        Layout.minimumWidth: 200
                        Layout.minimumHeight: 25
                        text: controller.port
                        onTextChanged: controller.port = text
                    }
                }
                
                CheckBox {
                    id: autoConnectCheck
                    text: "Auto connect"
                    checked: controller.autoConnect
                    enabled: controller.savePassword
                    Layout.minimumHeight: 25
                    ToolTip.text: "Automatically connect to the most recent login when SchreckNET opens"
                    onCheckedChanged: controller.autoConnect = checked
                }
            }
        }
        
        // Server Contact Group
        GroupBox {
            title: "Server Contact"
            Layout.fillWidth: true
            Layout.minimumWidth: 400
            Layout.minimumHeight: 80
            visible: controller.serverContact.length > 0 || controller.serverIssues.length > 0
            
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10
                
                Text {
                    text: controller.serverIssues
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    Layout.minimumHeight: 20
                    visible: controller.serverIssues.length > 0
                }
                
                RowLayout {
                    visible: controller.serverContact.length > 0
                    Layout.minimumHeight: 25
                    
                    Label {
                        text: "Webpage:"
                        Layout.minimumWidth: 60
                    }
                    
                    Text {
                        text: '<a href="' + controller.serverContact + '">' + controller.serverContact + '</a>'
                        textFormat: Text.RichText
                        onLinkActivated: Qt.openUrlExternally(link)
                        color: "blue"
                        Layout.fillWidth: true
                    }
                }
            }
        }
        
        // Login Group
        GroupBox {
            title: "Login"
            Layout.fillWidth: true
            Layout.minimumWidth: 400
            Layout.minimumHeight: 120
            
            GridLayout {
                anchors.fill: parent
                anchors.margins: 10
                columns: 3
                columnSpacing: 10
                rowSpacing: 8
                
                Label {
                    text: "Player name:"
                    Layout.minimumWidth: 80
                }
                TextField {
                    id: playerNameField
                    Layout.columnSpan: 2
                    Layout.fillWidth: true
                    Layout.minimumWidth: 200
                    Layout.minimumHeight: 25
                    text: controller.playerName
                    onTextChanged: controller.playerName = text
                    selectByMouse: true
                    focus: true
                }
                
                Label {
                    text: "Password:"
                    Layout.minimumWidth: 80
                }
                TextField {
                    id: passwordField
                    echoMode: TextInput.Password
                    Layout.fillWidth: true
                    Layout.minimumWidth: 180
                    Layout.minimumHeight: 25
                    text: controller.password
                    onTextChanged: controller.password = text
                    selectByMouse: true
                    
                    Keys.onReturnPressed: connectButton.clicked()
                }
                
                Button {
                    text: "?"
                    ToolTip.text: "Reset Password"
                    Layout.minimumWidth: 30
                    Layout.minimumHeight: 25
                    onClicked: controller.forgotPassword()
                }
                
                Item { Layout.columnSpan: 1 } // Spacer
                
                CheckBox {
                    id: savePasswordCheck
                    text: "Save password"
                    Layout.columnSpan: 2
                    Layout.minimumHeight: 25
                    checked: controller.savePassword
                    onCheckedChanged: controller.savePassword = checked
                }
            }
        }
        
        // Buttons
        RowLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignRight
            Layout.minimumHeight: 40
            spacing: 10
            
            Button {
                text: "Cancel"
                Layout.minimumWidth: 80
                Layout.minimumHeight: 30
                onClicked: Qt.quit()
            }
            
            Button {
                id: connectButton
                text: "Connect"
                highlighted: true
                Layout.minimumWidth: 80
                Layout.minimumHeight: 30
                enabled: playerNameField.text.length > 0
                onClicked: controller.connectToServer()
            }
        }
        
        // Spacer
        Item {
            Layout.fillHeight: true
            Layout.minimumHeight: 20
        }
    }
}