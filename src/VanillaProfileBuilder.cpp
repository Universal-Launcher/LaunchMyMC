#include "ClassPath.hpp"
#include "profileUtils.hpp"
#include <launchmymc/VanillaProfileBuilder.hpp>
#include <utility>

namespace lmm {

VanillaProfileBuilder::VanillaProfileBuilder(
    GameConfig gameConfig, UserInfo userInfo, const std::string &vanillaVersion)
    : gameConfig(std::move(gameConfig)),
      userInfo(std::move(userInfo)),
      vanillaJsonPath(
          this->gameConfig.gamePath.getGameDir() / (vanillaVersion + ".json")) {
}

VanillaProfileBuilder &VanillaProfileBuilder::withAdditionalArgs(
    const std::vector<std::string> &args) {
  additionalArgs.reserve(additionalArgs.size() + args.size());

  for (const auto &arg : args)
    additionalArgs.push_back(arg);

  return *this;
}

VanillaProfileBuilder &VanillaProfileBuilder::withAdditionalVmArgs(
    const std::vector<std::string> &args) {
  additionalVmArgs.reserve(additionalVmArgs.size() + args.size());

  for (const auto &arg : args)
    additionalVmArgs.push_back(arg);

  return *this;
}

VanillaProfileBuilder &VanillaProfileBuilder::setCustomVanillaJson(
    const std::filesystem::path &jsonPath) {
  vanillaJsonPath = jsonPath;
  return *this;
}

LauncherProfile VanillaProfileBuilder::build() const {
  simdjson::ondemand::parser parser;
  const auto jsonStr = simdjson::padded_string::load(vanillaJsonPath.string());
  auto json = parser.iterate(jsonStr);

  LauncherProfile profile;

  profile.name = gameConfig.name;
  profile.directory = gameConfig.gamePath.getGameDir();

  profile.mainClass = std::string(json["mainClass"].get_string().value());

  profile.classPath =
      ClassPath()
          .addFromJson(json.value(), gameConfig.gamePath.getLibsDir())
          .add(gameConfig.gamePath.getJarFile())
          .get();

  const auto argsMap =
      getReplaceMap(gameConfig.gamePath, userInfo, json.value(), json.value());

  profile.args = additionalArgs;
  addArgsFromJson(profile.args, json.value(), argsMap);

  profile.vmArgs = additionalVmArgs;
  addVmArgsFromJson(profile.vmArgs, json.value(), argsMap);

  return profile;
}

} // namespace lmm
