#include "views/GameView.h"

#include "models/GameModel.h"
#include "models/CardModel.h"
#include "views/CardView.h"

USING_NS_CC;

namespace
{
    // Simplified layout constants.
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

    // Create background (optional - comment out if not needed)
    createBackground();

    // Create container nodes for playfield and stack areas.
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
    // Option 1: Use a background image
    // Replace "res/background.png" with your background image path
    // _backgroundSprite = Sprite::create("res/background.png");
    // if (_backgroundSprite)
    // {
    //     const auto size = Director::getInstance()->getVisibleSize();
    //     _backgroundSprite->setPosition(Vec2(size.width * 0.5f, size.height * 0.5f));
    //     _backgroundSprite->setContentSize(size);
    //     addChild(_backgroundSprite, -1); // Add to back layer
    // }

    // Option 2: Use a solid color background
    const auto size = Director::getInstance()->getVisibleSize();
    auto layerColor = LayerColor::create(Color4B(34, 139, 34, 255), size.width, size.height);
    if (layerColor)
    {
        layerColor->setPosition(Vec2::ZERO);
        addChild(layerColor, -1); // Add to back layer
    }
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

    // Clean existing nodes.
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

        // Determine which container to attach to.
        Node* parent = c.inPlayfield ? _playfieldLayer : _stackLayer;
        parent->addChild(view);

        view->setPosition(c.position);

        view->setClickCallback([this, &c](int cardId) {
            if (!_gameModel)
            {
                return;
            }
            // Route click callback based on model state.
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

    // Move onto the current stack-top card visual position.
    Vec2 targetPos = toView->getPosition();

    // If the two cards live under different parent layers (playfield vs stack),
    // reparent the moving card into the target layer while keeping its visual
    // world position, so it can cover the bottom card with correct y-baseline.
    Node* targetParent = toView->getParent();
    Node* sourceParent = fromView->getParent();
    if (targetParent && sourceParent && targetParent != sourceParent)
    {
        // Preserve current world position to avoid jumps during reparent.
        Vec2 worldCurrent = sourceParent->convertToWorldSpace(fromView->getPosition());
        Vec2 worldTarget = targetParent->convertToWorldSpace(targetPos);

        fromView->retain(); // avoid accidental deletion during remove/add
        fromView->removeFromParent();
        targetParent->addChild(fromView);
        fromView->release();

        fromView->setPosition(targetParent->convertToNodeSpace(worldCurrent));
        targetPos = targetParent->convertToNodeSpace(worldTarget);
    }

    // Ensure the newly selected card visually stays above the previous top card.
    fromView->setLocalZOrder(toView->getLocalZOrder() + 1);

    // Simple move animation without lambdas to avoid compiler issues.
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

    // If the card needs to move back across layers (e.g., stack -> playfield),
    // reparent based on model flags so the y-baseline matches the destination.
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



