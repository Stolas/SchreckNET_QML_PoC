#include "GameLobbyController.h"
#include <QDebug>

// GameListModel implementation
GameListModel::GameListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    loadSampleGames();
}

int GameListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_games.size();
}

QVariant GameListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_games.size())
        return QVariant();

    const GameInfo &game = m_games[index.row()];

    switch (role) {
    case NameRole:
        return game.name;
    case HostRole:
        return game.host;
    case FormatRole:
        return game.format;
    case CurrentPlayersRole:
        return game.currentPlayers;
    case MaxPlayersRole:
        return game.maxPlayers;
    case SpectatorsRole:
        return game.spectators;
    case PasswordProtectedRole:
        return game.passwordProtected;
    case BuddiesOnlyRole:
        return game.buddiesOnly;
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

void GameListModel::addGame(const QString &name, const QString &host, const QString &format,
                          int currentPlayers, int maxPlayers, int spectators,
                          bool passwordProtected, bool buddiesOnly)
{
    beginInsertRows(QModelIndex(), m_games.size(), m_games.size());
    m_games.append({name, host, format, currentPlayers, maxPlayers, spectators, passwordProtected, buddiesOnly});
    endInsertRows();
}

void GameListModel::removeGame(int index)
{
    if (index >= 0 && index < m_games.size()) {
        beginRemoveRows(QModelIndex(), index, index);
        m_games.removeAt(index);
        endRemoveRows();
    }
}

void GameListModel::refresh()
{
    beginResetModel();
    m_games.clear();
    loadSampleGames();
    endResetModel();
}

void GameListModel::loadSampleGames()
{
    // Sample game data
    m_games = {
        /* Name, Host Player, Format, Player Count, Seats Available, Spectator Count, Password, Buddies */
        {"Casual Standard", "PlayerOne", "Rated Regular", 2, 5, 1, false, false},
        {"Competitive Modern", "ProPlayer", "Casual Dual", 1, 2, 0, false, false},
        {"Friends Only", "BuddyHost", "Legacy", 3, 5, 0, false, true},
        {"Tournament Practice", "TourneyPrep", "Standard", 4, 5, 2, true, false},
        {"Beginner Friendly", "NewbieHelper", "Pauper", 1, 5, 0, true, true}
    };
}

// GameLobbyController implementation
GameLobbyController::GameLobbyController(QObject *parent)
    : QObject(parent)
    , m_gameModel(new GameListModel(this))
    , m_playerName("Player")
{
    addSystemMessage("Welcome to SchreckNET! Connected to server.");
    addSystemMessage("Type your message and press Enter to chat.");
}

void GameLobbyController::setChatMessage(const QString &message)
{
    if (m_chatMessage != message) {
        m_chatMessage = message;
        emit chatMessageChanged();
    }
}

void GameLobbyController::setPlayerName(const QString &name)
{
    if (m_playerName != name) {
        m_playerName = name;
        emit playerNameChanged();
    }
}

void GameLobbyController::sendChatMessage()
{
    if (!m_chatMessage.trimmed().isEmpty()) {
        QString formattedMessage = QString("[%1]: %2").arg(m_playerName, m_chatMessage);
        m_chatHistory.append(formattedMessage);
        emit chatHistoryChanged();
        
        setChatMessage("");
    }
}

void GameLobbyController::joinGame(int gameIndex)
{
    if (gameIndex >= 0 && gameIndex < m_gameModel->rowCount()) {
        QModelIndex index = m_gameModel->index(gameIndex);
        QString gameName = m_gameModel->data(index, GameListModel::NameRole).toString();
        
        addSystemMessage(QString("Attempting to join game: %1").arg(gameName));
        emit gameJoined(gameName);
    }
}

void GameLobbyController::spectateGame(int gameIndex)
{
    if (gameIndex >= 0 && gameIndex < m_gameModel->rowCount()) {
        QModelIndex index = m_gameModel->index(gameIndex);
        QString gameName = m_gameModel->data(index, GameListModel::NameRole).toString();
        
        addSystemMessage(QString("Spectating game: %1").arg(gameName));
    }
}

void GameLobbyController::createGame()
{
    addSystemMessage("Opening game creation dialog...");
    emit gameCreated();
}

void GameLobbyController::refreshGames()
{
    m_gameModel->refresh();
    addSystemMessage("Game list refreshed.");
}

void GameLobbyController::openSettings()
{
    emit settingsRequested();
}

void GameLobbyController::addSystemMessage(const QString &message)
{
    QString formattedMessage = QString("* %1").arg(message);
    m_chatHistory.append(formattedMessage);
    emit chatHistoryChanged();
}
