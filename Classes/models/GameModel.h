#pragma once

#include <vector>
#include "cocos2d.h"
#include "models/CardModel.h"

/**
 * @brief 完整游戏会话的运行时数据模型。
 *
 * 仅存储状态；无复杂的业务逻辑。
 * 支持简单的序列化以用于保存/加载流程。
 */
class GameModel
{
public:
    GameModel();
    ~GameModel();

    /// 游戏中当前所有卡牌（牌区 + 备用堆）。
    std::vector<CardModel> cards;

    /// 当前栈顶卡牌 ID，如果没有则为 -1。
    int currentStackTopCardId;

    /// 备用堆的固定基准位置（例如来自关卡配置的视觉托盘槽）。
    cocos2d::Vec2 stackBasePosition;

    /// 将所有数据序列化为 ValueMap。
    cocos2d::ValueMap toValueMap() const;

    /// 从 ValueMap 反序列化。
    void fromValueMap(const cocos2d::ValueMap& map);

    /// 通过 ID 查找卡牌模型，如果未找到则返回 nullptr。
    CardModel* findCard(int cardId);
    const CardModel* findCard(int cardId) const;
};



