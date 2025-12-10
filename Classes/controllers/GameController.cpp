#include "controllers/GameController.h"

#include "configs/loaders/LevelConfigLoader.h"
#include "configs/models/LevelConfig.h"
#include "models/GameModel.h"
#include "models/CardModel.h"
#include "services/GameModelFromLevelGenerator.h"
#include "views/GameView.h"
#include "managers/UndoManager.h"

USING_NS_CC;

namespace
{
    CardStateSnapshot makeSnapshot(const CardModel& m)
    {
        CardStateSnapshot s;
        s.cardId = m.cardId;
        s.position = m.position;
        s.inPlayfield = m.inPlayfield;
        s.inStack = m.inStack;
        s.isTopOfStack = m.isTopOfStack;
        s.isFaceUp = m.isFaceUp;
        return s;
    }
}

GameController::GameController()
    : _gameModel(nullptr)
    , _gameView(nullptr)
    , _undoManager(nullptr)
    , _generator(nullptr)
{
}

GameController::~GameController()
{
    CC_SAFE_DELETE(_gameModel);
    CC_SAFE_DELETE(_undoManager);
    CC_SAFE_DELETE(_generator);
}

bool GameController::startGame(int levelId, Node* parent)
{
    if (!initGameModel(levelId))
    {
        return false;
    }
    if (!initGameView(parent))
    {
        return false;
    }
    bindViewCallbacks();
    return true;
}

bool GameController::initGameModel(int levelId)
{
    CC_SAFE_DELETE(_gameModel);
    _gameModel = new (std::nothrow) GameModel();
    if (!_gameModel)
    {
        return false;
    }

    CC_SAFE_DELETE(_undoManager);
    _undoManager = new (std::nothrow) UndoManager();
    if (!_undoManager)
    {
        return false;
    }

    CC_SAFE_DELETE(_generator);
    _generator = new (std::nothrow) GameModelFromLevelGenerator();
    if (!_generator)
    {
        return false;
    }

    LevelConfig levelConfig;
    if (!LevelConfigLoader::loadLevelConfig(levelId, levelConfig))
    {
        CCLOG("GameController: failed to load level %d", levelId);
        return false;
    }

    if (!_generator->generateGameModel(levelConfig, *_gameModel))
    {
        CCLOG("GameController: failed to generate GameModel");
        return false;
    }

    return true;
}

bool GameController::initGameView(Node* parent)
{
    if (!parent || !_gameModel)
    {
        return false;
    }

    _gameView = GameView::create();
    if (!_gameView)
    {
        return false;
    }

    _gameView->initWithModel(_gameModel);
    parent->addChild(_gameView);

    return true;
}

void GameController::bindViewCallbacks()
{
    if (!_gameView)
    {
        return;
    }

    _gameView->onPlayfieldCardClicked = [this](int cardId) {
        handlePlayfieldCardClick(cardId);
    };

    _gameView->onStackCardClicked = [this](int cardId) {
        handleStackCardClick(cardId);
    };

    _gameView->onUndoClicked = [this]() {
        handleUndo();
    };
}

bool GameController::handlePlayfieldCardClick(int cardId)
{
    if (!_gameModel)
    {
        return false;
    }

    CardModel* clicked = _gameModel->findCard(cardId);
    if (!clicked || !clicked->inPlayfield || !clicked->isFaceUp)
    {
        return false;
    }

    CardModel* top = _gameModel->findCard(_gameModel->currentStackTopCardId);
    if (!top)
    {
        return false;
    }

    int diff = static_cast<int>(clicked->faceType) - static_cast<int>(top->faceType);
    if (diff != 1 && diff != -1)
    {
        // 规则不满足：点数必须相差恰好为 1
        return false;
    }

    // 存储操作前的状态
    UndoRecord record;
    record.firstBefore = makeSnapshot(*top);
    record.secondBefore = makeSnapshot(*clicked);
    record.canUseSecond = true;

    // 将被点击的牌区卡牌移动到当前栈顶卡牌的位置
    clicked->inPlayfield = false;
    clicked->inStack = true;
    clicked->isTopOfStack = true;
    clicked->position = top->position;

    top->isTopOfStack = false;

    _gameModel->currentStackTopCardId = clicked->cardId;

    // 存储操作后的状态
    record.firstAfter = makeSnapshot(*top);
    record.secondAfter = makeSnapshot(*clicked);

    if (_undoManager)
    {
        _undoManager->pushRecord(record);
    }

    if (_gameView)
    {
        _gameView->playMoveToStackTopAnimation(clicked->cardId, top->cardId);
    }

    return true;
}

bool GameController::handleStackCardClick(int cardId)
{
    if (!_gameModel)
    {
        return false;
    }

    CardModel* clicked = _gameModel->findCard(cardId);
    if (!clicked || !clicked->inStack)
    {
        return false;
    }

    if (clicked->isTopOfStack)
    {
        // 忽略对当前栈顶卡牌的点击
        return false;
    }

    CardModel* top = _gameModel->findCard(_gameModel->currentStackTopCardId);
    if (!top)
    {
        return false;
    }

    // 存储操作前的状态
    UndoRecord record;
    record.firstBefore = makeSnapshot(*top);
    record.secondBefore = makeSnapshot(*clicked);
    record.canUseSecond = true;

    // 交换位置和栈顶标志以执行替换
    Vec2 tempPos = top->position;
    top->position = clicked->position;
    clicked->position = tempPos;

    top->isTopOfStack = false;
    clicked->isTopOfStack = true;
    _gameModel->currentStackTopCardId = clicked->cardId;

    // 存储操作后的状态
    record.firstAfter = makeSnapshot(*top);
    record.secondAfter = makeSnapshot(*clicked);

    if (_undoManager)
    {
        _undoManager->pushRecord(record);
    }

    if (_gameView)
    {
        _gameView->playMoveToStackTopAnimation(clicked->cardId, top->cardId);
    }

    return true;
}

void GameController::handleUndo()
{
    if (!_gameModel || !_undoManager)
    {
        return;
    }

    UndoRecord record;
    if (!_undoManager->popRecord(record))
    {
        return;
    }

    // 使用保存的快照恢复模型
    if (record.firstBefore.cardId >= 0)
    {
        if (auto* m = _gameModel->findCard(record.firstBefore.cardId))
        {
            m->position = record.firstBefore.position;
            m->inPlayfield = record.firstBefore.inPlayfield;
            m->inStack = record.firstBefore.inStack;
            m->isTopOfStack = record.firstBefore.isTopOfStack;
            m->isFaceUp = record.firstBefore.isFaceUp;
            if (m->isTopOfStack)
            {
                _gameModel->currentStackTopCardId = m->cardId;
            }
        }
    }

    if (record.canUseSecond && record.secondBefore.cardId >= 0)
    {
        if (auto* m = _gameModel->findCard(record.secondBefore.cardId))
        {
            m->position = record.secondBefore.position;
            m->inPlayfield = record.secondBefore.inPlayfield;
            m->inStack = record.secondBefore.inStack;
            m->isTopOfStack = record.secondBefore.isTopOfStack;
            m->isFaceUp = record.secondBefore.isFaceUp;
            if (m->isTopOfStack)
            {
                _gameModel->currentStackTopCardId = m->cardId;
            }
        }
    }

    // 简单动画：将最近移动的卡牌移回
    if (_gameView && record.canUseSecond)
    {
        _gameView->playUndoMoveAnimation(
            record.secondAfter.cardId,
            record.secondBefore.position);
    }
}



