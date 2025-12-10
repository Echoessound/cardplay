#include "views/GameView.h"

#include "models/GameModel.h"
#include "models/CardModel.h"
#include "views/CardView.h"

USING_NS_CC;

namespace
{
    // 简化的布局常量。
    const float kPlayfieldHeight = 1500.0f;
    const float kStackHeight = 580.0f;
}

GameView::GameView() noexcept
    : _gameModel(nullptr)
    , _playfieldLayer(nullptr)
    , _stackLayer(nullptr)
    , _backgroundSprite(nullptr)
{
}

GameView::~GameView() = default;

bool GameView::init()
{
    if (!Node::init())
    {
        return false;
    }

    // 创建背景
    createBackground();

    // 为牌区和备用堆区域创建容器节点。
    const auto size = Director::getInstance()->getVisibleSize();

    _playfieldLayer = Node::create();
    _playfieldLayer->setContentSize(Size(size.width, kPlayfieldHeight));
    _playfieldLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _playfieldLayer->setPosition(Vec2(size.width * 0.5f, size.height));
    addChild(_playfieldLayer);

    _stackLayer = Node::create();
    _stackLayer->setContentSize(Size(size.width, kStackHeight));
    _stackLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _stackLayer->setPosition(Vec2(size.width * 0.5f, 0.0f));
    addChild(_stackLayer);

    createUndoButton();

    return true;
}

bool GameView::initWithModel(const GameModel* model)
{
    _gameModel = model;
    refreshAllCards();
    return true;
}

void GameView::createBackground()
{
    // 选项1：使用背景图像
    // 替换 "res/background.png" 
    _backgroundSprite = Sprite::create("res/background.jpg");
    if (_backgroundSprite)
     {
        const auto size = Director::getInstance()->getVisibleSize();
        _backgroundSprite->setPosition(Vec2(size.width * 0.5f, size.height * 0.5f));
        _backgroundSprite->setContentSize(size);
        addChild(_backgroundSprite, -1); 
    }

    // 选项 2：使用纯色背景
    //const auto size = Director::getInstance()->getVisibleSize();
    //auto layerColor = LayerColor::create(Color4B(34, 139, 34, 255), size.width, size.height);
    //if (layerColor)
    //{
        //layerColor->setPosition(Vec2::ZERO);
        //addChild(layerColor, -1); // Add to back layer
    //}
}

void GameView::createUndoButton()
{
    auto size = Director::getInstance()->getVisibleSize();
    auto label = Label::createWithTTF("Undo", "fonts/Marker Felt.ttf", 40);
    if (!label)
    {
        return;
    }
    auto item = MenuItemLabel::create(label, [this](Ref*) {
        if (onUndoClicked)
        {
            onUndoClicked();
        }
    });
    auto menu = Menu::create(item, nullptr);
    menu->setPosition(Vec2(size.width - 80.0f, size.height - 80.0f));
    addChild(menu, 10);
}

void GameView::refreshAllCards()
{
    if (!_gameModel)
    {
        return;
    }

    // 清理现有节点。
    _playfieldLayer->removeAllChildren();
    _stackLayer->removeAllChildren();
    _cardViews.clear();

    createOrUpdateCardView(*_gameModel);
}

void GameView::createOrUpdateCardView(const GameModel& model)
{
    for (const auto& c : model.cards)
    {
        auto view = CardView::create();
        view->cardId = c.cardId;

        auto sprite = Sprite::create("res/card_general.png");
        view->setCardSprite(sprite);
        view->applyModel(c);

        // 确定使用哪个容器
        Node* parent = c.inPlayfield ? _playfieldLayer : _stackLayer;
        parent->addChild(view);

        view->setPosition(c.position);

        view->setClickCallback([this, &c](int cardId) {
            if (!_gameModel)
            {
                return;
            }
            // 根据模型状态路由点击回调。
            const CardModel* modelCard = _gameModel->findCard(cardId);
            if (!modelCard)
            {
                return;
            }
            if (modelCard->inPlayfield)
            {
                if (onPlayfieldCardClicked)
                {
                    onPlayfieldCardClicked(cardId);
                }
            }
            else if (modelCard->inStack)
            {
                if (onStackCardClicked)
                {
                    onStackCardClicked(cardId);
                }
            }
        });

        _cardViews[c.cardId] = view;
    }
}

void GameView::playMoveToStackTopAnimation(int fromCardId, int toCardId)
{
    auto fromIt = _cardViews.find(fromCardId);
    auto toIt = _cardViews.find(toCardId);
    if (fromIt == _cardViews.end() || toIt == _cardViews.end())
    {
        return;
    }

    auto fromView = fromIt->second;
    auto toView = toIt->second;

    // 移动到当前栈顶卡牌的视觉位置。
    Vec2 targetPos = toView->getPosition();

    // 如果两张卡牌位于不同的父层（牌区 vs 备用堆），
    // 将移动的卡牌重新父化到目标层，同时保持其视觉世界位置，
    // 以便它可以用正确的 y 基线覆盖底部卡牌。
    Node* targetParent = toView->getParent();
    Node* sourceParent = fromView->getParent();
    if (targetParent && sourceParent && targetParent != sourceParent)
    {
        // 保留当前世界位置以避免重新父化时的跳跃。
        Vec2 worldCurrent = sourceParent->convertToWorldSpace(fromView->getPosition());
        Vec2 worldTarget = targetParent->convertToWorldSpace(targetPos);

        fromView->retain(); // 避免在移除/添加时意外删除
        fromView->removeFromParent();
        targetParent->addChild(fromView);
        fromView->release();

        fromView->setPosition(targetParent->convertToNodeSpace(worldCurrent));
        targetPos = targetParent->convertToNodeSpace(worldTarget);
    }

    // 确保新选中的卡牌在视觉上保持在之前的栈顶卡牌之上。
    fromView->setLocalZOrder(toView->getLocalZOrder() + 1);

    // 简单的移动动画，不使用 lambda 以避免编译器问题。
    fromView->runAction(MoveTo::create(0.2f, targetPos));
}

void GameView::playUndoMoveAnimation(int movingCardId, const Vec2& targetPos)
{
    auto it = _cardViews.find(movingCardId);
    if (it == _cardViews.end())
    {
        return;
    }
    auto view = it->second;

    // 如果卡牌需要跨层移回（例如，备用堆 -> 牌区），
    // 根据模型标志重新父化，以便 y 基线与目标匹配。
    Vec2 adjustedTarget = targetPos;
    Node* currentParent = view->getParent();
    Node* desiredParent = currentParent;
    if (_gameModel)
    {
        if (const CardModel* m = _gameModel->findCard(movingCardId))
        {
            desiredParent = m->inPlayfield ? _playfieldLayer : _stackLayer;
        }
    }

    if (desiredParent && currentParent && desiredParent != currentParent)
    {
        Vec2 worldCurrent = currentParent->convertToWorldSpace(view->getPosition());
        Vec2 worldTarget = desiredParent->convertToWorldSpace(adjustedTarget);

        view->retain();
        view->removeFromParent();
        desiredParent->addChild(view);
        view->release();

        view->setPosition(desiredParent->convertToNodeSpace(worldCurrent));
        adjustedTarget = desiredParent->convertToNodeSpace(worldTarget);
    }

    view->runAction(MoveTo::create(0.2f, adjustedTarget));
}



