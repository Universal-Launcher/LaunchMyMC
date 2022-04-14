#include "ClassPath.hpp"
#include "profileUtils.hpp"
#include <launchmymc/ForgeProfileBuilder.hpp>
#include <utility>

namespace lmm {

ForgeProfileBuilder::ForgeProfileBuilder(
    GameConfig gameConfig,
    UserInfo userInfo,
    const std::string &vanillaVersion,
    const std::string &forgeVersion)
    : gameConfig(std::move(gameConfig)),
      userInfo(std::move(userInfo)),
      vanillaJsonPath(
          this->gameConfig.gamePath.getGameDir() / (vanillaVersion + ".json")),
      forgeJsonPath(
          this->gameConfig.gamePath.getGameDir() /
          (vanillaVersion + "-forge-" + forgeVersion + ".json")) {}

ForgeProfileBuilder &
ForgeProfileBuilder::withAdditionalArgs(const std::vector<std::string> &args) {
  additionalArgs.reserve(additionalArgs.size() + args.size());

  for (const auto &arg : args)
    additionalArgs.push_back(arg);

  return *this;
}

ForgeProfileBuilder &ForgeProfileBuilder::withAdditionalVmArgs(
    const std::vector<std::string> &args) {
  additionalVmArgs.reserve(additionalVmArgs.size() + args.size());

  for (const auto &arg : args)
    additionalVmArgs.push_back(arg);

  return *this;
}

ForgeProfileBuilder &ForgeProfileBuilder::withCustomVanillaJson(
    const std::filesystem::path &jsonPath) {
  vanillaJsonPath = jsonPath;
  return *this;
}

ForgeProfileBuilder &ForgeProfileBuilder::withCustomForgeJson(
    const std::filesystem::path &jsonPath) {
  forgeJsonPath = jsonPath;
  return *this;
}

LauncherProfile ForgeProfileBuilder::build() const {
  simdjson::ondemand::parser vanillaParser, forgeParser;

  auto vanillaJsonStr = simdjson::padded_string::load(vanillaJsonPath.string()),
       forgeJsonStr = simdjson::padded_string::load(forgeJsonPath.string());

  auto vanillaJson = vanillaParser.iterate(vanillaJsonStr),
       forgeJson = forgeParser.iterate(forgeJsonStr);

  LauncherProfile profile;

  profile.name = gameConfig.name;
  profile.directory = gameConfig.gamePath.getGameDir();

  profile.mainClass = std::string(forgeJson["mainClass"].get_string().value());

  profile.classPath =
      ClassPath()
          .addFromJson(forgeJson.value(), gameConfig.gamePath.getLibsDir())
          .addFromJson(vanillaJson.value(), gameConfig.gamePath.getLibsDir())
          .add(gameConfig.gamePath.getJarFile())
          .get();

  const auto argsMap = getReplaceMap(
      gameConfig.gamePath, userInfo, vanillaJson.value(), forgeJson.value());

  profile.args = additionalArgs;
  addArgsFromJson(profile.args, vanillaJson.value(), argsMap);
  addArgsFromJson(profile.args, forgeJson.value(), argsMap);

  profile.vmArgs = additionalVmArgs;
  addVmArgsFromJson(profile.vmArgs, vanillaJson.value(), argsMap);
  addVmArgsFromJson(profile.vmArgs, forgeJson.value(), argsMap);

  return profile;
}

} // namespace lmm
