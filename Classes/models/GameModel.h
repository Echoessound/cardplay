#pragma once

#include <vector>
#include "cocos2d.h"
#include "models/CardModel.h"

/**
 * @brief Runtime data model for a full game session.
 *
 * Stores state only; no complicated business logic.
 * Supports simple serialization for save/load flows.
 */
class GameModel
{
public:
    GameModel();
    ~GameModel();

    /// All cards currently in the game (playfield + stack).
    std::vector<CardModel> cards;

    /// Current stack-top card ID, -1 if none.
    int currentStackTopCardId;

    /// Fixed base position for the stack (e.g. visual tray slot from level config).
    cocos2d::Vec2 stackBasePosition;

    /// Serialize all data into a ValueMap.
    cocos2d::ValueMap toValueMap() const;

    /// Deserialize from a ValueMap.
    void fromValueMap(const cocos2d::ValueMap& map);

    /// Lookup card model by ID, nullptr if not found.
    CardModel* findCard(int cardId);
    const CardModel* findCard(int cardId) const;
};



