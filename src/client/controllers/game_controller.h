/*
 * Copyright (c) 2020-2025, Stolas <schrecknet@codeinject.org>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <QObject>
#include <QAbstractListModel>
#include <QVariantMap>
#include <qqmlregistration.h>

// Card class for VTEs cards
class Card
{
public:
    // Enum for VTEs card types
    enum class Type {
        Token           = 0x0000,
        Crypt           = 0x0001,
        Master          = 0x0002,
        Action          = 0x0004,
        ActionModifier  = 0x0008,
        PoliticalAction = 0x0010,
        Equipment       = 0x0020,
        Retainer        = 0x0040,
        Ally            = 0x0080,
        Combat          = 0x0100,
        Reaction        = 0x0200,
        Event           = 0x0400,
        Power           = 0x0800,
        Conviction      = 0x1000,
    };

    Card() = default;
    Card(const QString& name, Type type, const QString& image_url)
        : name(name), type(type), image_url(image_url) {}

    // Getters
    QString getName() const { return name; }
    Type getType() const { return type; }
    QString typeString() const { return cardTypeToString(type); }
    QString getImageUrl() const { return image_url; }
    int getQuantity() const { return quantity; }

    // Setters
    void setName(const QString& name_) { name = name_; }
    void setType(Type type_) { type = type_; }
    void setImageUrl(const QString& url) { image_url = url; }
    void setQuantity(int quantity_) { quantity = quantity_; }

    // Utility functions
    static QString cardTypeToString(Type type);
    static Type stringToCardType(const QString& typeStr);

private:
    QString name;
    Type type = Type::Token;
    QString image_url;
    int quantity = 0;
};

// Player in the game
struct GamePlayer
{
    QString name;
    QString status;      // Ready, Playing, Waiting, Disconnected, etc.
    int life;
    int hand_size;
    bool is_host;
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
        ImageUrlRole,
    };

    explicit DeckModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    Q_INVOKABLE int getCryptSize() const;
    Q_INVOKABLE int getLibrarySize() const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void loadDeck(const QString& deck_file);
    Q_INVOKABLE void clearDeck();
    Q_INVOKABLE QStringList getCardTypes() const;

    Q_INVOKABLE QVariantList getCryptCards() const;
    Q_INVOKABLE QVariantList getLibraryCards() const;
private:
    QList<Card> cards;
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
    Q_INVOKABLE void loadDeckFromFile();
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

