#pragma once

#include <string>
#include "models/CardEnums.h"

/**
 * @brief 返回卡牌点数和花色纹理路径的辅助函数。
 */
namespace CardTextureHelper
{
    /// 花色是否应渲染为红色。
    bool isRedSuit(CardSuitType suit);

    /// 花色图标的纹理路径。
    std::string getSuitTexture(CardSuitType suit);

    /// 小号点数纹理路径（角落）。
    std::string getSmallNumberTexture(CardFaceType face, CardSuitType suit);

    /// 大号点数纹理路径（中心）。
    std::string getBigNumberTexture(CardFaceType face, CardSuitType suit);
}



