/*
 * Copyright (c) 2020-2025, Stolas <schrecknet@codeinject.org>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <QString>

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