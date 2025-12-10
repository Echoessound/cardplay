#include "configs/loaders/LevelConfigLoader.h"

#include "cocos2d.h"
#include "external/json/document.h"

using namespace cocos2d;

namespace
{
    std::string buildLevelFilePath(int levelId)
    {
        // Convention: level files live under Resources/res/level_x.json
        char buf[64];
        snprintf(buf, sizeof(buf), "res/level_%d.json", levelId);
        return std::string(buf);
    }

    void fillCardConfig(LevelCardConfig& cfg, int& nextId,
                        int cardFace, int cardSuit, const Vec2& pos)
    {
        cfg.cardId = nextId++;
        cfg.cardFace = cardFace;
        cfg.cardSuit = cardSuit;
        cfg.pos = pos;
    }
}

bool LevelConfigLoader::loadLevelConfig(int levelId, LevelConfig& outConfig)
{
    const std::string filePath = buildLevelFilePath(levelId);
    return loadFromFile(filePath, outConfig);
}

bool LevelConfigLoader::loadFromFile(const std::string& filePath, LevelConfig& outConfig)
{
    outConfig.playfieldCards.clear();
    outConfig.stackCards.clear();

    const std::string fileContent = FileUtils::getInstance()->getStringFromFile(filePath);
    if (fileContent.empty())
    {
        CCLOG("LevelConfigLoader: failed to read file: %s", filePath.c_str());
        return false;
    }

    rapidjson::Document doc;
    doc.Parse<0>(fileContent.c_str());
    if (doc.HasParseError() || !doc.IsObject())
    {
        CCLOG("LevelConfigLoader: json parse error: %s", filePath.c_str());
        return false;
    }

    int nextId = 0;

    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray())
    {
        const auto& arr = doc["Playfield"];
        outConfig.playfieldCards.reserve(arr.Size());
        for (rapidjson::SizeType i = 0; i < arr.Size(); ++i)
        {
            const auto& item = arr[i];
            if (!item.IsObject())
            {
                continue;
            }

            const int cardFace = item.HasMember("CardFace") ? item["CardFace"].GetInt() : -1;
            const int cardSuit = item.HasMember("CardSuit") ? item["CardSuit"].GetInt() : -1;

            Vec2 pos = Vec2::ZERO;
            if (item.HasMember("Position") && item["Position"].IsObject())
            {
                const auto& p = item["Position"];
                const float x = p.HasMember("x") ? p["x"].GetFloat() : 0.0f;
                const float y = p.HasMember("y") ? p["y"].GetFloat() : 0.0f;
                pos.set(x, y);
            }

            LevelCardConfig cfg;
            fillCardConfig(cfg, nextId, cardFace, cardSuit, pos);
            outConfig.playfieldCards.push_back(cfg);
        }
    }

    if (doc.HasMember("Stack") && doc["Stack"].IsArray())
    {
        const auto& arr = doc["Stack"];
        outConfig.stackCards.reserve(arr.Size());
        for (rapidjson::SizeType i = 0; i < arr.Size(); ++i)
        {
            const auto& item = arr[i];
            if (!item.IsObject())
            {
                continue;
            }

            const int cardFace = item.HasMember("CardFace") ? item["CardFace"].GetInt() : -1;
            const int cardSuit = item.HasMember("CardSuit") ? item["CardSuit"].GetInt() : -1;

            // Stack layout may still override positions; keep values optional.
            Vec2 pos = Vec2::ZERO;
            if (item.HasMember("Position") && item["Position"].IsObject())
            {
                const auto& p = item["Position"];
                const float x = p.HasMember("x") ? p["x"].GetFloat() : 0.0f;
                const float y = p.HasMember("y") ? p["y"].GetFloat() : 0.0f;
                pos.set(x, y);
            }

            LevelCardConfig cfg;
            fillCardConfig(cfg, nextId, cardFace, cardSuit, pos);
            outConfig.stackCards.push_back(cfg);
        }
    }

    return true;
}


