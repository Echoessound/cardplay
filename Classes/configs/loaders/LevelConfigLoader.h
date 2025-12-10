#pragma once

#include <string>
#include "configs/models/LevelConfig.h"

/**
 * @brief Loader that reads JSON level files into LevelConfig structs.
 *        Stateless and only lives inside configs/ layer.
 */
class LevelConfigLoader
{
public:
    /**
     * @brief Load configuration by level ID.
     * @param levelId  Level identifier (1-based).
     * @param outConfig Output parameter filled with result.
     * @return Whether loading succeeded.
     */
    static bool loadLevelConfig(int levelId, LevelConfig& outConfig);

private:
    static bool loadFromFile(const std::string& filePath, LevelConfig& outConfig);
};


