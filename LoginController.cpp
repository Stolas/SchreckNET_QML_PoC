#include "LoginController.h"
#include <QDebug>

LoginController::LoginController(QObject *parent)
    : QObject(parent)
    , m_port("4747")
    , m_savePassword(false)
    , m_autoConnect(false)
    , m_isConnected(false)
{
    loadPreviousHosts();
}

void LoginController::setSelectedHost(const QString &host)
{
    if (m_selectedHost != host) {
        m_selectedHost = host;
        emit selectedHostChanged();
        loadServerInfo(host);
    }
}

void LoginController::setHostUrl(const QString &url)
{
    if (m_hostUrl != url) {
        m_hostUrl = url;
        emit hostUrlChanged();
    }
}

void LoginController::setPort(const QString &port)
{
    if (m_port != port) {
        m_port = port;
        emit portChanged();
    }
}

void LoginController::setPlayerName(const QString &name)
{
    if (m_playerName != name) {
        m_playerName = name;
        emit playerNameChanged();
    }
}

void LoginController::setPassword(const QString &password)
{
    if (m_password != password) {
        m_password = password;
        emit passwordChanged();
    }
}

void LoginController::setSaveName(const QString &name)
{
    if (m_saveName != name) {
        m_saveName = name;
        emit saveNameChanged();
    }
}

void LoginController::setSavePassword(bool save)
{
    if (m_savePassword != save) {
        m_savePassword = save;
        emit savePasswordChanged();
        
        if (!save) {
            setAutoConnect(false);
        }
    }
}

void LoginController::setAutoConnect(bool autoConnect)
{
    if (m_autoConnect != autoConnect) {
        m_autoConnect = autoConnect;
        emit autoConnectChanged();
    }
}

void LoginController::refreshServers()
{
    // Simulate server refresh - in real implementation, this would fetch from network
    qDebug() << "Refreshing server list...";
    loadPreviousHosts();
}

bool LoginController::connectToServer()
{
    if (m_playerName.isEmpty()) {
        emit connectionFailed("Player name cannot be empty.");
        return false;
    }
    
    if (m_hostUrl.isEmpty()) {
        emit connectionFailed("Host URL cannot be empty.");
        return false;
    }
    
    // Simulate connection logic - in real implementation, this would attempt actual connection
    qDebug() << "Connecting to" << m_hostUrl << ":" << m_port << "as" << m_playerName;
    
    // For demo purposes, always succeed
    m_isConnected = true;
    emit isConnectedChanged();
    emit connectionSucceeded();
    return true;
}

void LoginController::forgotPassword()
{
    qDebug() << "Forgot password request for" << m_playerName << "on" << m_hostUrl;
    // In real implementation, this would trigger password reset logic
}

void LoginController::disconnect()
{
    m_isConnected = false;
    emit isConnectedChanged();
}

void LoginController::loadServerInfo(const QString &saveName)
{
    if (saveName.isEmpty()) {
        return;
    }
    
    // Simulate loading server info - in real implementation, this would load from settings
    if (saveName == "Official Server") {
        m_hostUrl = "server.schrecknet.com";
        m_port = "4747";
        m_serverContact = "https://schrecknet.com";
        m_serverIssues = "Official SchreckNET server. Contact support if you experience issues.";
    } else if (saveName == "Test Server") {
        m_hostUrl = "test.schrecknet.com";
        m_port = "4748";
        m_serverContact = "https://test.schrecknet.com";
        m_serverIssues = "Test server for development. May be unstable.";
    } else {
        m_hostUrl = "";
        m_port = "4747";
        m_serverContact = "";
        m_serverIssues = "";
    }
    
    emit hostUrlChanged();
    emit portChanged();
    emit serverContactChanged();
    emit serverIssuesChanged();
}

void LoginController::loadPreviousHosts()
{
    // Simulate loading previous hosts - in real implementation, this would load from settings
    m_previousHosts = {"Official Server", "Test Server", "Local Server"};
    emit previousHostsChanged();
    
    if (!m_previousHosts.isEmpty()) {
        setSelectedHost(m_previousHosts.first());
    }
}