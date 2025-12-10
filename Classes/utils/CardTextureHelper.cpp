#include "utils/CardTextureHelper.h"

namespace
{
    std::string faceToString(CardFaceType face)
    {
        switch (face)
        {
        case CardFaceType::CFT_ACE:
            return "A";
        case CardFaceType::CFT_TWO:
            return "2";
        case CardFaceType::CFT_THREE:
            return "3";
        case CardFaceType::CFT_FOUR:
            return "4";
        case CardFaceType::CFT_FIVE:
            return "5";
        case CardFaceType::CFT_SIX:
            return "6";
        case CardFaceType::CFT_SEVEN:
            return "7";
        case CardFaceType::CFT_EIGHT:
            return "8";
        case CardFaceType::CFT_NINE:
            return "9";
        case CardFaceType::CFT_TEN:
            return "10";
        case CardFaceType::CFT_JACK:
            return "J";
        case CardFaceType::CFT_QUEEN:
            return "Q";
        case CardFaceType::CFT_KING:
            return "K";
        default:
            return "A";
        }
    }

    std::string buildNumberTexturePath(bool small, CardFaceType face, CardSuitType suit)
    {
        const std::string sizePrefix = small ? "small" : "big";
        const std::string color = CardTextureHelper::isRedSuit(suit) ? "red" : "black";
        const std::string faceStr = faceToString(face);
        return "res/number/" + sizePrefix + "_" + color + "_" + faceStr + ".png";
    }
}

bool CardTextureHelper::isRedSuit(CardSuitType suit)
{
    return suit == CardSuitType::CST_HEARTS || suit == CardSuitType::CST_DIAMONDS;
}

std::string CardTextureHelper::getSuitTexture(CardSuitType suit)
{
    switch (suit)
    {
    case CardSuitType::CST_CLUBS:
        return "res/suits/club.png";
    case CardSuitType::CST_DIAMONDS:
        return "res/suits/diamond.png";
    case CardSuitType::CST_HEARTS:
        return "res/suits/heart.png";
    case CardSuitType::CST_SPADES:
        return "res/suits/spade.png";
    default:
        return "res/suits/spade.png";
    }
}

std::string CardTextureHelper::getSmallNumberTexture(CardFaceType face, CardSuitType suit)
{
    return buildNumberTexturePath(true, face, suit);
}

std::string CardTextureHelper::getBigNumberTexture(CardFaceType face, CardSuitType suit)
{
    return buildNumberTexturePath(false, face, suit);
}



