/*
 * Copyright (c) 2020-2025, Stolas <schrecknet@codeinject.org>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <QAbstractListModel>
#include <qqmlregistration.h>
#include "game/game_player.h"

class GamePlayersModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum PlayerRoles {
        NameRole = Qt::UserRole + 1,
        StatusRole,
        LifeRole,
        HandSizeRole,
        IsHostRole,
        AvatarRole
    };

    explicit GamePlayersModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void updatePlayerStatus(const QString& player_name, const QString& status);
    Q_INVOKABLE void updatePlayerLife(const QString& player_name, int life);

private:
    QList<GamePlayer> players;
    void loadSamplePlayers();
};