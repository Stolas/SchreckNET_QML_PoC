#pragma once

#include <QObject>
#include <QAbstractListModel>
#include <QVariantMap>
#include <qqmlregistration.h>

// Card structure for MTG-style cards
struct Card {
    QString name;
    QString type;        // Land, Creature, Spell, Artifact, etc.
    QString manaCost;
    QString imageUrl;
    int quantity;
    QString rarity;      // Common, Uncommon, Rare, Mythic
};

// Player in the game
struct GamePlayer {
    QString name;
    QString status;      // Ready, Playing, Waiting, Disconnected, etc.
    int life;
    int handSize;
    bool isHost;
    QString avatar;
};

class DeckModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum DeckRoles {
        NameRole = Qt::UserRole + 1,
        TypeRole,
        ManaCostRole,
        ImageUrlRole,
        QuantityRole,
        RarityRole
    };

    explicit DeckModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void loadDeck(const QString &deckFile);
    Q_INVOKABLE void clearDeck();
    Q_INVOKABLE QStringList getCardTypes() const;
    Q_INVOKABLE QVariantList getCardsByType(const QString &type) const;

private:
    QList<Card> m_cards;
    void loadSampleDeck();
};

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

    explicit GamePlayersModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void updatePlayerStatus(const QString &playerName, const QString &status);
    Q_INVOKABLE void updatePlayerLife(const QString &playerName, int life);

private:
    QList<GamePlayer> m_players;
    void loadSamplePlayers();
};

class GameController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    
    Q_PROPERTY(DeckModel* deckModel READ deckModel CONSTANT)
    Q_PROPERTY(GamePlayersModel* playersModel READ playersModel CONSTANT)
    Q_PROPERTY(QString gameName READ gameName WRITE setGameName NOTIFY gameNameChanged)
    Q_PROPERTY(QString currentPlayer READ currentPlayer WRITE setCurrentPlayer NOTIFY currentPlayerChanged)
    Q_PROPERTY(QString gamePhase READ gamePhase WRITE setGamePhase NOTIFY gamePhaseChanged)
    Q_PROPERTY(QString chatMessage READ chatMessage WRITE setChatMessage NOTIFY chatMessageChanged)
    Q_PROPERTY(QStringList chatHistory READ chatHistory NOTIFY chatHistoryChanged)
    Q_PROPERTY(bool isHost READ isHost WRITE setIsHost NOTIFY isHostChanged)

public:
    explicit GameController(QObject *parent = nullptr);

    DeckModel* deckModel() const { return m_deckModel; }
    GamePlayersModel* playersModel() const { return m_playersModel; }
    QString gameName() const { return m_gameName; }
    QString currentPlayer() const { return m_currentPlayer; }
    QString gamePhase() const { return m_gamePhase; }
    QString chatMessage() const { return m_chatMessage; }
    QStringList chatHistory() const { return m_chatHistory; }
    bool isHost() const { return m_isHost; }

    void setGameName(const QString &name);
    void setCurrentPlayer(const QString &player);
    void setGamePhase(const QString &phase);
    void setChatMessage(const QString &message);
    void setIsHost(bool host);

public slots:
    Q_INVOKABLE void loadDeckFromFile();
    Q_INVOKABLE void sendChatMessage();
    Q_INVOKABLE void leaveGame();
    Q_INVOKABLE void startGame(); // Host only
    Q_INVOKABLE void endTurn();
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
    DeckModel* m_deckModel;
    GamePlayersModel* m_playersModel;
    QString m_gameName;
    QString m_currentPlayer;
    QString m_gamePhase;
    QString m_chatMessage;
    QStringList m_chatHistory;
    bool m_isHost;

    void addSystemMessage(const QString &message);
};