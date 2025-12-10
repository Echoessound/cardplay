#pragma once

#include "cocos2d.h"

class GameModel;
class GameView;
class UndoManager;
class GameModelFromLevelGenerator;

/**
 * @brief 主要的游戏控制器，将模型和视图连接在一起。
 *
 * 职责：
 * - 从请求的关卡配置构建 GameModel。
 * - 处理从视图转发来的用户输入事件。
 * - 驱动 UndoManager 以持久化和恢复操作。
 * - 请求 GameView 播放所需的动画。
 */
class GameController
{
public:
    GameController();
    ~GameController();

    /**
     * @brief 启动特定关卡。
     * @param levelId 关卡标识符。
     * @param parent  承载 GameView 的父节点（大多数情况下是 Scene）。
     * @return 关卡启动是否成功。
     */
    bool startGame(int levelId, cocos2d::Node* parent);

    /**
     * @brief 牌区卡牌点击的入口点。
     * @param cardId 被点击的逻辑卡牌标识符。
     * @return 如果交互有效且已处理，返回 true。
     */
    bool handlePlayfieldCardClick(int cardId);

    /**
     * @brief 备用堆卡牌点击的入口点（托盘/备用堆）。
     *        用于"备用堆交换"功能。
     * @param cardId 被点击的逻辑卡牌标识符。
     * @return 如果交互有效且已处理，返回 true。
     */
    bool handleStackCardClick(int cardId);

    /**
     * @brief 处理撤销按钮。
     *        尝试从 UndoManager 弹出一条记录并回滚。
     */
    void handleUndo();

    /** 获取用于检查的 GameModel 的只读指针。 */
    const GameModel* getGameModel() const
    {
        return _gameModel;
    }

private:
    GameModel* _gameModel;                 ///< 运行时游戏数据
    GameView* _gameView;                   ///< 游戏视图根节点
    UndoManager* _undoManager;             ///< 管理撤销记录

    GameModelFromLevelGenerator* _generator; ///< 将 LevelConfig 转换为 GameModel 的服务

    bool initGameModel(int levelId);
    bool initGameView(cocos2d::Node* parent);
    void bindViewCallbacks();
};



