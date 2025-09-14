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
    // Layout.preferredHeight: parent.height
    
    ScrollView {
        anchors.fill: parent
        anchors.margins: 5
        Layout.fillWidth: true
        Layout.fillHeight: true
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
                            border.color: getTypeColor(parent.card.rarity)
                            border.width: 2
                            radius: 4
                            
                            // Card Image with better debugging
                            Image {
                                anchors.fill: parent
                                anchors.margins: 2
                                source: card.imageUrl || getCardImageUrl(card.name)
                                fillMode: Image.PreserveAspectFit
                                asynchronous: true
                                cache: false  // Disable caching to force reload
                                
                                Component.onCompleted: {
                                    console.log("Image component created for:", parent.card.name)
                                    console.log("Using source:", source)
                                }
                                
                                onStatusChanged: {
                                    console.log("=== IMAGE STATUS CHANGE ===")
                                    console.log("Card:", card.name)
                                    console.log("Status:", status, "(0=Null, 1=Ready, 2=Loading, 3=Error)")
                                    console.log("Source:", source)
                                    console.log("Progress:", progress)
                                    console.log("Width:", width, "Height:", height)
                                    console.log("PaintedWidth:", paintedWidth, "PaintedHeight:", paintedHeight)
                                    console.log("========================")
                                }
                                
                                onProgressChanged: {
                                    if (progress > 0) {
                                        console.log("Loading progress for", parent.card.name + ":", Math.round(progress * 100) + "%")
                                    }
                                }
                                
                                // Default fallback - always visible with card name
                                Rectangle {
                                    anchors.fill: parent
                                    color: parent.status === Image.Error ? "#ffebee" : 
                                           parent.status === Image.Loading ? "#fff3e0" : "#f0f0f0"
                                    visible: parent.status !== Image.Ready || parent.paintedWidth === 0
                                    
                                    Column {
                                        anchors.centerIn: parent
                                        spacing: 1
                                        
                                        Text {
                                            text: parent.parent.status === Image.Error ? "ERR" :
                                                  parent.parent.status === Image.Loading ? "..." : "?"
                                            font.pixelSize: 8
                                            color: parent.parent.status === Image.Error ? "#d32f2f" :
                                                   parent.parent.status === Image.Loading ? "#ff9800" : "#666"
                                            anchors.horizontalCenter: parent.horizontalCenter
                                        }
                                        
                                        Text {
                                            text: parent.parent.parent.card.name
                                            font.pixelSize: 5
                                            color: "#333"
                                            wrapMode: Text.WordWrap
                                            width: 38
                                            horizontalAlignment: Text.AlignHCenter
                                            maximumLineCount: 3
                                        }
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
                                    console.log("=== CARD CLICKED ===")
                                    // console.log("Card name:", parent.card.name)
                                    // console.log("Type:", parent.card.type)
                                    // console.log("imageUrl:", parent.card.imageUrl)
                                    // console.log("Quantity:", parent.card.quantity)
                                    console.log("Full card data:", JSON.stringify(parent.card))
                                    console.log("==================")
                                }
                            }
                        }
                        
                        // Tooltip with detailed info
                        ToolTip {
                            id: cardTooltip
                            text: "Card: " + card.name + 
                                  "\nType: " + card.type + 
                                  "\nQuantity: " + card.quantity +
                                  "\nImageURL: " + card.imageUrl +
                                  "\nGenerated: " + getCardImageUrl(card.name)
                                  //"\nFull card data:", JSON.stringify(card)
                            visible: cardMouseArea.containsMouse
                            delay: 300
                        }
                    }
                }
            }
        }
    }
    
    function getTypeColor(rarity) {
        switch(rarity) {
            case "Master": return "#95a5a6"
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
            .replace(/\s+/g, '')        // Replace spaces with hyphens
            .replace(/-+/g, '-')         // Replace multiple hyphens with single
            .replace(/^-|-$/g, '');      // Remove leading/trailing hyphens
        
        var url = "https://static.krcg.org/card/" + urlName + ".jpg"
        console.log("Generated URL for '" + cardName + "': " + url)
        return url
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
