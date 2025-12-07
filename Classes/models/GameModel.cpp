#include "models/GameModel.h"

using namespace cocos2d;

GameModel::GameModel()
    : currentStackTopCardId(-1)
    , stackBasePosition(Vec2::ZERO)
{
}

GameModel::~GameModel() = default;

ValueMap GameModel::toValueMap() const
{
    ValueMap root;
    root["currentStackTopCardId"] = currentStackTopCardId;
    root["stackBasePosX"] = stackBasePosition.x;
    root["stackBasePosY"] = stackBasePosition.y;

    ValueVector cardArray;
    cardArray.reserve(cards.size());
    for (const auto& c : cards)
    {
        cardArray.push_back(Value(c.toValueMap()));
    }
    root["cards"] = cardArray;
    return root;
}

void GameModel::fromValueMap(const ValueMap& map)
{
    cards.clear();
    currentStackTopCardId = map.at("currentStackTopCardId").asInt();
    stackBasePosition.x = map.at("stackBasePosX").asFloat();
    stackBasePosition.y = map.at("stackBasePosY").asFloat();

    const auto& arr = map.at("cards").asValueVector();
    cards.reserve(arr.size());
    for (const auto& v : arr)
    {
        CardModel model;
        model.fromValueMap(v.asValueMap());
        cards.push_back(model);
    }
}

CardModel* GameModel::findCard(int cardId)
{
    for (auto& c : cards)
    {
        if (c.cardId == cardId)
        {
            return &c;
        }
    }
    return nullptr;
}

const CardModel* GameModel::findCard(int cardId) const
{
    for (const auto& c : cards)
    {
        if (c.cardId == cardId)
        {
            return &c;
        }
    }
    return nullptr;
}



