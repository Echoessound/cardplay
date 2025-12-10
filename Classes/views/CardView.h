#pragma once

#include "cocos2d.h"
#include "models/CardModel.h"

/**
 * @brief Lightweight card view that renders a single card sprite and reports clicks.
 */
class CardView : public cocos2d::Node
{
public:
    CREATE_FUNC(CardView);

    CardView();
    virtual ~CardView();

    bool init() override;

    /// Logical card identifier provided by controller/model.
    int cardId;

    /// Assign background sprite (sample implementation reuses a shared texture).
    void setCardSprite(cocos2d::Sprite* sprite);

    /// Register click callback.
    void setClickCallback(const std::function<void(int)>& cb);

    /// Apply CardModel to render rank/suit overlays.
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



