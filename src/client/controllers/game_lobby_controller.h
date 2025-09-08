/*
 * Copyright (c) 2020-2025, Stolas <schrecknet@codeinject.org>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <QObject>
#include <QAbstractListModel>
#include <QVariantMap>
#include <qqmlregistration.h>

struct GameInfo
{
    QString name;
    QString host;
    QString format;
    int current_players;
    int max_players;
    int spectators;
    bool password_protected;
    bool buddies_only;
};

class GameListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum GameRoles {
        NameRole = Qt::UserRole + 1,
        HostRole,
        FormatRole,
        CurrentPlayersRole,
        MaxPlayersRole,
        SpectatorsRole,
        PasswordProtectedRole,
        BuddiesOnlyRole
    };

    explicit GameListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addGame(const QString& name, const QString& host, const QString& format,
                           int current_players, int max_players, int spectators,
                           bool password_protected, bool buddies_only);
    Q_INVOKABLE void removeGame(int index);
    Q_INVOKABLE void refresh();

private:
    QList<GameInfo> games;
    void loadSampleGames();
};

class GameLobbyController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    
    Q_PROPERTY(GameListModel* gameModel READ getGameModel CONSTANT)
    Q_PROPERTY(QString chatMessage READ getChatMessage WRITE setChatMessage NOTIFY chatMessageChanged)
    Q_PROPERTY(QStringList chatHistory READ getChatHistory NOTIFY chatHistoryChanged)
    Q_PROPERTY(QString playerName READ getPlayerName WRITE setPlayerName NOTIFY playerNameChanged)

public:
    explicit GameLobbyController(QObject* parent = nullptr);

    GameListModel* getGameModel() const { return game_model; }
    QString getChatMessage() const { return chat_message; }
    QStringList getChatHistory() const { return chat_history; }
    QString getPlayerName() const { return player_name; }

    void setChatMessage(const QString& message);
    void setPlayerName(const QString& name);

public slots:
    Q_INVOKABLE void sendChatMessage();
    Q_INVOKABLE void joinGame(int game_index);
    Q_INVOKABLE void spectateGame(int game_index);
    Q_INVOKABLE void createGame();
    Q_INVOKABLE void refreshGames();
    Q_INVOKABLE void openSettings();

signals:
    void chatMessageChanged();
    void chatHistoryChanged();
    void playerNameChanged();
    void gameJoined(const QString& game_name);
    void gameCreated();
    void settingsRequested();

private:
    GameListModel* game_model;
    QString chat_message;
    QStringList chat_history;
    QString player_name;

    void addSystemMessage(const QString& message);
};