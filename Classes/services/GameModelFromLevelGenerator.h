#pragma once

class LevelConfig;
class GameModel;

/**
 * @brief 从静态 LevelConfig 构建运行时 GameModel 的服务。
 *
 * 无状态：仅修改传入的 GameModel。
 */
class GameModelFromLevelGenerator
{
public:
    GameModelFromLevelGenerator() = default;
    ~GameModelFromLevelGenerator() = default;

    /**
     * @brief 根据 LevelConfig 填充 GameModel 条目。
     * @param levelConfig 静态配置数据。
     * @param gameModel   要修改的运行时模型。
     * @return 如果生成成功则返回 true。
     */
    bool generateGameModel(const LevelConfig& levelConfig, GameModel& gameModel);
};



