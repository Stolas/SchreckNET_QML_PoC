/*
 * Copyright (c) 2020-2025, Stolas <schrecknet@codeinject.org>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <qqmlregistration.h>

class LoginController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    
    Q_PROPERTY(QStringList previousHosts READ getPreviousHosts NOTIFY previousHostsChanged)
    Q_PROPERTY(QString selectedHost READ getSelectedHost WRITE setSelectedHost NOTIFY selectedHostChanged)
    Q_PROPERTY(QString hostUrl READ getHostUrl WRITE setHostUrl NOTIFY hostUrlChanged)
    Q_PROPERTY(QString port READ getPort WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString playerName READ getPlayerName WRITE setPlayerName NOTIFY playerNameChanged)
    Q_PROPERTY(QString password READ getPassword WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString saveName READ getSaveName WRITE setSaveName NOTIFY saveNameChanged)
    Q_PROPERTY(bool savePassword READ getSavePassword WRITE setSavePassword NOTIFY savePasswordChanged)
    Q_PROPERTY(bool autoConnect READ getAutoConnect WRITE setAutoConnect NOTIFY autoConnectChanged)
    Q_PROPERTY(bool isConnected READ getIsConnected NOTIFY isConnectedChanged)
    Q_PROPERTY(QString serverContact READ getServerContact NOTIFY serverContactChanged)
    Q_PROPERTY(QString serverIssues READ getServerIssues NOTIFY serverIssuesChanged)

public:
    explicit LoginController(QObject* parent = nullptr);

    // Property getters
    QStringList getPreviousHosts() const { return previous_hosts; }
    QString getSelectedHost() const { return selected_host; }
    QString getHostUrl() const { return host_url; }
    QString getPort() const { return port; }
    QString getPlayerName() const { return player_name; }
    QString getPassword() const { return password; }
    QString getSaveName() const { return save_name; }
    bool getSavePassword() const { return save_password; }
    bool getAutoConnect() const { return auto_connect; }
    bool getIsConnected() const { return is_connected; }
    QString getServerContact() const { return server_contact; }
    QString getServerIssues() const { return server_issues; }

    // Property setters
    void setSelectedHost(const QString& host);
    void setHostUrl(const QString& url);
    void setPort(const QString& port_);
    void setPlayerName(const QString& name);
    void setPassword(const QString& password_);
    void setSaveName(const QString& name);
    void setSavePassword(bool save);
    void setAutoConnect(bool auto_connect_);

public slots:
    Q_INVOKABLE void refreshServers();
    Q_INVOKABLE bool connectToServer();
    Q_INVOKABLE void forgotPassword();
    Q_INVOKABLE void disconnect();
    Q_INVOKABLE void loadServerInfo(const QString& save_name);

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
    void connectionFailed(const QString& error);
    void connectionSucceeded();

private:
    void loadPreviousHosts();
    void updateServerInfo();

    QStringList previous_hosts;
    QString selected_host;
    QString host_url;
    QString port;
    QString player_name;
    QString password;
    QString save_name;
    bool save_password;
    bool auto_connect;
    bool is_connected;
    QString server_contact;
    QString server_issues;
};