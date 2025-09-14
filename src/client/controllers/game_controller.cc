/*
 * Copyright (c) 2020-2025, Stolas <schrecknet@codeinject.org>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "game_controller.h"
#include <QDebug>
#include <QUrl>

// GameController implementation
GameController::GameController(QObject* parent)
    : QObject(parent)
    , deck_model(new DeckModel(this))
    , players_model(new GamePlayersModel(this))
    , game_name("Casual Standard")
    , current_player("PlayerOne")
    , game_phase("")
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

void GameController::loadDeckFromFile(const QUrl& fileUrl)
{
    if (fileUrl.isEmpty()) {
        // No file selected, load sample deck
        addSystemMessage("No file selected. Loading sample deck...");
        deck_model->loadDeck("");
        addSystemMessage("Sample deck loaded successfully! 60 cards total.");
    } else {
        // Convert QUrl to local file path
        QString filePath = fileUrl.toLocalFile();
        if (filePath.isEmpty()) {
            addSystemMessage("Invalid file path. Loading sample deck...");
            deck_model->loadDeck("");
            addSystemMessage("Sample deck loaded successfully! 60 cards total.");
        } else {
            addSystemMessage(QString("Loading deck from: %1").arg(filePath));
            deck_model->loadDeck(filePath);
            addSystemMessage("Deck loaded successfully!");
            emit deckLoaded();
        }
    }
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
        setGamePhase("Unlock");
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
