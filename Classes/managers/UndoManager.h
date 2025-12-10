#pragma once

#include <vector>
#include "cocos2d.h"

/**
 * @brief 用于比较操作前后状态的单张卡牌快照。
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
 * @brief 描述一个可逆操作的撤销记录。
 *
 * 简化假设：每个操作最多涉及两张卡牌。
 * - firstXXX 通常指向离开/进入备用堆的栈顶卡牌。
 * - secondXXX 代表移动的卡牌（牌区或备用堆）。
 * 当操作只涉及一张卡牌时，将 canUseSecond 设置为 false。
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
 * @brief 撤销管理器维护一个 UndoRecord 实例的栈。
 *
 * 职责：
 * - 由控制器拥有，但不依赖于控制器 API。
 * - 轻量级且易于测试。
 * - 仅存储 UndoRecord 结构体，而不拥有模型。
 */
class UndoManager
{
public:
    UndoManager() = default;
    ~UndoManager() = default;

    /// 将一条记录压入栈。
    void pushRecord(const UndoRecord& record);

    /// 弹出最新记录；如果栈为空则返回 false。
    bool popRecord(UndoRecord& outRecord);

    /// 清除所有记录。
    void clear();

    /// 是否至少有一条撤销条目。
    bool hasRecord() const;

private:
    std::vector<UndoRecord> _records;
};



