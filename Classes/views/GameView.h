#pragma once

#include "cocos2d.h"
#include <functional>
#include <unordered_map>

class GameModel;
class CardView;

/**
 * @brief Primary gameplay view that lays out playfield, stack areas and UI chrome.
 *
 * Only responsible for rendering and relaying input via callbacks;
 * no gameplay rules should live here.
 */
class GameView : public cocos2d::Node
{
public:
    CREATE_FUNC(GameView);

    GameView() noexcept;
    virtual ~GameView();

    /**
     * @brief Initialize layout with a readonly GameModel snapshot.
     * @param model Data model that describes current card states.
     * @return True when layout succeeds.
     */
    bool initWithModel(const GameModel* model);

    /// Playfield card click callback (cardId passed as parameter).
    std::function<void(int)> onPlayfieldCardClicked;

    /// Stack card click callback (cardId passed as parameter).
    std::function<void(int)> onStackCardClicked;

    /// Undo button callback.
    std::function<void()> onUndoClicked;

    /**
     * @brief Play animation that moves a card onto the current stack top.
     * @param fromCardId Source card ID.
     * @param toCardId   Destination card ID (current stack top).
     */
    void playMoveToStackTopAnimation(int fromCardId, int toCardId);

    /**
     * @brief Play single undo animation, moving card back to target position.
     * @param movingCardId Card that should move.
     * @param targetPos    Destination position.
     */
    void playUndoMoveAnimation(int movingCardId, const cocos2d::Vec2& targetPos);

protected:
    bool init() override;

private:
    const GameModel* _gameModel; ///< Readonly reference, lifecycle owned by controller.

    std::unordered_map<int, CardView*> _cardViews; ///< cardId -> CardView

    cocos2d::Node* _playfieldLayer;
    cocos2d::Node* _stackLayer;
    cocos2d::Sprite* _backgroundSprite; ///< Optional background image

    void createUndoButton();
    void createBackground();
    void refreshAllCards();
    void createOrUpdateCardView(const GameModel& model);
};



