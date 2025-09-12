/*
 * Copyright (c) 2020-2025, Stolas <schrecknet@codeinject.org>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "game_lobby_controller.h"
#include <QDebug>

// GameListModel implementation
GameListModel::GameListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    loadSampleGames();
}

int GameListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return games.size();
}

QVariant GameListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= games.size())
        return QVariant();

    const GameInfo& game = games[index.row()];

    switch (role) {
    case NameRole:
        return game.name;
    case HostRole:
        return game.host;
    case FormatRole:
        return game.format;
    case CurrentPlayersRole:
        return game.current_players;
    case MaxPlayersRole:
        return game.max_players;
    case SpectatorsRole:
        return game.spectators;
    case PasswordProtectedRole:
        return game.password_protected;
    case BuddiesOnlyRole:
        return game.buddies_only;
    }

    return QVariant();
}

QHash<int, QByteArray> GameListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[HostRole] = "host";
    roles[FormatRole] = "format";
    roles[CurrentPlayersRole] = "currentPlayers";
    roles[MaxPlayersRole] = "maxPlayers";
    roles[SpectatorsRole] = "spectators";
    roles[PasswordProtectedRole] = "passwordProtected";
    roles[BuddiesOnlyRole] = "buddiesOnly";
    return roles;
}

void GameListModel::addGame(const QString& name, const QString& host, const QString& format,
                          int current_players, int max_players, int spectators,
                          bool password_protected, bool buddies_only)
{
    beginInsertRows(QModelIndex(), games.size(), games.size());
    games.append({name, host, format, current_players, max_players, spectators, password_protected, buddies_only});
    endInsertRows();
}

void GameListModel::removeGame(int index)
{
    if (index >= 0 && index < games.size()) {
        beginRemoveRows(QModelIndex(), index, index);
        games.removeAt(index);
        endRemoveRows();
    }
}

void GameListModel::refresh()
{
    beginResetModel();
    games.clear();
    loadSampleGames();
    endResetModel();
}

void GameListModel::loadSampleGames()
{
    // Sample game data
    games = {
        /* Name, Host Player, Format, Player Count, Seats Available, Spectator Count, Password, Buddies */
        {"Casual Standard", "PlayerOne", "Rated Regular", 2, 5, 1, false, false},
        {"Competitive Modern", "ProPlayer", "Casual Dual", 1, 2, 0, false, false},
        {"Friends Only", "BuddyHost", "Legacy", 3, 5, 0, false, true},
        {"Tournament Practice", "TourneyPrep", "Standard", 4, 5, 2, true, false},
        {"Beginner Friendly", "NewbieHelper", "Pauper", 1, 5, 0, true, true}
    };
}

// GameLobbyController implementation
GameLobbyController::GameLobbyController(QObject* parent)
    : QObject(parent)
    , game_model(new GameListModel(this))
    , player_name("Player")
{
    addSystemMessage("Welcome to SchreckNET! Connected to server.");
    addSystemMessage("Type your message and press Enter to chat.");
}

void GameLobbyController::setChatMessage(const QString& message)
{
    if (chat_message != message) {
        chat_message = message;
        emit chatMessageChanged();
    }
}

void GameLobbyController::setPlayerName(const QString& name)
{
    if (player_name != name) {
        player_name = name;
        emit playerNameChanged();
    }
}

void GameLobbyController::sendChatMessage()
{
    if (!chat_message.trimmed().isEmpty()) {
        QString formatted_message = QString("[%1]: %2").arg(player_name, chat_message);
        chat_history.append(formatted_message);
        emit chatHistoryChanged();
        
        setChatMessage("");
    }
}

void GameLobbyController::joinGame(int game_index)
{
    if (game_index >= 0 && game_index < game_model->rowCount()) {
        QModelIndex index = game_model->index(game_index);
        QString game_name = game_model->data(index, GameListModel::NameRole).toString();
        
        addSystemMessage(QString("Attempting to join game: %1").arg(game_name));
        emit gameJoined(game_name);
    }
}

void GameLobbyController::spectateGame(int game_index)
{
    if (game_index >= 0 && game_index < game_model->rowCount()) {
        QModelIndex index = game_model->index(game_index);
        QString game_name = game_model->data(index, GameListModel::NameRole).toString();
        
        addSystemMessage(QString("Spectating game: %1").arg(game_name));
    }
}

void GameLobbyController::createGame()
{
    addSystemMessage("Opening game creation dialog...");
    emit gameCreated();
}

void GameLobbyController::refreshGames()
{
    game_model->refresh();
    addSystemMessage("Game list refreshed.");
}

void GameLobbyController::openSettings()
{
    emit settingsRequested();
}

void GameLobbyController::addSystemMessage(const QString& message)
{
    QString formatted_message = QString("* %1").arg(message);
    chat_history.append(formatted_message);
    emit chatHistoryChanged();
}