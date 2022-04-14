#include "profileUtils.hpp"
#include "defs.hpp"

namespace lmm {

std::map<std::string, std::string> getReplaceMap(
    const GamePath &gamePath,
    const UserInfo &userInfo,
    simdjson::ondemand::document &vanillaJson,
    simdjson::ondemand::document &forgeJson) {
  return {
      {"${library_directory}", gamePath.getLibsDir().string()},
      {"${classpath_separator}", JAVA_PATH_SEP},
      {"${auth_player_name}", userInfo.userName},
      {"${version_name}", std::string(forgeJson["id"].get_string().value())},
      {"${game_directory}", gamePath.getGameDir().string()},
      {"${assets_root}", gamePath.getAssetsDir().string()},
      {"${assets_index_name}",
       std::string(vanillaJson["assetIndex"]["id"].get_string().value())},
      {"${auth_uuid}", userInfo.uuid},
      {"${auth_access_token}", userInfo.accessToken},
      {"${user_type}", "mojang"},
      {"${version_type}", "release"},
      {"${natives_directory}", gamePath.getNativesDir().string()}};
}

void replaceWithMap(
    std::string &str, std::map<std::string, std::string> const &map) {
  size_t pos;

  for (const auto &[k, v] : map)
    while ((pos = str.find(k)) != std::string::npos)
      str.replace(pos, k.size(), v);
}

void addArgsFromJson(
    std::vector<std::string> &args,
    simdjson::ondemand::document &json,
    const std::map<std::string, std::string> &argsMap) {
  auto argArray = json["arguments"]["game"];

  if (argArray.error() == simdjson::error_code::NO_SUCH_FIELD)
    return;

  for (auto argObject : argArray) {
    if (argObject.type().value() == simdjson::ondemand::json_type::string) {
      auto argStr = std::string(argObject.get_string().value());

      replaceWithMap(argStr, argsMap);

      args.push_back(argStr);
    }
  }
}

void addVmArgsFromJson(
    std::vector<std::string> &vmArgs,
    simdjson::ondemand::document &json,
    const std::map<std::string, std::string> &argsMap) {
  auto argArray = json["arguments"]["jvm"];

  if (argArray.error() == simdjson::error_code::NO_SUCH_FIELD)
    return;

  for (auto argObject : argArray) {
    if (argObject.type().value() == simdjson::ondemand::json_type::string) {
      auto argStr = std::string(argObject.get_string().value());

      if (argStr.find("minecraft.launcher") != std::string::npos ||
          argStr.find("${classpath}") != std::string::npos ||
          argStr.find("-cp") != std::string::npos)
        continue;

      replaceWithMap(argStr, argsMap);

      vmArgs.push_back(argStr);
    }
  }
}

} // namespace lmm
