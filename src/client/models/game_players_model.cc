/*
 * Copyright (c) 2020-2025, Stolas <schrecknet@codeinject.org>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "game_players_model.h"

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