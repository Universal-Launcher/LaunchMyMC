#pragma once

#include <launchmymc/GamePath.hpp>
#include <launchmymc/UserInfo.hpp>
#include <map>
#include <simdjson.h>

namespace lmm {

/**
 * Creates an argument replace map based on game configuration.
 *
 * @param gamePath    The configuration's GamePath object.
 * @param userInfo    The account info.
 * @param vanillaJson Vanilla version json document.
 * @param forgeJson   Forge version json document.
 *
 * @return A replace map to be used on the game's argument.
 */
std::map<std::string, std::string> getReplaceMap(
    const GamePath &gamePath,
    const UserInfo &userInfo,
    simdjson::ondemand::document &vanillaJson,
    simdjson::ondemand::document &forgeJson);

/**
 * Replaces in "str" every occurrence of every key of "map" by its corresponding
 * value.
 *
 * @param str The string to replace in.
 * @param map The replace map.
 */
void replaceWithMap(
    std::string &str, const std::map<std::string, std::string> &map);

/**
 * Reads all program arguments from a minecraft version json and add them to an
 * array.
 *
 * @param args    The list to add arguments to.
 * @param json    The minecraft version json document.
 * @param argsMap The replace map to inject variables inside the arguments.
 */
void addArgsFromJson(
    std::vector<std::string> &args,
    simdjson::ondemand::document &json,
    const std::map<std::string, std::string> &argsMap);

/**
 * Reads all VM arguments from a minecraft version json and add them to an
 * array.
 *
 * @param vmArgs  The list to add arguments to.
 * @param json    The minecraft version json document.
 * @param argsMap The replace map to inject variables inside the arguments.
 */
void addVmArgsFromJson(
    std::vector<std::string> &vmArgs,
    simdjson::ondemand::document &json,
    const std::map<std::string, std::string> &argsMap);

} // namespace lmm
