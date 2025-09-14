/*
 * Copyright (c) 2020-2025, Stolas <schrecknet@codeinject.org>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <QAbstractListModel>
#include <QVariantMap>
#include <qqmlregistration.h>
#include "models/card.h"

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
    bool parseDeckFile(const QString& filePath);
    bool parseDeckLine(const QString& line);
    Card::Type parseCardType(const QString& typeString);
    QString generateImageUrl(const QString& cardName);
};