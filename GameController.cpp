#include "GameController.h"
#include <QDebug>
#include <QStandardPaths>

// DeckModel implementation
DeckModel::DeckModel(QObject *parent)
    : QAbstractListModel(parent)
{
    loadSampleDeck();
}

int DeckModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_cards.size();
}

QVariant DeckModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_cards.size())
        return QVariant();

    const Card &card = m_cards[index.row()];

    switch (role) {
    case NameRole:
        return card.name;
    case TypeRole:
        return card.type;
    case ManaCostRole:
        return card.manaCost;
    case ImageUrlRole:
        return card.imageUrl;
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

void DeckModel::loadDeck(const QString &deckFile)
{
    Q_UNUSED(deckFile)
    // In a real implementation, this would parse a deck file format
    beginResetModel();
    m_cards.clear();
    loadSampleDeck();
    endResetModel();
}

void DeckModel::clearDeck()
{
    beginResetModel();
    m_cards.clear();
    endResetModel();
}

QStringList DeckModel::getCardTypes() const
{
    QStringList types;
    for (const Card &card : m_cards) {
        if (!types.contains(card.type)) {
            types.append(card.type);
        }
    }
    return types;
}

QVariantList DeckModel::getCardsByType(const QString &type) const
{
    QVariantList cards;
    for (const Card &card : m_cards) {
        if (card.type == type) {
            QVariantMap cardMap;
            cardMap["name"] = card.name;
            cardMap["type"] = card.type;
            cardMap["manaCost"] = card.manaCost;
            cardMap["imageUrl"] = card.imageUrl;
            cardMap["quantity"] = card.quantity;
            cardMap["rarity"] = card.rarity;
            cards.append(cardMap);
        }
    }
    return cards;
}

void DeckModel::loadSampleDeck()
{

    // Sample VTEs
    m_cards = {
        // Crypt
        {"Howler", "Crypt", "", "https://static.krcg.org/card/howler.jpg", 4, ""},
        {"Siamese", "Crypt", "", "https://static.krcg.org/card/siamese.jpg", 3, ""},
        {"Cynthia", "Crypt", "", "https://static.krcg.org/card/cynthia.jpg", 3, ""},
        {"Nettie", "Crypt", "", "https://static.krcg.org/card/nettie.jpg", 1, ""},
        {"Juanita", "Crypt", "", "https://static.krcg.org/card/juanita.jpg", 1, ""},
        // Master
        {"Archon Investigation", "Master", "", "https://static.krcg.org/card/archoninvestigation.jpg", 1, ""},
        {"Guardian Angel", "Master", "", "https://static.krcg.org/card/guardianangel.jpg", 1, ""},
        {"Powerbase: Montreal", "Master", "", "https://static.krcg.org/card/powerbasemontreal.jpg", 1, ""},
        {"Rack, The", "Master", "", "https://static.krcg.org/card/therack.jpg", 1, ""},
        {"Smiling Jack, The Anarch", "Master", "", "https://static.krcg.org/card/smilingjacktheanarch.jpg", 2, ""},
        {"Vessel", "Master", "", "https://static.krcg.org/card/vessel.jpg", 4, ""},
        {"Villein", "Master", "", "https://static.krcg.org/card/villein.jpg", 4, ""},
        // Action
        {"Abbot", "Action", "", "https://static.krcg.org/card/abbot.jpg", 2, ""},
        {"Army of Rats", "Action", "", "https://static.krcg.org/card/armyofrats.jpg", 1, ""},
        {"Charge of the Buffalo", "Action", "", "https://static.krcg.org/card/chargeofthebuffalo.jpg", 2, ""},
        {"Enchant Kindred", "Action", "", "https://static.krcg.org/card/enchantkindred.jpg", 7, ""},
        {"Engling Fury", "Action", "", "https://static.krcg.org/card/englingfury.jpg", 4, ""},
        // Ally
        {"High Top", "Ally", "", "https://static.krcg.org/card/hightop.jpg", 1, ""},
        {"Ossian", "Ally", "", "https://static.krcg.org/card/ossian.jpg", 1, ""},
        // Action Modifier
        {"Aire of Elation", "Modifier", "", "https://static.krcg.org/card/aireofelation.jpg", 2, ""},
        {"Squirrel Balance", "Modifier", "", "https://static.krcg.org/card/squirrelbalance.jpg", 3, ""},
        // Action Modifier/Combat
        {"Swiftness of the Stag", "Mixed", "", "https://static.krcg.org/card/swiftnessofthestag.jpg", 8, ""},
        // Reaction
        {"Cats' Guidance", "Reaction", "", "https://static.krcg.org/card/catsguidance.jpg", 4, ""},
        {"Ears of the Hare", "Reaction", "", "https://static.krcg.org/card/earsofthehare.jpg", 6, ""},
        {"Falcon's Eye", "Reaction", "", "https://static.krcg.org/card/falconseye.jpg", 1, ""},
        {"On the Qui Vive", "Reaction", "", "https://static.krcg.org/card/onthequivive.jpg", 3, ""},
        {"Speak with Spirits", "Reaction", "", "https://static.krcg.org/card/speakwithspirits.jpg", 8, ""},
        // Combat
        {"Canine Horde", "Combat", "", "https://static.krcg.org/card/caninehorde.jpg", 1, ""},
        {"Carrion Crows", "Combat", "", "https://static.krcg.org/card/carrioncrows.jpg", 3, ""},
        {"Drawing Out the Beast", "Combat", "", "https://static.krcg.org/card/drawingoutthebeast.jpg", 2, ""},
        {"Target Vitals", "Combat", "", "https://static.krcg.org/card/targetvitals.jpg", 6, ""},
        {"Taste of Vitae", "Combat", "", "https://static.krcg.org/card/tasteofvitae.jpg", 4, ""},
        {"Weighted Walking Stick", "Combat", "", "https://static.krcg.org/card/weightedwalkingstick.jpg", 6, ""},
        // Event
        {"Narrow Minds", "Event", "", "https://static.krcg.org/card/narrowminds.jpg", 1, ""},
    };
}

// GamePlayersModel implementation
GamePlayersModel::GamePlayersModel(QObject *parent)
    : QAbstractListModel(parent)
{
    loadSamplePlayers();
}

int GamePlayersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_players.size();
}

QVariant GamePlayersModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_players.size())
        return QVariant();

    const GamePlayer &player = m_players[index.row()];

    switch (role) {
    case NameRole:
        return player.name;
    case StatusRole:
        return player.status;
    case LifeRole:
        return player.life;
    case HandSizeRole:
        return player.handSize;
    case IsHostRole:
        return player.isHost;
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

void GamePlayersModel::updatePlayerStatus(const QString &playerName, const QString &status)
{
    for (int i = 0; i < m_players.size(); ++i) {
        if (m_players[i].name == playerName) {
            m_players[i].status = status;
            QModelIndex idx = index(i);
            emit dataChanged(idx, idx, {StatusRole});
            break;
        }
    }
}

void GamePlayersModel::updatePlayerLife(const QString &playerName, int life)
{
    for (int i = 0; i < m_players.size(); ++i) {
        if (m_players[i].name == playerName) {
            m_players[i].life = life;
            QModelIndex idx = index(i);
            emit dataChanged(idx, idx, {LifeRole});
            break;
        }
    }
}

void GamePlayersModel::loadSamplePlayers()
{
    m_players = {
        {"PlayerOne", "Ready", 20, 7, true, "qrc:/images/avatar1.png"},
        {"ProPlayer", "Playing", 18, 5, false, "qrc:/images/avatar2.png"},
        {"CurrentUser", "Playing", 20, 6, false, "qrc:/images/avatar3.png"}
    };
}

// GameController implementation
GameController::GameController(QObject *parent)
    : QObject(parent)
    , m_deckModel(new DeckModel(this))
    , m_playersModel(new GamePlayersModel(this))
    , m_gameName("Casual Standard")
    , m_currentPlayer("PlayerOne")
    , m_gamePhase("Main Phase")
    , m_isHost(false)
{
    addSystemMessage("Game joined successfully!");
    addSystemMessage("Load your deck to begin playing.");
}

void GameController::setGameName(const QString &name)
{
    if (m_gameName != name) {
        m_gameName = name;
        emit gameNameChanged();
    }
}

void GameController::setCurrentPlayer(const QString &player)
{
    if (m_currentPlayer != player) {
        m_currentPlayer = player;
        emit currentPlayerChanged();
    }
}

void GameController::setGamePhase(const QString &phase)
{
    if (m_gamePhase != phase) {
        m_gamePhase = phase;
        emit gamePhaseChanged();
    }
}

void GameController::setChatMessage(const QString &message)
{
    if (m_chatMessage != message) {
        m_chatMessage = message;
        emit chatMessageChanged();
    }
}

void GameController::setIsHost(bool host)
{
    if (m_isHost != host) {
        m_isHost = host;
        emit isHostChanged();
    }
}

void GameController::loadDeckFromFile()
{
    // In a real implementation, this would open a file dialog
    addSystemMessage("Loading deck...");
    m_deckModel->loadDeck("");
    addSystemMessage("Deck loaded successfully! 60 cards total.");
    emit deckLoaded();
}

void GameController::sendChatMessage()
{
    if (!m_chatMessage.trimmed().isEmpty()) {
        QString formattedMessage = QString("[CurrentUser]: %1").arg(m_chatMessage);
        m_chatHistory.append(formattedMessage);
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
    if (m_isHost) {
        addSystemMessage("Starting game...");
        setGamePhase("Upkeep");
        m_playersModel->updatePlayerStatus("PlayerOne", "Playing");
        m_playersModel->updatePlayerStatus("ProPlayer", "Playing");
        m_playersModel->updatePlayerStatus("CurrentUser", "Playing");
    }
}

void GameController::endTurn()
{
    addSystemMessage("Turn ended.");
    // In a real implementation, this would handle turn logic
    if (m_currentPlayer == "CurrentUser") {
        setCurrentPlayer("PlayerOne");
    } else if (m_currentPlayer == "PlayerOne") {
        setCurrentPlayer("ProPlayer");
    } else {
        setCurrentPlayer("CurrentUser");
    }
}

void GameController::concede()
{
    addSystemMessage("You have conceded the game.");
    m_playersModel->updatePlayerStatus("CurrentUser", "Conceded");
}

void GameController::addSystemMessage(const QString &message)
{
    QString formattedMessage = QString("* %1").arg(message);
    m_chatHistory.append(formattedMessage);
    emit chatHistoryChanged();
}
