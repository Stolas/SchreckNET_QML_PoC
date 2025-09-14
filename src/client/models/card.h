/*
 * Copyright (c) 2020-2025, Stolas <schrecknet@codeinject.org>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <QString>

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
    QString text;
    // Todo; add rulings. contains of a list of rules with 'refs'. and each 'ref' contains an id and a url.
    QString image_url;
    int quantity = 0;
};
