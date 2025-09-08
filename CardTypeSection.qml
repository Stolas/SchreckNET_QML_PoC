import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GroupBox {
    id: root
    
    property var cards: []
    
    title: getTotalCardCount() + " cards"
    
    visible: cards && cards.length > 0
    
    Layout.fillWidth: true
    Layout.minimumWidth: parent ? parent.width : 0
    Layout.preferredWidth: parent ? parent.width : 0
    Layout.minimumHeight: 80
    Layout.preferredHeight: 150
    
    ScrollView {
        anchors.fill: parent
        anchors.margins: 5
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
        
        Flow {
            id: cardFlow
            width: Math.max(parent.width, root.width - 20)
            layoutDirection: Qt.LeftToRight
            flow: Flow.LeftToRight
            spacing: 2
            
            Repeater {
                model: root.cards
                
                Repeater {
                    id: innerRepeater
                    model: modelData.quantity
                    property var cardData: modelData
                    
                    delegate: Item {
                        width: 44
                        height: 62
                        
                        property var card: innerRepeater.cardData
                        
                        Rectangle {
                            anchors.fill: parent
                            color: cardMouseArea.containsMouse ? "#e3f2fd" : "#f8f9fa"
                            border.color: getRarityColor(parent.card.rarity)
                            border.width: 2
                            radius: 4
                            
                            // Card Image
                            Image {
                                anchors.fill: parent
                                anchors.margins: 2
                                source: parent.card.imageUrl || getCardImageUrl(parent.card.name)
                                fillMode: Image.PreserveAspectFit
                                asynchronous: true
                                
                                onStatusChanged: {
                                    if (status === Image.Error) {
                                        console.log("Failed to load image for:", parent.card.name)
                                        console.log("URL was:", source)
                                    }
                                }
                                
                                // Fallback when image fails to load
                                Rectangle {
                                    anchors.fill: parent
                                    color: "#e9ecef"
                                    visible: parent.status === Image.Error
                                    
                                    Text {
                                        anchors.centerIn: parent
                                        text: "IMG"
                                        font.pixelSize: 8
                                        color: "#6c757d"
                                    }
                                }
                                
                                // Loading indicator
                                Rectangle {
                                    anchors.fill: parent
                                    color: "#f0f0f0"
                                    visible: parent.status === Image.Loading
                                    
                                    Text {
                                        anchors.centerIn: parent
                                        text: "..."
                                        font.pixelSize: 8
                                        color: "#999"
                                    }
                                }
                            }
                            
                            // Card name overlay (bottom)
                            Rectangle {
                                anchors.bottom: parent.bottom
                                anchors.left: parent.left
                                anchors.right: parent.right
                                height: 14
                                color: "#000000"
                                opacity: 0.7
                                visible: cardMouseArea.containsMouse
                                
                                Text {
                                    anchors.centerIn: parent
                                    text: parent.card.name
                                    font.pixelSize: 8
                                    color: "white"
                                    elide: Text.ElideRight
                                    maximumLineCount: 1
                                }
                            }
                            
                            MouseArea {
                                id: cardMouseArea
                                anchors.fill: parent
                                hoverEnabled: true
                                
                                onClicked: {
                                    console.log("Selected card:", parent.card.name)
                                }
                            }
                        }
                        
                        // Tooltip
                        ToolTip {
                            id: cardTooltip
                            text: card.name + 
                                  (card.manaCost ? "\nMana Cost: " + card.manaCost : "") + 
                                  "\nType: " + card.type + 
                                  "\nRarity: " + card.rarity +
                                  "\nQuantity: " + card.quantity
                            visible: cardMouseArea.containsMouse
                            delay: 500
                        }
                    }
                }
            }
        }
    }
    
    function getRarityColor(rarity) {
        switch(rarity) {
            case "Common": return "#95a5a6"
            case "Uncommon": return "#3498db"
            case "Rare": return "#f39c12"
            case "Mythic": return "#e74c3c"
            default: return "#95a5a6"
        }
    }
    
    function getCardImageUrl(cardName) {
        // Convert card name to URL-friendly format for KRCG
        var urlName = cardName.toLowerCase()
            .replace(/[^a-z0-9\s]/g, '') // Remove special characters
            .replace(/\s+/g, '-')        // Replace spaces with hyphens
            .replace(/-+/g, '-')         // Replace multiple hyphens with single
            .replace(/^-|-$/g, '');      // Remove leading/trailing hyphens
        
        return "https://static.krcg.org/card/" + urlName + ".jpg"
    }
    
    function getTotalCardCount() {
        var total = 0
        if (cards) {
            for (var i = 0; i < cards.length; i++) {
                total += cards[i].quantity || 1
            }
        }
        return total
    }
}
