/*
 * Copyright (c) 2020-2025, Stolas <schrecknet@codeinject.org>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "deck_model.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QRegularExpression>
#include <QSet>
#include <QTextStream>

// DeckModel implementation
DeckModel::DeckModel(QObject* parent)
    : QAbstractListModel(parent)
{
    loadSampleDeck();
}

int DeckModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return cards.size();
}

int DeckModel::getCryptSize() const {
  int cnt = 0;
  for (auto card : cards) {
    if (card.getType() == Card::Type::Crypt) {
      cnt++;
    }
  }
  return cnt;
}

int DeckModel::getLibrarySize() const {
  int cnt = 0;
  for (auto card : cards) {
    if (card.getType() != Card::Type::Crypt) {
      cnt++;
    }
  }
  return cnt;
}

QVariant DeckModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= cards.size())
        return QVariant();

    const Card& card = cards[index.row()];

    switch (role) {
    case NameRole:
        return card.getName();
    case TypeRole:
        return card.typeString();
    case ImageUrlRole:
        return card.getImageUrl();
    }

    return QVariant();
}

QHash<int, QByteArray> DeckModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[TypeRole] = "type";
    roles[ImageUrlRole] = "imageUrl";
    return roles;
}

void DeckModel::loadDeck(const QString& deck_file)
{
    beginResetModel();
    cards.clear();
    
    if (deck_file.isEmpty()) {
        // If no file is provided, load sample deck
        loadSampleDeck();
    } else {
        // Try to parse the deck file
        if (!parseDeckFile(deck_file)) {
            // If parsing fails, load sample deck as fallback
            qDebug() << "Failed to parse deck file, loading sample deck instead";
            loadSampleDeck();
        }
    }
    
    endResetModel();
}

void DeckModel::clearDeck()
{
    beginResetModel();
    cards.clear();
    endResetModel();
}

QStringList DeckModel::getCardTypes() const
{
    QStringList types;
    for (const Card& card : cards) {
        QString typeStr = card.typeString();
        if (!types.contains(typeStr)) {
            types.append(typeStr);
        }
    }
    return types;
}

QVariantList DeckModel::getCryptCards() const
{
    QVariantList card_list;
    QMap<QString, int> card_counts;
    
    // Count occurrences of each card
    for (const Card& card : cards) {
        if (card.getType() == Card::Type::Crypt) {
            card_counts[card.getName()]++;
        }
    }
    
    // Create unique cards with quantities
    QSet<QString> processed_cards;
    for (const Card& card : cards) {
        if (card.getType() == Card::Type::Crypt && !processed_cards.contains(card.getName())) {
            QVariantMap card_map;
            card_map["name"] = card.getName();
            card_map["type"] = card.typeString();
            card_map["imageUrl"] = card.getImageUrl();
            card_map["quantity"] = card_counts[card.getName()];
            card_list.append(card_map);
            processed_cards.insert(card.getName());
        }
    }
    return card_list;
}

QVariantList DeckModel::getLibraryCards() const
{
    QVariantList card_list;
    QMap<QString, int> card_counts;
    
    // Count occurrences of each card
    for (const Card& card : cards) {
        if (card.getType() != Card::Type::Crypt) {
            card_counts[card.getName()]++;
        }
    }
    
    // Create unique cards with quantities
    QSet<QString> processed_cards;
    for (const Card& card : cards) {
        if (card.getType() != Card::Type::Crypt && !processed_cards.contains(card.getName())) {
            QVariantMap card_map;
            card_map["name"] = card.getName();
            card_map["type"] = card.typeString();
            card_map["imageUrl"] = card.getImageUrl();
            card_map["quantity"] = card_counts[card.getName()];
            card_list.append(card_map);
            processed_cards.insert(card.getName());
        }
    }
    return card_list;
}

void DeckModel::loadSampleDeck()
{
    // Sample VTEs cards - each Card needs: name, type, image_url, quantity
    cards = {
        // Crypt
        Card("Howler", Card::Type::Crypt, "https://static.krcg.org/card/howler.jpg"),
        Card("Howler", Card::Type::Crypt, "https://static.krcg.org/card/howler.jpg"),
        Card("Howler", Card::Type::Crypt, "https://static.krcg.org/card/howler.jpg"),
        Card("Howler", Card::Type::Crypt, "https://static.krcg.org/card/howler.jpg"),

        Card("Siamese", Card::Type::Crypt, "https://static.krcg.org/card/siamesethe.jpg"),
        Card("Siamese", Card::Type::Crypt, "https://static.krcg.org/card/siamesethe.jpg"),
        Card("Siamese", Card::Type::Crypt, "https://static.krcg.org/card/siamesethe.jpg"),

        Card("Cynthia", Card::Type::Crypt, "https://static.krcg.org/card/cynthiaingold.jpg"),
        Card("Cynthia", Card::Type::Crypt, "https://static.krcg.org/card/cynthiaingold.jpg"),
        Card("Cynthia", Card::Type::Crypt, "https://static.krcg.org/card/cynthiaingold.jpg"),

        Card("Nettie", Card::Type::Crypt, "https://static.krcg.org/card/nettiehale.jpg"),
        Card("Juanita", Card::Type::Crypt, "https://static.krcg.org/card/juanitasantiago.jpg"),
        
        // Master
        Card("Archon Investigation", Card::Type::Master, "https://static.krcg.org/card/archoninvestigation.jpg"),
        Card("Guardian Angel", Card::Type::Master, "https://static.krcg.org/card/guardianangel.jpg"),
        Card("Powerbase: Montreal", Card::Type::Master, "https://static.krcg.org/card/powerbasemontreal.jpg"),
        Card("Rack, The", Card::Type::Master, "https://static.krcg.org/card/rackthe.jpg"),
        Card("Smiling Jack, The Anarch", Card::Type::Master, "https://static.krcg.org/card/smilingjacktheanarch.jpg"),
        Card("Vessel", Card::Type::Master, "https://static.krcg.org/card/vessel.jpg"),
        Card("Vessel", Card::Type::Master, "https://static.krcg.org/card/vessel.jpg"),
        Card("Vessel", Card::Type::Master, "https://static.krcg.org/card/vessel.jpg"),
        Card("Vessel", Card::Type::Master, "https://static.krcg.org/card/vessel.jpg"),

        Card("Villein", Card::Type::Master, "https://static.krcg.org/card/villein.jpg"),
        Card("Villein", Card::Type::Master, "https://static.krcg.org/card/villein.jpg"),
        Card("Villein", Card::Type::Master, "https://static.krcg.org/card/villein.jpg"),
        Card("Villein", Card::Type::Master, "https://static.krcg.org/card/villein.jpg"),
        
        // Action
        Card("Abbot", Card::Type::Action, "https://static.krcg.org/card/abbot.jpg"),
        Card("Abbot", Card::Type::Action, "https://static.krcg.org/card/abbot.jpg"),
        Card("Army of Rats", Card::Type::Action, "https://static.krcg.org/card/armyofrats.jpg"),
        Card("Charge of the Buffalo", Card::Type::Action, "https://static.krcg.org/card/chargeofthebuffalo.jpg"),
        Card("Charge of the Buffalo", Card::Type::Action, "https://static.krcg.org/card/chargeofthebuffalo.jpg"),
        Card("Enchant Kindred", Card::Type::Action, "https://static.krcg.org/card/enchantkindred.jpg"),
        Card("Enchant Kindred", Card::Type::Action, "https://static.krcg.org/card/enchantkindred.jpg"),
        Card("Enchant Kindred", Card::Type::Action, "https://static.krcg.org/card/enchantkindred.jpg"),
        Card("Enchant Kindred", Card::Type::Action, "https://static.krcg.org/card/enchantkindred.jpg"),
        Card("Enchant Kindred", Card::Type::Action, "https://static.krcg.org/card/enchantkindred.jpg"),
        Card("Enchant Kindred", Card::Type::Action, "https://static.krcg.org/card/enchantkindred.jpg"),
        Card("Enchant Kindred", Card::Type::Action, "https://static.krcg.org/card/enchantkindred.jpg"),

        Card("Engling Fury", Card::Type::Action, "https://static.krcg.org/card/englingfury.jpg"),
        Card("Engling Fury", Card::Type::Action, "https://static.krcg.org/card/englingfury.jpg"),
        Card("Engling Fury", Card::Type::Action, "https://static.krcg.org/card/englingfury.jpg"),
        Card("Engling Fury", Card::Type::Action, "https://static.krcg.org/card/englingfury.jpg"),
        
        // Ally
        Card("High Top", Card::Type::Ally, "https://static.krcg.org/card/hightop.jpg"),
        Card("Ossian", Card::Type::Ally, "https://static.krcg.org/card/ossian.jpg"),
        
        // Action Modifier
        Card("Aire of Elation", Card::Type::ActionModifier, "https://static.krcg.org/card/aireofelation.jpg"),
        Card("Aire of Elation", Card::Type::ActionModifier, "https://static.krcg.org/card/aireofelation.jpg"),

        Card("Squirrel Balance", Card::Type::ActionModifier, "https://static.krcg.org/card/squirrelbalance.jpg"),
        Card("Squirrel Balance", Card::Type::ActionModifier, "https://static.krcg.org/card/squirrelbalance.jpg"),
        Card("Squirrel Balance", Card::Type::ActionModifier, "https://static.krcg.org/card/squirrelbalance.jpg"),
        
        // Action Modifier/Combat
        Card("Swiftness of the Stag", static_cast<Card::Type>( static_cast<int>(Card::Type::ActionModifier) | static_cast<int>(Card::Type::Combat)), "https://static.krcg.org/card/swiftnessofthestag.jpg"),
        Card("Swiftness of the Stag", static_cast<Card::Type>( static_cast<int>(Card::Type::ActionModifier) | static_cast<int>(Card::Type::Combat)), "https://static.krcg.org/card/swiftnessofthestag.jpg"),
        Card("Swiftness of the Stag", static_cast<Card::Type>( static_cast<int>(Card::Type::ActionModifier) | static_cast<int>(Card::Type::Combat)), "https://static.krcg.org/card/swiftnessofthestag.jpg"),
        Card("Swiftness of the Stag", static_cast<Card::Type>( static_cast<int>(Card::Type::ActionModifier) | static_cast<int>(Card::Type::Combat)), "https://static.krcg.org/card/swiftnessofthestag.jpg"),
        Card("Swiftness of the Stag", static_cast<Card::Type>( static_cast<int>(Card::Type::ActionModifier) | static_cast<int>(Card::Type::Combat)), "https://static.krcg.org/card/swiftnessofthestag.jpg"),
        Card("Swiftness of the Stag", static_cast<Card::Type>( static_cast<int>(Card::Type::ActionModifier) | static_cast<int>(Card::Type::Combat)), "https://static.krcg.org/card/swiftnessofthestag.jpg"),
        Card("Swiftness of the Stag", static_cast<Card::Type>( static_cast<int>(Card::Type::ActionModifier) | static_cast<int>(Card::Type::Combat)), "https://static.krcg.org/card/swiftnessofthestag.jpg"),
        Card("Swiftness of the Stag", static_cast<Card::Type>( static_cast<int>(Card::Type::ActionModifier) | static_cast<int>(Card::Type::Combat)), "https://static.krcg.org/card/swiftnessofthestag.jpg"),
        
        // Reaction
        Card("Cats\x27 Guidance", Card::Type::Reaction, "https://static.krcg.org/card/catsguidance.jpg"),
        Card("Cats\x27 Guidance", Card::Type::Reaction, "https://static.krcg.org/card/catsguidance.jpg"),
        Card("Cats\x27 Guidance", Card::Type::Reaction, "https://static.krcg.org/card/catsguidance.jpg"),
        Card("Cats\x27 Guidance", Card::Type::Reaction, "https://static.krcg.org/card/catsguidance.jpg"),

        Card("Ears of the Hare", Card::Type::Reaction, "https://static.krcg.org/card/earsofthehare.jpg"),
        Card("Ears of the Hare", Card::Type::Reaction, "https://static.krcg.org/card/earsofthehare.jpg"),
        Card("Ears of the Hare", Card::Type::Reaction, "https://static.krcg.org/card/earsofthehare.jpg"),
        Card("Ears of the Hare", Card::Type::Reaction, "https://static.krcg.org/card/earsofthehare.jpg"),
        Card("Ears of the Hare", Card::Type::Reaction, "https://static.krcg.org/card/earsofthehare.jpg"),
        Card("Ears of the Hare", Card::Type::Reaction, "https://static.krcg.org/card/earsofthehare.jpg"),

        Card("Falcon\x27s Eye", Card::Type::Reaction, "https://static.krcg.org/card/falconseye.jpg"),
        Card("On the Qui Vive", Card::Type::Reaction, "https://static.krcg.org/card/onthequivive.jpg"),
        Card("On the Qui Vive", Card::Type::Reaction, "https://static.krcg.org/card/onthequivive.jpg"),
        Card("On the Qui Vive", Card::Type::Reaction, "https://static.krcg.org/card/onthequivive.jpg"),
        Card("Speak with Spirits", Card::Type::Reaction, "https://static.krcg.org/card/speakwithspirits.jpg"),
        Card("Speak with Spirits", Card::Type::Reaction, "https://static.krcg.org/card/speakwithspirits.jpg"),
        Card("Speak with Spirits", Card::Type::Reaction, "https://static.krcg.org/card/speakwithspirits.jpg"),
        Card("Speak with Spirits", Card::Type::Reaction, "https://static.krcg.org/card/speakwithspirits.jpg"),
        Card("Speak with Spirits", Card::Type::Reaction, "https://static.krcg.org/card/speakwithspirits.jpg"),
        Card("Speak with Spirits", Card::Type::Reaction, "https://static.krcg.org/card/speakwithspirits.jpg"),
        Card("Speak with Spirits", Card::Type::Reaction, "https://static.krcg.org/card/speakwithspirits.jpg"),
        Card("Speak with Spirits", Card::Type::Reaction, "https://static.krcg.org/card/speakwithspirits.jpg"),
        
        // Combat
        Card("Canine Horde", Card::Type::Combat, "https://static.krcg.org/card/caninehorde.jpg"),
        Card("Carrion Crows", Card::Type::Combat, "https://static.krcg.org/card/carrioncrows.jpg"),
        Card("Carrion Crows", Card::Type::Combat, "https://static.krcg.org/card/carrioncrows.jpg"),
        Card("Carrion Crows", Card::Type::Combat, "https://static.krcg.org/card/carrioncrows.jpg"),
        Card("Drawing Out the Beast", Card::Type::Combat, "https://static.krcg.org/card/drawingoutthebeast.jpg"),
        Card("Drawing Out the Beast", Card::Type::Combat, "https://static.krcg.org/card/drawingoutthebeast.jpg"),
        Card("Target Vitals", Card::Type::Combat, "https://static.krcg.org/card/targetvitals.jpg"),
        Card("Target Vitals", Card::Type::Combat, "https://static.krcg.org/card/targetvitals.jpg"),
        Card("Target Vitals", Card::Type::Combat, "https://static.krcg.org/card/targetvitals.jpg"),
        Card("Target Vitals", Card::Type::Combat, "https://static.krcg.org/card/targetvitals.jpg"),
        Card("Target Vitals", Card::Type::Combat, "https://static.krcg.org/card/targetvitals.jpg"),
        Card("Target Vitals", Card::Type::Combat, "https://static.krcg.org/card/targetvitals.jpg"),
        Card("Taste of Vitae", Card::Type::Combat, "https://static.krcg.org/card/tasteofvitae.jpg"),
        Card("Taste of Vitae", Card::Type::Combat, "https://static.krcg.org/card/tasteofvitae.jpg"),
        Card("Taste of Vitae", Card::Type::Combat, "https://static.krcg.org/card/tasteofvitae.jpg"),
        Card("Taste of Vitae", Card::Type::Combat, "https://static.krcg.org/card/tasteofvitae.jpg"),
        Card("Weighted Walking Stick", Card::Type::Combat, "https://static.krcg.org/card/weightedwalkingstick.jpg"),
        Card("Weighted Walking Stick", Card::Type::Combat, "https://static.krcg.org/card/weightedwalkingstick.jpg"),
        Card("Weighted Walking Stick", Card::Type::Combat, "https://static.krcg.org/card/weightedwalkingstick.jpg"),
        Card("Weighted Walking Stick", Card::Type::Combat, "https://static.krcg.org/card/weightedwalkingstick.jpg"),
        Card("Weighted Walking Stick", Card::Type::Combat, "https://static.krcg.org/card/weightedwalkingstick.jpg"),
        Card("Weighted Walking Stick", Card::Type::Combat, "https://static.krcg.org/card/weightedwalkingstick.jpg"),
        
        // Event
        Card("Narrow Minds", Card::Type::Event, "https://static.krcg.org/card/narrowminds.jpg"),
    };
}

bool DeckModel::parseDeckFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file:" << filePath;
        return false;
    }
    
    QJsonDocument doc;
    QTextStream in(&file);
    while (!in.atEnd()) {
        doc = QJsonDocument::fromJson(in.readAll().toUtf8());
    }
    if (doc.isNull() || !doc.isObject() || doc.isEmpty()) { return false; }

    QJsonObject obj = doc.object();

    // Extract top-level string values.
    QString name = obj.value("name").toString();
    QString author = obj.value("author").toString();
    QString description = obj.value("description").toString();

    qDebug() << "Deck Name:" << name;
    qDebug() << "Author:" << author;
    qDebug() << "Description:" << description;

    // Extract the "crypt" array.

    QStringList section_lists;
    section_lists << "crypt" << "library";
    Q_FOREACH(const QString &section, section_lists) {
        QJsonArray card_array = obj.value(section).toArray();
        for (const QJsonValue &value : card_array) {
            if (value.isObject()) {
                QJsonObject card_object = value.toObject();
                int count = card_object.value("count").toInt();
                int id = card_object.value("id").toInt();
                // Todo; lookup card id
                // Add for 'count'-times card to `cards`
            }
        }
    }
    return !cards.isEmpty(); // Return true if we loaded at least one card
}

Card::Type DeckModel::parseCardType(const QString& typeString)
{
    QString lowerType = typeString.toLower().trimmed();
    
    if (lowerType.contains("crypt") || lowerType.contains("vampire")) {
        return Card::Type::Crypt;
    } else if (lowerType.contains("master")) {
        return Card::Type::Master;
    } else if (lowerType.contains("modifier")) {
        return Card::Type::ActionModifier;
    } else if (lowerType.contains("action")) {
        return Card::Type::Action;
    } else if (lowerType.contains("ally")) {
        return Card::Type::Ally;
    } else if (lowerType.contains("reaction")) {
        return Card::Type::Reaction;
    } else if (lowerType.contains("combat")) {
        return Card::Type::Combat;
    } else if (lowerType.contains("retainer")) {
        return Card::Type::Retainer;
    } else if (lowerType.contains("equipment")) {
        return Card::Type::Equipment;
    } else if (lowerType.contains("event")) {
        return Card::Type::Event;
    } else if (lowerType.contains("political")) {
        return Card::Type::PoliticalAction;
    } else if (lowerType.contains("power")) {
        return Card::Type::Power;
    } else if (lowerType.contains("conviction")) {
        return Card::Type::Conviction;
    }
    
    // Default to Action if we can't determine the type
    return Card::Type::Action;
}

QString DeckModel::generateImageUrl(const QString& cardName)
{
    // Generate a URL based on the card name
    // This is a simple implementation - in a real app you might have a card database
    QString normalizedName = cardName.toLower()
                                    .replace(" ", "")
                                    .replace(",", "")
                                    .replace("'", "")
                                    .replace(".", "")
                                    .replace("-", "");
    
    return QString("https://static.krcg.org/card/%1.jpg").arg(normalizedName);
}
