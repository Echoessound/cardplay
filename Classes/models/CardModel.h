#pragma once

#include "cocos2d.h"
#include "models/CardEnums.h"

/**
 * @brief 单张卡牌的运行时数据模型（无复杂逻辑）。
 */
class CardModel
{
public:
    int cardId;              ///< 逻辑唯一标识符（映射到视图/撤销）。
    CardFaceType faceType;   ///< 点数。
    CardSuitType suitType;   ///< 花色。

    bool isFaceUp;           ///< 是否正面朝上。
    bool inPlayfield;        ///< 是否在牌区。
    bool inStack;            ///< 是否在备用堆。
    bool isTopOfStack;       ///< 是否当前为栈顶。

    cocos2d::Vec2 position;  ///< 当前世界坐标（用于持久化）。

    CardModel();

    /** 序列化为 ValueMap 用于保存数据。 */
    cocos2d::ValueMap toValueMap() const;

    /** 从 ValueMap 反序列化以恢复。 */
    void fromValueMap(const cocos2d::ValueMap& map);
};


