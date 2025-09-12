/*
 * Copyright (c) 2020-2025, Stolas <schrecknet@codeinject.org>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "login_controller.h"
#include <QDebug>

LoginController::LoginController(QObject* parent)
    : QObject(parent)
    , port("4747")
    , save_password(false)
    , auto_connect(false)
    , is_connected(false)
{
    loadPreviousHosts();
}

void LoginController::setSelectedHost(const QString& host)
{
    if (selected_host != host) {
        selected_host = host;
        emit selectedHostChanged();
        loadServerInfo(host);
    }
}

void LoginController::setHostUrl(const QString& url)
{
    if (host_url != url) {
        host_url = url;
        emit hostUrlChanged();
    }
}

void LoginController::setPort(const QString& port_)
{
    if (port != port_) {
        port = port_;
        emit portChanged();
    }
}

void LoginController::setPlayerName(const QString& name)
{
    if (player_name != name) {
        player_name = name;
        emit playerNameChanged();
    }
}

void LoginController::setPassword(const QString& password_)
{
    if (password != password_) {
        password = password_;
        emit passwordChanged();
    }
}

void LoginController::setSaveName(const QString& name)
{
    if (save_name != name) {
        save_name = name;
        emit saveNameChanged();
    }
}

void LoginController::setSavePassword(bool save)
{
    if (save_password != save) {
        save_password = save;
        emit savePasswordChanged();
        
        if (!save) {
            setAutoConnect(false);
        }
    }
}

void LoginController::setAutoConnect(bool auto_connect_)
{
    if (auto_connect != auto_connect_) {
        auto_connect = auto_connect_;
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
    if (player_name.isEmpty()) {
        emit connectionFailed("Player name cannot be empty.");
        return false;
    }
    
    if (host_url.isEmpty()) {
        emit connectionFailed("Host URL cannot be empty.");
        return false;
    }
    
    // Simulate connection logic - in real implementation, this would attempt actual connection
    qDebug() << "Connecting to" << host_url << ":" << port << "as" << player_name;
    
    // For demo purposes, always succeed
    is_connected = true;
    emit isConnectedChanged();
    emit connectionSucceeded();
    return true;
}

void LoginController::forgotPassword()
{
    qDebug() << "Forgot password request for" << player_name << "on" << host_url;
    // In real implementation, this would trigger password reset logic
}

void LoginController::disconnect()
{
    is_connected = false;
    emit isConnectedChanged();
}

void LoginController::loadServerInfo(const QString& save_name)
{
    if (save_name.isEmpty()) {
        return;
    }
    
    // Simulate loading server info - in real implementation, this would load from settings
    if (save_name == "Official Server") {
        host_url = "server.schrecknet.com";
        port = "4747";
        server_contact = "https://schrecknet.com";
        server_issues = "Official SchreckNET server. Contact support if you experience issues.";
    } else if (save_name == "Test Server") {
        host_url = "test.schrecknet.com";
        port = "4748";
        server_contact = "https://test.schrecknet.com";
        server_issues = "Test server for development. May be unstable.";
    } else {
        host_url = "";
        port = "4747";
        server_contact = "";
        server_issues = "";
    }
    
    emit hostUrlChanged();
    emit portChanged();
    emit serverContactChanged();
    emit serverIssuesChanged();
}

void LoginController::loadPreviousHosts()
{
    // Simulate loading previous hosts - in real implementation, this would load from settings
    previous_hosts = {"Official Server", "Test Server", "Local Server"};
    emit previousHostsChanged();
    
    if (!previous_hosts.isEmpty()) {
        setSelectedHost(previous_hosts.first());
    }
}