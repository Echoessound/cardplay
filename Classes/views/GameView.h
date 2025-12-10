#pragma once

#include "cocos2d.h"
#include <functional>
#include <unordered_map>

class GameModel;
class CardView;

/**
 * @brief 主要的游戏视图，布局牌区、备用堆区域和 UI 界面。
 *
 * 仅负责渲染和通过回调中继输入；
 * 不应包含游戏规则逻辑。
 */
class GameView : public cocos2d::Node
{
public:
    CREATE_FUNC(GameView);

    GameView() noexcept;
    virtual ~GameView();

    /**
     * @brief 使用只读 GameModel 快照初始化布局。
     * @param model 描述当前卡牌状态的数据模型。
     * @return 布局成功时返回 true。
     */
    bool initWithModel(const GameModel* model);

    /// 牌区卡牌点击回调（cardId 作为参数传递）。
    std::function<void(int)> onPlayfieldCardClicked;

    /// 备用堆卡牌点击回调（cardId 作为参数传递）。
    std::function<void(int)> onStackCardClicked;

    /// 撤销按钮回调。
    std::function<void()> onUndoClicked;

    /**
     * @brief 播放将卡牌移动到当前栈顶的动画。
     * @param fromCardId 源卡牌 ID。
     * @param toCardId   目标卡牌 ID（当前栈顶）。
     */
    void playMoveToStackTopAnimation(int fromCardId, int toCardId);

    /**
     * @brief 播放单个撤销动画，将卡牌移回目标位置。
     * @param movingCardId 应移动的卡牌。
     * @param targetPos    目标位置。
     */
    void playUndoMoveAnimation(int movingCardId, const cocos2d::Vec2& targetPos);

protected:
    bool init() override;

private:
    const GameModel* _gameModel; ///< 只读引用，生命周期由控制器拥有。

    std::unordered_map<int, CardView*> _cardViews; ///< cardId -> CardView

    cocos2d::Node* _playfieldLayer;
    cocos2d::Node* _stackLayer;
    cocos2d::Sprite* _backgroundSprite; ///< 可选的背景图像

    void createUndoButton();
    void createBackground();
    void refreshAllCards();
    void createOrUpdateCardView(const GameModel& model);
};



