#pragma once

#include <vector>
#include "cocos2d.h"

/**
 * @brief Single card snapshot used to compare states before/after an operation.
 */
struct CardStateSnapshot
{
    int cardId = -1;
    cocos2d::Vec2 position = cocos2d::Vec2::ZERO;
    bool inPlayfield = false;
    bool inStack = false;
    bool isTopOfStack = false;
    bool isFaceUp = true;
};

/**
 * @brief Undo record describing one reversible action.
 *
 * Simplified assumption: each action touches at most two cards.
 * - firstXXX usually points to the stack-top card leaving/entering the stack.
 * - secondXXX stands for the moving card (playfield or stack).
 * When an action involves only one card, set canUseSecond to false.
 */
struct UndoRecord
{
    CardStateSnapshot firstBefore;
    CardStateSnapshot secondBefore;
    CardStateSnapshot firstAfter;
    CardStateSnapshot secondAfter;
    bool canUseSecond = false;
};

/**
 * @brief Undo manager keeps a stack of UndoRecord instances.
 *
 * Responsibilities:
 * - Owned by controller, but does not depend on controller APIs.
 * - Lightweight and easy to test.
 * - Only stores UndoRecord structs rather than owning models.
 */
class UndoManager
{
public:
    UndoManager() = default;
    ~UndoManager() = default;

    /// Push a record onto the stack.
    void pushRecord(const UndoRecord& record);

    /// Pop latest record; returns false if stack is empty.
    bool popRecord(UndoRecord& outRecord);

    /// Clear all records.
    void clear();

    /// Whether there is at least one undo entry.
    bool hasRecord() const;

private:
    std::vector<UndoRecord> _records;
};



