#pragma once

class LevelConfig;
class GameModel;

/**
 * @brief Service that builds a runtime GameModel from a static LevelConfig.
 *
 * Stateless: only mutates the GameModel passed in.
 */
class GameModelFromLevelGenerator
{
public:
    GameModelFromLevelGenerator() = default;
    ~GameModelFromLevelGenerator() = default;

    /**
     * @brief Fill GameModel entries according to a LevelConfig.
     * @param levelConfig Static configuration payload.
     * @param gameModel   Runtime model to be mutated.
     * @return True if generation succeeded.
     */
    bool generateGameModel(const LevelConfig& levelConfig, GameModel& gameModel);
};



