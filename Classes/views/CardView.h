#pragma once

#include "cocos2d.h"
#include "models/CardModel.h"

/**
 * @brief 轻量级卡牌视图，渲染单张卡牌精灵并报告点击。
 */
class CardView : public cocos2d::Node
{
public:
    CREATE_FUNC(CardView);

    CardView();
    virtual ~CardView();

    bool init() override;

    /// 由控制器/模型提供的逻辑卡牌标识符。
    int cardId;

    /// 分配背景精灵（示例实现重用共享纹理）。
    void setCardSprite(cocos2d::Sprite* sprite);

    /// 注册点击回调。
    void setClickCallback(const std::function<void(int)>& cb);

    /// 应用 CardModel 以渲染点数/花色叠加层。
    void applyModel(const CardModel& model);

private:
    cocos2d::Sprite* _sprite;
    std::function<void(int)> _clickCallback;
    CardFaceType _faceType;
    CardSuitType _suitType;
    bool _isFaceUp;

    void drawCardFaces();

    void setupTouchListener();
};



