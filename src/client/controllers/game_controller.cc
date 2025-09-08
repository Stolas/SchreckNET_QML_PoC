/*
 * Copyright (c) 2020-2025, Stolas <schrecknet@codeinject.org>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "game_controller.h"
#include <QDebug>
#include <QStandardPaths>

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

QVariant DeckModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= cards.size())
        return QVariant();

    const Card& card = cards[index.row()];

    switch (role) {
    case NameRole:
        return card.name;
    case TypeRole:
        return card.type;
    case ManaCostRole:
        return card.mana_cost;
    case ImageUrlRole:
        return card.image_url;
    case QuantityRole:
        return card.quantity;
    case RarityRole:
        return card.rarity;
    }

    return QVariant();
}

QHash<int, QByteArray> DeckModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[TypeRole] = "type";
    roles[ManaCostRole] = "manaCost";
    roles[ImageUrlRole] = "imageUrl";
    roles[QuantityRole] = "quantity";
    roles[RarityRole] = "rarity";
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
        if (!types.contains(card.type)) {
            types.append(card.type);
        }
    }
    return types;
}

QVariantList DeckModel::getCardsByType(const QString& type) const
{
    QVariantList card_list;
    for (const Card& card : cards) {
        if (card.type == type) {
            QVariantMap card_map;
            card_map["name"] = card.name;
            card_map["type"] = card.type;
            card_map["manaCost"] = card.mana_cost;
            card_map["imageUrl"] = card.image_url;
            card_map["quantity"] = card.quantity;
            card_map["rarity"] = card.rarity;
            card_list.append(card_map);
        }
    }
    return card_list;
}

void DeckModel::loadSampleDeck()
{
    // Sample VTEs
    cards = {
        // Crypt
        {"Howler", "Crypt", "", "https://static.krcg.org/card/howler.jpg", 4, ""},
        {"Siamese", "Crypt", "", "https://static.krcg.org/card/siamese.jpg", 3, ""},
        {"Cynthia", "Crypt", "", "https://static.krcg.org/card/cynthia.jpg", 3, ""},
        {"Nettie", "Crypt", "", "https://static.krcg.org/card/nettie.jpg", 1, ""},
        {"Juanita", "Crypt", "", "https://static.krcg.org/card/juanita.jpg", 1, ""},
        // Master
        {"Archon Investigation", "Master", "", "https://static.krcg.org/card/archon-investigation.jpg", 1, ""},
        {"Guardian Angel", "Master", "", "https://static.krcg.org/card/guardian-angel.jpg", 1, ""},
        {"Powerbase: Montreal", "Master", "", "https://static.krcg.org/card/powerbase-montreal.jpg", 1, ""},
        {"Rack, The", "Master", "", "https://static.krcg.org/card/rack-the.jpg", 1, ""},
        {"Smiling Jack, The Anarch", "Master", "", "https://static.krcg.org/card/smiling-jack-the-anarch.jpg", 2, ""},
        {"Vessel", "Master", "", "https://static.krcg.org/card/vessel.jpg", 4, ""},
        {"Villein", "Master", "", "https://static.krcg.org/card/villein.jpg", 4, ""},
        // Action
        {"Abbot", "Action", "", "https://static.krcg.org/card/abbot.jpg", 2, ""},
        {"Army of Rats", "Action", "", "https://static.krcg.org/card/army-of-rats.jpg", 1, ""},
        {"Charge of the Buffalo", "Action", "", "https://static.krcg.org/card/charge-of-the-buffalo.jpg", 2, ""},
        {"Enchant Kindred", "Action", "", "https://static.krcg.org/card/enchant-kindred.jpg", 7, ""},
        {"Engling Fury", "Action", "", "https://static.krcg.org/card/engling-fury.jpg", 4, ""},
        // Ally
        {"High Top", "Ally", "", "https://static.krcg.org/card/high-top.jpg", 1, ""},
        {"Ossian", "Ally", "", "https://static.krcg.org/card/ossian.jpg", 1, ""},
        // Action Modifier
        {"Aire of Elation", "Modifier", "", "https://static.krcg.org/card/aire-of-elation.jpg", 2, ""},
        {"Squirrel Balance", "Modifier", "", "https://static.krcg.org/card/squirrel-balance.jpg", 3, ""},
        // Action Modifier/Combat
        {"Swiftness of the Stag", "Mixed", "", "https://static.krcg.org/card/swiftness-of-the-stag.jpg", 8, ""},
        // Reaction
        {"Cats' Guidance", "Reaction", "", "https://static.krcg.org/card/cats-guidance.jpg", 4, ""},
        {"Ears of the Hare", "Reaction", "", "https://static.krcg.org/card/ears-of-the-hare.jpg", 6, ""},
        {"Falcon's Eye", "Reaction", "", "https://static.krcg.org/card/falcons-eye.jpg", 1, ""},
        {"On the Qui Vive", "Reaction", "", "https://static.krcg.org/card/on-the-qui-vive.jpg", 3, ""},
        {"Speak with Spirits", "Reaction", "", "https://static.krcg.org/card/speak-with-spirits.jpg", 8, ""},
        // Combat
        {"Canine Horde", "Combat", "", "https://static.krcg.org/card/canine-horde.jpg", 1, ""},
        {"Carrion Crows", "Combat", "", "https://static.krcg.org/card/carrion-crows.jpg", 3, ""},
        {"Drawing Out the Beast", "Combat", "", "https://static.krcg.org/card/drawing-out-the-beast.jpg", 2, ""},
        {"Target Vitals", "Combat", "", "https://static.krcg.org/card/target-vitals.jpg", 6, ""},
        {"Taste of Vitae", "Combat", "", "https://static.krcg.org/card/taste-of-vitae.jpg", 4, ""},
        {"Weighted Walking Stick", "Combat", "", "https://static.krcg.org/card/weighted-walking-stick.jpg", 6, ""},
        // Event
        {"Narrow Minds", "Event", "", "https://static.krcg.org/card/narrow-minds.jpg", 1, ""},
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
        {"PlayerOne", "Ready", 20, 7, true, "qrc:/images/avatar1.png"},
        {"ProPlayer", "Playing", 18, 5, false, "qrc:/images/avatar2.png"},
        {"CurrentUser", "Playing", 20, 6, false, "qrc:/images/avatar3.png"}
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