/*
 * Copyright (c) 2020-2025, Stolas <schrecknet@codeinject.org>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "game_controller.h"
#include <QDebug>
#include <QStandardPaths>
#include <QSet>
#include <QMap>

// Card utility functions implementation
QString Card::cardTypeToString(Card::Type type) {
  QStringList type_strings;

  // Check each flag and add corresponding string
  if (static_cast<int>(type) & static_cast<int>(Type::Crypt))
    type_strings << "Crypt";
  if (static_cast<int>(type) & static_cast<int>(Type::Master))
    type_strings << "Master";
  if (static_cast<int>(type) & static_cast<int>(Type::Action))
    type_strings << "Action";
  if (static_cast<int>(type) & static_cast<int>(Type::ActionModifier))
    type_strings << "Modifier";
  if (static_cast<int>(type) & static_cast<int>(Type::PoliticalAction))
    type_strings << "Political Action";
  if (static_cast<int>(type) & static_cast<int>(Type::Equipment))
    type_strings << "Equipment";
  if (static_cast<int>(type) & static_cast<int>(Type::Retainer))
    type_strings << "Retainer";
  if (static_cast<int>(type) & static_cast<int>(Type::Ally))
    type_strings << "Ally";
  if (static_cast<int>(type) & static_cast<int>(Type::Combat))
    type_strings << "Combat";
  if (static_cast<int>(type) & static_cast<int>(Type::Reaction))
    type_strings << "Reaction";
  if (static_cast<int>(type) & static_cast<int>(Type::Event))
    type_strings << "Event";
  if (static_cast<int>(type) & static_cast<int>(Type::Power))
    type_strings << "Power";
  if (static_cast<int>(type) & static_cast<int>(Type::Conviction))
    type_strings << "Conviction";

  // Handle special cases
  if (type_strings.isEmpty())
    return "Unknown";

  // Join multiple types with "/"
  return type_strings.join("/");
}

Card::Type Card::stringToCardType(const QString &typeStr) {
  // Handle single types first
  if (typeStr == "Crypt")
    return static_cast<Type>(static_cast<int>(Type::Crypt));
  if (typeStr == "Master")
    return static_cast<Type>(static_cast<int>(Type::Master));
  if (typeStr == "Action")
    return static_cast<Type>(static_cast<int>(Type::Action));
  if (typeStr == "Modifier")
    return static_cast<Type>(static_cast<int>(Type::ActionModifier));
  if (typeStr == "Political Action")
    return static_cast<Type>(static_cast<int>(Type::PoliticalAction));
  if (typeStr == "Equipment")
    return static_cast<Type>(static_cast<int>(Type::Equipment));
  if (typeStr == "Retainer")
    return static_cast<Type>(static_cast<int>(Type::Retainer));
  if (typeStr == "Ally")
    return static_cast<Type>(static_cast<int>(Type::Ally));
  if (typeStr == "Combat")
    return static_cast<Type>(static_cast<int>(Type::Combat));
  if (typeStr == "Reaction")
    return static_cast<Type>(static_cast<int>(Type::Reaction));
  if (typeStr == "Event")
    return static_cast<Type>(static_cast<int>(Type::Event));
  if (typeStr == "Power")
    return static_cast<Type>(static_cast<int>(Type::Power));
  if (typeStr == "Conviction")
    return static_cast<Type>(static_cast<int>(Type::Conviction));

  // Handle compound types (e.g., "Action/Combat")
  if (typeStr.contains("/")) {
    QStringList parts = typeStr.split("/");
    int combinedType = 0;
    for (const QString &part : parts) {
      Card::Type partType = stringToCardType(part.trimmed());
      combinedType |= static_cast<int>(partType);
    }
    return static_cast<Type>(combinedType);
  }

  // Default to Token for unknown types
  return Type::Token;
}

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
    Q_UNUSED(deck_file)
    // In a real implementation, this would parse a deck file format
    beginResetModel();
    cards.clear();
    loadSampleDeck();
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
        Card("Cats' Guidance", Card::Type::Reaction, "https://static.krcg.org/card/catsguidance.jpg"),
        Card("Cats' Guidance", Card::Type::Reaction, "https://static.krcg.org/card/catsguidance.jpg"),
        Card("Cats' Guidance", Card::Type::Reaction, "https://static.krcg.org/card/catsguidance.jpg"),
        Card("Cats' Guidance", Card::Type::Reaction, "https://static.krcg.org/card/catsguidance.jpg"),

        Card("Ears of the Hare", Card::Type::Reaction, "https://static.krcg.org/card/earsofthehare.jpg"),
        Card("Ears of the Hare", Card::Type::Reaction, "https://static.krcg.org/card/earsofthehare.jpg"),
        Card("Ears of the Hare", Card::Type::Reaction, "https://static.krcg.org/card/earsofthehare.jpg"),
        Card("Ears of the Hare", Card::Type::Reaction, "https://static.krcg.org/card/earsofthehare.jpg"),
        Card("Ears of the Hare", Card::Type::Reaction, "https://static.krcg.org/card/earsofthehare.jpg"),
        Card("Ears of the Hare", Card::Type::Reaction, "https://static.krcg.org/card/earsofthehare.jpg"),

        Card("Falcon's Eye", Card::Type::Reaction, "https://static.krcg.org/card/falconseye.jpg"),
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

// GamePlayersModel implementation
GamePlayersModel::GamePlayersModel(QObject* parent)
    : QAbstractListModel(parent)
{
    loadSamplePlayers();
}

int GamePlayersModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return players.size();
}

QVariant GamePlayersModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= players.size())
        return QVariant();

    const GamePlayer& player = players[index.row()];

    switch (role) {
    case NameRole:
        return player.name;
    case StatusRole:
        return player.status;
    case LifeRole:
        return player.life;
    case HandSizeRole:
        return player.hand_size;
    case IsHostRole:
        return player.is_host;
    case AvatarRole:
        return player.avatar;
    }

    return QVariant();
}

QHash<int, QByteArray> GamePlayersModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[StatusRole] = "status";
    roles[LifeRole] = "life";
    roles[HandSizeRole] = "handSize";
    roles[IsHostRole] = "isHost";
    roles[AvatarRole] = "avatar";
    return roles;
}

void GamePlayersModel::updatePlayerStatus(const QString& player_name, const QString& status)
{
    for (int i = 0; i < players.size(); ++i) {
        if (players[i].name == player_name) {
            players[i].status = status;
            QModelIndex idx = index(i);
            emit dataChanged(idx, idx, {StatusRole});
            break;
        }
    }
}

void GamePlayersModel::updatePlayerLife(const QString& player_name, int life)
{
    for (int i = 0; i < players.size(); ++i) {
        if (players[i].name == player_name) {
            players[i].life = life;
            QModelIndex idx = index(i);
            emit dataChanged(idx, idx, {LifeRole});
            break;
        }
    }
}

void GamePlayersModel::loadSamplePlayers()
{
    players = {
        {"PlayerOne", "Ready", 20, 7, true, "https://placecats.com/128/128"},
        {"ProPlayer", "Selecting Deck", 18, 5, false, "https://placecats.com/64/128"},
        {"CurrentUser", "Selecting Deck", 20, 6, false, "https://placecats.com/64/64"}
    };
}

// GameController implementation
GameController::GameController(QObject* parent)
    : QObject(parent)
    , deck_model(new DeckModel(this))
    , players_model(new GamePlayersModel(this))
    , game_name("Casual Standard")
    , current_player("PlayerOne")
    , game_phase("Main Phase")
    , is_host(false)
{
    addSystemMessage("Game joined successfully!");
    addSystemMessage("Load your deck to begin playing.");
}

void GameController::setGameName(const QString& name)
{
    if (game_name != name) {
        game_name = name;
        emit gameNameChanged();
    }
}

void GameController::setCurrentPlayer(const QString& player)
{
    if (current_player != player) {
        current_player = player;
        emit currentPlayerChanged();
    }
}

void GameController::setGamePhase(const QString& phase)
{
    if (game_phase != phase) {
        game_phase = phase;
        emit gamePhaseChanged();
    }
}

void GameController::setChatMessage(const QString& message)
{
    if (chat_message != message) {
        chat_message = message;
        emit chatMessageChanged();
    }
}

void GameController::setIsHost(bool host)
{
    if (is_host != host) {
        is_host = host;
        emit isHostChanged();
    }
}

void GameController::loadDeckFromFile()
{
    // In a real implementation, this would open a file dialog
    addSystemMessage("Loading deck...");
    deck_model->loadDeck("");
    addSystemMessage("Deck loaded successfully! 60 cards total.");
    emit deckLoaded();
}

void GameController::sendChatMessage()
{
    if (!chat_message.trimmed().isEmpty()) {
        QString formatted_message = QString("[CurrentUser]: %1").arg(chat_message);
        chat_history.append(formatted_message);
        emit chatHistoryChanged();
        
        setChatMessage("");
    }
}

void GameController::leaveGame()
{
    addSystemMessage("Leaving game...");
    emit gameLeft();
}

void GameController::startGame()
{
    if (is_host) {
        addSystemMessage("Starting game...");
        setGamePhase("Upkeep");
        players_model->updatePlayerStatus("PlayerOne", "Playing");
        players_model->updatePlayerStatus("ProPlayer", "Playing");
        players_model->updatePlayerStatus("CurrentUser", "Playing");
    }
}

void GameController::endTurn()
{
    addSystemMessage("Turn ended.");
    // In a real implementation, this would handle turn logic
    if (current_player == "CurrentUser") {
        setCurrentPlayer("PlayerOne");
    } else if (current_player == "PlayerOne") {
        setCurrentPlayer("ProPlayer");
    } else {
        setCurrentPlayer("CurrentUser");
    }
}

void GameController::setReady()
{
    addSystemMessage("You are now ready to play.");
    players_model->updatePlayerStatus("CurrentUser", "Ready");
}

void GameController::concede()
{
    addSystemMessage("You have conceded the game.");
    players_model->updatePlayerStatus("CurrentUser", "Conceded");
}

void GameController::addSystemMessage(const QString& message)
{
    QString formatted_message = QString("* %1").arg(message);
    chat_history.append(formatted_message);
    emit chatHistoryChanged();
}
