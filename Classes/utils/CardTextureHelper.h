#pragma once

#include <string>
#include "models/CardEnums.h"

/**
 * @brief Helper functions that return texture paths for card ranks and suits.
 */
namespace CardTextureHelper
{
    /// Whether suit should be rendered as red.
    bool isRedSuit(CardSuitType suit);

    /// Texture path for the suit icon.
    std::string getSuitTexture(CardSuitType suit);

    /// Texture path for the small rank print (corner).
    std::string getSmallNumberTexture(CardFaceType face, CardSuitType suit);

    /// Texture path for the large rank print (center).
    std::string getBigNumberTexture(CardFaceType face, CardSuitType suit);
}



