/*
 * Copyright (c) 2020-2025, Stolas <schrecknet@codeinject.org>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "card.h"
#include <QStringList>

// Card utility functions implementation
QString Card::cardTypeToString(Card::Type type) {
  QStringList type_strings;

  // Check each flag and add corresponding string
  if (static_cast<int>(type) & static_cast<int>(Type::Crypt))
    type_strings << "Crypt";
  if (static_cast<int>(type) & static_cast<int>(Type::Master))
    type_strings << "Master";
  if (static_cast<int>(type) & static_cast<int>(Type::Action))
    type_strings << "Action";
  if (static_cast<int>(type) & static_cast<int>(Type::ActionModifier))
    type_strings << "Modifier";
  if (static_cast<int>(type) & static_cast<int>(Type::PoliticalAction))
    type_strings << "Political Action";
  if (static_cast<int>(type) & static_cast<int>(Type::Equipment))
    type_strings << "Equipment";
  if (static_cast<int>(type) & static_cast<int>(Type::Retainer))
    type_strings << "Retainer";
  if (static_cast<int>(type) & static_cast<int>(Type::Ally))
    type_strings << "Ally";
  if (static_cast<int>(type) & static_cast<int>(Type::Combat))
    type_strings << "Combat";
  if (static_cast<int>(type) & static_cast<int>(Type::Reaction))
    type_strings << "Reaction";
  if (static_cast<int>(type) & static_cast<int>(Type::Event))
    type_strings << "Event";
  if (static_cast<int>(type) & static_cast<int>(Type::Power))
    type_strings << "Power";
  if (static_cast<int>(type) & static_cast<int>(Type::Conviction))
    type_strings << "Conviction";

  // Handle special cases
  if (type_strings.isEmpty())
    return "Unknown";

  // Join multiple types with "/"
  return type_strings.join("/");
}

Card::Type Card::stringToCardType(const QString &typeStr) {
  // Handle single types first
  if (typeStr == "Crypt")
    return static_cast<Type>(static_cast<int>(Type::Crypt));
  if (typeStr == "Master")
    return static_cast<Type>(static_cast<int>(Type::Master));
  if (typeStr == "Action")
    return static_cast<Type>(static_cast<int>(Type::Action));
  if (typeStr == "Modifier")
    return static_cast<Type>(static_cast<int>(Type::ActionModifier));
  if (typeStr == "Political Action")
    return static_cast<Type>(static_cast<int>(Type::PoliticalAction));
  if (typeStr == "Equipment")
    return static_cast<Type>(static_cast<int>(Type::Equipment));
  if (typeStr == "Retainer")
    return static_cast<Type>(static_cast<int>(Type::Retainer));
  if (typeStr == "Ally")
    return static_cast<Type>(static_cast<int>(Type::Ally));
  if (typeStr == "Combat")
    return static_cast<Type>(static_cast<int>(Type::Combat));
  if (typeStr == "Reaction")
    return static_cast<Type>(static_cast<int>(Type::Reaction));
  if (typeStr == "Event")
    return static_cast<Type>(static_cast<int>(Type::Event));
  if (typeStr == "Power")
    return static_cast<Type>(static_cast<int>(Type::Power));
  if (typeStr == "Conviction")
    return static_cast<Type>(static_cast<int>(Type::Conviction));

  // Handle compound types (e.g., "Action/Combat")
  if (typeStr.contains("/")) {
    QStringList parts = typeStr.split("/");
    int combinedType = 0;
    for (const QString &part : parts) {
      Card::Type partType = stringToCardType(part.trimmed());
      combinedType |= static_cast<int>(partType);
    }
    return static_cast<Type>(combinedType);
  }

  // Default to Token for unknown types
  return Type::Token;
}