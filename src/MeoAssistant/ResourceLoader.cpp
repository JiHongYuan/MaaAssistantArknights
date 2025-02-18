#include "ResourceLoader.h"

#include <filesystem>

#include "Logger.hpp"

#include "BattleDataConfiger.h"
#include "CopilotConfiger.h"
#include "GeneralConfiger.h"
#include "InfrastConfiger.h"
#include "ItemConfiger.h"
#include "OcrPack.h"
#include "RecruitConfiger.h"
#include "RoguelikeCopilotConfiger.h"
#include "RoguelikeRecruitConfiger.h"
#include "RoguelikeShoppingConfiger.h"
#include "StageDropsConfiger.h"
#include "TaskData.h"
#include "TemplResource.h"
#include "TilePack.h"

bool asst::ResourceLoader::load(const std::filesystem::path& path)
{
    using namespace asst::utils::path_literals;

#define LoadResourceAndCheckRet(Configer, Filename)                         \
    {                                                                       \
        LogTraceScope(std::string("LoadResourceAndCheckRet ") + #Configer); \
        auto full_path = path / Filename;                                   \
        bool ret = load_resource<Configer>(full_path);                      \
        if (!ret) {                                                         \
            Log.error(#Configer, " load failed, path:", full_path);         \
            return false;                                                   \
        }                                                                   \
    }

#define LoadResourceWithTemplAndCheckRet(Configer, Filename, TemplDir)                             \
    {                                                                                              \
        LogTraceScope(std::string("LoadResourceWithTemplAndCheckRet ") + #Configer);               \
        auto full_path = path / Filename;                                                          \
        auto full_templ_dir = path / TemplDir;                                                     \
        bool ret = load_resource_with_templ<Configer>(full_path, full_templ_dir);                  \
        if (!ret) {                                                                                \
            Log.error(#Configer, "load failed, path:", full_path, ", templ dir:", full_templ_dir); \
            return false;                                                                          \
        }                                                                                          \
    }

    LogTraceFunction;

    /* load resource with json files*/
    LoadResourceAndCheckRet(GeneralConfiger, "config.json"_p);
    LoadResourceAndCheckRet(RecruitConfiger, "recruitment.json"_p);
    LoadResourceAndCheckRet(StageDropsConfiger, "stages.json"_p);
    LoadResourceAndCheckRet(RoguelikeCopilotConfiger, "roguelike_copilot.json"_p);
    LoadResourceAndCheckRet(RoguelikeRecruitConfiger, "roguelike_recruit.json"_p);
    LoadResourceAndCheckRet(RoguelikeShoppingConfiger, "roguelike_shopping.json"_p);
    LoadResourceAndCheckRet(BattleDataConfiger, "battle_data.json"_p);

    /* load resource with json and template files*/
    LoadResourceWithTemplAndCheckRet(TaskData, "tasks.json"_p, "template"_p);
    LoadResourceWithTemplAndCheckRet(InfrastConfiger, "infrast.json"_p, "template"_p / "infrast"_p);
    LoadResourceWithTemplAndCheckRet(ItemConfiger, "item_index.json"_p, "template"_p / "items"_p);

    /* load 3rd parties resource */
    LoadResourceAndCheckRet(TilePack, "Arknights-Tile-Pos"_p / "levels.json"_p);
    LoadResourceAndCheckRet(OcrPack, "PaddleOCR"_p);

    m_loaded = true;

#undef LoadTemplByConfigerAndCheckRet
#undef LoadResourceAndCheckRet

    return true;
}
