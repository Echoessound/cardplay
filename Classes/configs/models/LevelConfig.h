#pragma once

#include "cocos2d.h"
#include <vector>

/**
 * @brief 关卡配置文件中的单张卡牌条目。
 *        仅包含位置、点数和花色信息。
 */
class LevelCardConfig
{
public:
    int cardId;          ///< 由加载器生成的逻辑唯一ID。
    int cardFace;        ///< CardFaceType 枚举值（点数）。
    int cardSuit;        ///< CardSuitType 枚举值（花色）。
    cocos2d::Vec2 pos;   ///< 场景中的初始位置。

    LevelCardConfig()
        : cardId(-1)
        , cardFace(-1)
        , cardSuit(-1)
        , pos(cocos2d::Vec2::ZERO)
    {
    }
};

/**
 * @brief 单个关卡的静态数据（由JSON文件定义）。
 */
class LevelConfig
{
public:
    /** 初始牌区的卡牌。 */
    std::vector<LevelCardConfig> playfieldCards;

    /** 初始备用堆的卡牌。 */
    std::vector<LevelCardConfig> stackCards;

    LevelConfig() = default;
};


