#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <qqmlregistration.h>

class LoginController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    
    Q_PROPERTY(QStringList previousHosts READ previousHosts NOTIFY previousHostsChanged)
    Q_PROPERTY(QString selectedHost READ selectedHost WRITE setSelectedHost NOTIFY selectedHostChanged)
    Q_PROPERTY(QString hostUrl READ hostUrl WRITE setHostUrl NOTIFY hostUrlChanged)
    Q_PROPERTY(QString port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString playerName READ playerName WRITE setPlayerName NOTIFY playerNameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString saveName READ saveName WRITE setSaveName NOTIFY saveNameChanged)
    Q_PROPERTY(bool savePassword READ savePassword WRITE setSavePassword NOTIFY savePasswordChanged)
    Q_PROPERTY(bool autoConnect READ autoConnect WRITE setAutoConnect NOTIFY autoConnectChanged)
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)
    Q_PROPERTY(QString serverContact READ serverContact NOTIFY serverContactChanged)
    Q_PROPERTY(QString serverIssues READ serverIssues NOTIFY serverIssuesChanged)

public:
    explicit LoginController(QObject *parent = nullptr);

    // Property getters
    QStringList previousHosts() const { return m_previousHosts; }
    QString selectedHost() const { return m_selectedHost; }
    QString hostUrl() const { return m_hostUrl; }
    QString port() const { return m_port; }
    QString playerName() const { return m_playerName; }
    QString password() const { return m_password; }
    QString saveName() const { return m_saveName; }
    bool savePassword() const { return m_savePassword; }
    bool autoConnect() const { return m_autoConnect; }
    bool isConnected() const { return m_isConnected; }
    QString serverContact() const { return m_serverContact; }
    QString serverIssues() const { return m_serverIssues; }

    // Property setters
    void setSelectedHost(const QString &host);
    void setHostUrl(const QString &url);
    void setPort(const QString &port);
    void setPlayerName(const QString &name);
    void setPassword(const QString &password);
    void setSaveName(const QString &name);
    void setSavePassword(bool save);
    void setAutoConnect(bool autoConnect);

public slots:
    Q_INVOKABLE void refreshServers();
    Q_INVOKABLE bool connectToServer();
    Q_INVOKABLE void forgotPassword();
    Q_INVOKABLE void disconnect();
    Q_INVOKABLE void loadServerInfo(const QString &saveName);

signals:
    void previousHostsChanged();
    void selectedHostChanged();
    void hostUrlChanged();
    void portChanged();
    void playerNameChanged();
    void passwordChanged();
    void saveNameChanged();
    void savePasswordChanged();
    void autoConnectChanged();
    void isConnectedChanged();
    void serverContactChanged();
    void serverIssuesChanged();
    void connectionFailed(const QString &error);
    void connectionSucceeded();

private:
    void loadPreviousHosts();
    void updateServerInfo();

    QStringList m_previousHosts;
    QString m_selectedHost;
    QString m_hostUrl;
    QString m_port;
    QString m_playerName;
    QString m_password;
    QString m_saveName;
    bool m_savePassword;
    bool m_autoConnect;
    bool m_isConnected;
    QString m_serverContact;
    QString m_serverIssues;
};