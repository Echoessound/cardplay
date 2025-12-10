#include "services/GameModelFromLevelGenerator.h"

#include "configs/models/LevelConfig.h"
#include "models/GameModel.h"
#include "models/CardEnums.h"

using namespace cocos2d;

bool GameModelFromLevelGenerator::generateGameModel(const LevelConfig& levelConfig, GameModel& gameModel)
{
    gameModel.cards.clear();
    gameModel.currentStackTopCardId = -1;
    gameModel.stackBasePosition = Vec2::ZERO;

    // 首先构建牌区卡牌。
    for (const auto& cfg : levelConfig.playfieldCards)
    {
        CardModel model;
        model.cardId = cfg.cardId;
        model.faceType = static_cast<CardFaceType>(cfg.cardFace);
        model.suitType = static_cast<CardSuitType>(cfg.cardSuit);
        model.position = cfg.pos;
        model.inPlayfield = true;
        model.inStack = false;
        model.isTopOfStack = false;
        model.isFaceUp = true;
        gameModel.cards.push_back(model);
    }

    // 构建备用堆卡牌；配置中较晚的条目更接近栈顶。
    for (size_t i = 0; i < levelConfig.stackCards.size(); ++i)
    {
        const auto& cfg = levelConfig.stackCards[i];
        CardModel model;
        model.cardId = cfg.cardId;
        model.faceType = static_cast<CardFaceType>(cfg.cardFace);
        model.suitType = static_cast<CardSuitType>(cfg.cardSuit);
        model.position = cfg.pos;
        model.inPlayfield = false;
        model.inStack = true;
        model.isFaceUp = true;
        model.isTopOfStack = (i == levelConfig.stackCards.size() - 1);
        if (model.isTopOfStack)
        {
            gameModel.currentStackTopCardId = model.cardId;
        }
        gameModel.cards.push_back(model);
    }

    // 使用第一个备用堆卡牌位置作为视觉栈基准位置（初始底部卡牌）。
    if (!levelConfig.stackCards.empty())
    {
        const auto& firstCfg = levelConfig.stackCards.front();
        gameModel.stackBasePosition = firstCfg.pos;
    }

    return true;
}



