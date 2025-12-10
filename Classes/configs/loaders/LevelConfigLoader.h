#pragma once

#include <string>
#include "configs/models/LevelConfig.h"

/**
 * @brief 将 JSON 关卡文件读取到 LevelConfig 结构体的加载器。
 *        无状态，仅存在于 configs/ 层。
 */
class LevelConfigLoader
{
public:
    /**
     * @brief 根据关卡 ID 加载配置。
     * @param levelId  关卡标识符（从1开始）。
     * @param outConfig 输出参数，填充结果。
     * @return 加载是否成功。
     */
    static bool loadLevelConfig(int levelId, LevelConfig& outConfig);

private:
    static bool loadFromFile(const std::string& filePath, LevelConfig& outConfig);
};


