/*
 * Copyright (c) 2020-2025, Stolas <schrecknet@codeinject.org>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <QObject>
#include <QStringList>
#include <QUrl>
#include <qqmlregistration.h>
#include "models/deck_model.h"
#include "models/game_players_model.h"

class GameController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    
    Q_PROPERTY(DeckModel* deckModel READ getDeckModel CONSTANT)
    Q_PROPERTY(GamePlayersModel* playersModel READ getPlayersModel CONSTANT)
    Q_PROPERTY(QString gameName READ getGameName WRITE setGameName NOTIFY gameNameChanged)
    Q_PROPERTY(QString currentPlayer READ getCurrentPlayer WRITE setCurrentPlayer NOTIFY currentPlayerChanged)
    Q_PROPERTY(QString gamePhase READ getGamePhase WRITE setGamePhase NOTIFY gamePhaseChanged)
    Q_PROPERTY(QString chatMessage READ getChatMessage WRITE setChatMessage NOTIFY chatMessageChanged)
    Q_PROPERTY(QStringList chatHistory READ getChatHistory NOTIFY chatHistoryChanged)
    Q_PROPERTY(bool isHost READ getIsHost WRITE setIsHost NOTIFY isHostChanged)

public:
    explicit GameController(QObject* parent = nullptr);

    DeckModel* getDeckModel() const { return deck_model; }
    GamePlayersModel* getPlayersModel() const { return players_model; }
    QString getGameName() const { return game_name; }
    QString getCurrentPlayer() const { return current_player; }
    QString getGamePhase() const { return game_phase; }
    QString getChatMessage() const { return chat_message; }
    QStringList getChatHistory() const { return chat_history; }
    bool getIsHost() const { return is_host; }

    void setGameName(const QString& name);
    void setCurrentPlayer(const QString& player);
    void setGamePhase(const QString& phase);
    void setChatMessage(const QString& message);
    void setIsHost(bool host);

public slots:
    Q_INVOKABLE void loadDeckFromFile(const QUrl& fileUrl = QUrl());
    Q_INVOKABLE void sendChatMessage();
    Q_INVOKABLE void leaveGame();
    Q_INVOKABLE void startGame(); // Host only
    Q_INVOKABLE void endTurn();
    Q_INVOKABLE void setReady();
    Q_INVOKABLE void concede();

signals:
    void gameNameChanged();
    void currentPlayerChanged();
    void gamePhaseChanged();
    void chatMessageChanged();
    void chatHistoryChanged();
    void isHostChanged();
    void gameLeft();
    void deckLoaded();

private:
    DeckModel* deck_model;
    GamePlayersModel* players_model;
    QString game_name;
    QString current_player;
    QString game_phase;
    QString chat_message;
    QStringList chat_history;
    bool is_host;

    void addSystemMessage(const QString& message);
};

