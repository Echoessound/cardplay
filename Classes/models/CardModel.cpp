#include "models/CardModel.h"

using namespace cocos2d;

CardModel::CardModel()
    : cardId(-1)
    , faceType(CardFaceType::CFT_NONE)
    , suitType(CardSuitType::CST_NONE)
    , isFaceUp(true)
    , inPlayfield(false)
    , inStack(false)
    , isTopOfStack(false)
    , position(Vec2::ZERO)
{
}

ValueMap CardModel::toValueMap() const
{
    ValueMap map;
    map["cardId"] = cardId;
    map["faceType"] = static_cast<int>(faceType);
    map["suitType"] = static_cast<int>(suitType);
    map["isFaceUp"] = isFaceUp;
    map["inPlayfield"] = inPlayfield;
    map["inStack"] = inStack;
    map["isTopOfStack"] = isTopOfStack;
    map["posX"] = position.x;
    map["posY"] = position.y;
    return map;
}

void CardModel::fromValueMap(const ValueMap& map)
{
    cardId = map.at("cardId").asInt();
    faceType = static_cast<CardFaceType>(map.at("faceType").asInt());
    suitType = static_cast<CardSuitType>(map.at("suitType").asInt());
    isFaceUp = map.at("isFaceUp").asBool();
    inPlayfield = map.at("inPlayfield").asBool();
    inStack = map.at("inStack").asBool();
    isTopOfStack = map.at("isTopOfStack").asBool();
    const float x = map.at("posX").asFloat();
    const float y = map.at("posY").asFloat();
    position.set(x, y);
}



