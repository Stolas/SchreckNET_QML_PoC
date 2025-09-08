#pragma once

#include <QObject>
#include <QAbstractListModel>
#include <QVariantMap>
#include <qqmlregistration.h>

struct GameInfo {
    QString name;
    QString host;
    QString format;
    int currentPlayers;
    int maxPlayers;
    int spectators;
    bool passwordProtected;
    bool buddiesOnly;
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

    explicit GameListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addGame(const QString &name, const QString &host, const QString &format,
                           int currentPlayers, int maxPlayers, int spectators,
                           bool passwordProtected, bool buddiesOnly);
    Q_INVOKABLE void removeGame(int index);
    Q_INVOKABLE void refresh();

private:
    QList<GameInfo> m_games;
    void loadSampleGames();
};

class GameLobbyController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    
    Q_PROPERTY(GameListModel* gameModel READ gameModel CONSTANT)
    Q_PROPERTY(QString chatMessage READ chatMessage WRITE setChatMessage NOTIFY chatMessageChanged)
    Q_PROPERTY(QStringList chatHistory READ chatHistory NOTIFY chatHistoryChanged)
    Q_PROPERTY(QString playerName READ playerName WRITE setPlayerName NOTIFY playerNameChanged)

public:
    explicit GameLobbyController(QObject *parent = nullptr);

    GameListModel* gameModel() const { return m_gameModel; }
    QString chatMessage() const { return m_chatMessage; }
    QStringList chatHistory() const { return m_chatHistory; }
    QString playerName() const { return m_playerName; }

    void setChatMessage(const QString &message);
    void setPlayerName(const QString &name);

public slots:
    Q_INVOKABLE void sendChatMessage();
    Q_INVOKABLE void joinGame(int gameIndex);
    Q_INVOKABLE void spectateGame(int gameIndex);
    Q_INVOKABLE void createGame();
    Q_INVOKABLE void refreshGames();
    Q_INVOKABLE void openSettings();

signals:
    void chatMessageChanged();
    void chatHistoryChanged();
    void playerNameChanged();
    void gameJoined(const QString &gameName);
    void gameCreated();
    void settingsRequested();

private:
    GameListModel* m_gameModel;
    QString m_chatMessage;
    QStringList m_chatHistory;
    QString m_playerName;

    void addSystemMessage(const QString &message);
};