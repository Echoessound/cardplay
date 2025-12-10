#include "views/CardView.h"

#include "utils/CardTextureHelper.h"

USING_NS_CC;

CardView::CardView()
    : cardId(-1)
    , _sprite(nullptr)
    , _faceType(CardFaceType::CFT_NONE)
    , _suitType(CardSuitType::CST_NONE)
    , _isFaceUp(true)
{
}

CardView::~CardView() = default;

bool CardView::init()
{
    if (!Node::init())
    {
        return false;
    }

    setupTouchListener();
    return true;
}

void CardView::setCardSprite(Sprite* sprite)
{
    if (_sprite)
    {
        removeChild(_sprite);
        _sprite = nullptr;
    }
    _sprite = sprite;
    if (_sprite)
    {
        addChild(_sprite);
    }

    drawCardFaces();
}

void CardView::setClickCallback(const std::function<void(int)>& cb)
{
    _clickCallback = cb;
}

void CardView::applyModel(const CardModel& model)
{
    cardId = model.cardId;
    _faceType = model.faceType;
    _suitType = model.suitType;
    _isFaceUp = model.isFaceUp;
    drawCardFaces();
}

void CardView::setupTouchListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        if (!_sprite)
        {
            return false;
        }
        Vec2 local = convertToNodeSpace(touch->getLocation());
        Rect bounds = _sprite->getBoundingBox();
        if (bounds.containsPoint(local))
        {
            return true;
        }
        return false;
    };

    listener->onTouchEnded = [this](Touch* touch, Event* event) {
        if (_clickCallback)
        {
            _clickCallback(cardId);
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void CardView::drawCardFaces()
{
    if (!_sprite)
    {
        return;
    }

    _sprite->removeAllChildren();

     if (!_isFaceUp)
    {
        return;
    }

    const Size size = _sprite->getContentSize();
    const float margin = 20.0f;

    // 中心大号点数。
    const std::string bigTexture = CardTextureHelper::getBigNumberTexture(_faceType, _suitType);
    if (auto centerRank = Sprite::create(bigTexture))
    {
        centerRank->setPosition(Vec2(size.width * 0.5f, size.height * 0.55f));
        _sprite->addChild(centerRank);
    }

    // 角落的小号点数纹理。
    const std::string smallTexture = CardTextureHelper::getSmallNumberTexture(_faceType, _suitType);
    Sprite* smallTop = nullptr;
    if (auto spriteTop = Sprite::create(smallTexture))
    {
        spriteTop->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        spriteTop->setPosition(Vec2(margin, size.height - margin));
        _sprite->addChild(spriteTop);
        smallTop = spriteTop;
    }

  

    // 靠近角落的花色图标。
    const std::string suitTexture = CardTextureHelper::getSuitTexture(_suitType);
    float topSuitY = size.height - margin * 2.0f;
    if (smallTop)
    {
        topSuitY = smallTop->getPositionY() - smallTop->getContentSize().height - 5.0f;
    }

    if (auto suitTop = Sprite::create(suitTexture))
    {
        suitTop->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        suitTop->setPosition(Vec2(margin, topSuitY));
        _sprite->addChild(suitTop);
    }

    
}





