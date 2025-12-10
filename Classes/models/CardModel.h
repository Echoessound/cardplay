#pragma once

#include "cocos2d.h"
#include "models/CardEnums.h"

/**
 * @brief Runtime data model for a single card (no heavy logic).
 */
class CardModel
{
public:
    int cardId;              ///< Logical unique identifier (mapped to view/undo).
    CardFaceType faceType;   ///< Rank.
    CardSuitType suitType;   ///< Suit.

    bool isFaceUp;           ///< Whether face-up.
    bool inPlayfield;        ///< Whether inside playfield area.
    bool inStack;            ///< Whether inside stack area.
    bool isTopOfStack;       ///< Whether currently on top of stack.

    cocos2d::Vec2 position;  ///< Current world position (for persistence).

    CardModel();

    /** Serialize into ValueMap for save data. */
    cocos2d::ValueMap toValueMap() const;

    /** Deserialize from ValueMap to restore. */
    void fromValueMap(const cocos2d::ValueMap& map);
};


