#include "ClassPath.hpp"
#include "profileUtils.hpp"
#include <launchmymc/FabricProfileBuilder.hpp>
#include <utility>

namespace lmm {

FabricProfileBuilder::FabricProfileBuilder(
    GameConfig gameConfig, UserInfo userInfo, const std::string &vanillaVersion)
    : gameConfig(std::move(gameConfig)),
      userInfo(std::move(userInfo)),
      vanillaJsonPath(
          this->gameConfig.gamePath.getGameDir() / (vanillaVersion + ".json")) {
}

FabricProfileBuilder &
FabricProfileBuilder::withAdditionalArgs(const std::vector<std::string> &args) {
  additionalArgs.reserve(additionalArgs.size() + args.size());

  for (const auto &arg : args)
    additionalArgs.push_back(arg);

  return *this;
}

FabricProfileBuilder &FabricProfileBuilder::withAdditionalVmArgs(
    const std::vector<std::string> &args) {
  additionalVmArgs.reserve(additionalVmArgs.size() + args.size());

  for (const auto &arg : args)
    additionalVmArgs.push_back(arg);

  return *this;
}

FabricProfileBuilder &FabricProfileBuilder::setCustomVanillaJson(
    const std::filesystem::path &jsonPath) {
  vanillaJsonPath = jsonPath;
  return *this;
}

LauncherProfile FabricProfileBuilder::build() const {
  simdjson::ondemand::parser parser;
  const auto jsonStr = simdjson::padded_string::load(vanillaJsonPath.string());
  auto json = parser.iterate(jsonStr);

  LauncherProfile profile;

  profile.name = gameConfig.name;
  profile.directory = gameConfig.gamePath.getGameDir();

  profile.mainClass = "net.fabricmc.loader.launch.knot.KnotClient";

  auto classPath =
      ClassPath()
          .addFromJson(json.value(), gameConfig.gamePath.getLibsDir())
          .add(gameConfig.gamePath.getJarFile());

  for (const auto &dirEntry : std::filesystem::recursive_directory_iterator(
           gameConfig.gamePath.getLibsDir() / "net" / "fabricmc"))
    if (dirEntry.path().extension() == ".jar")
      classPath.add(dirEntry.path());

  for (const auto &dirEntry : std::filesystem::recursive_directory_iterator(
           gameConfig.gamePath.getLibsDir() / "org" / "ow2"))
    if (dirEntry.path().extension() == ".jar")
      classPath.add(dirEntry.path());

  profile.classPath = classPath.get();

  const auto argsMap =
      getReplaceMap(gameConfig.gamePath, userInfo, json.value(), json.value());

  profile.args = additionalArgs;
  addArgsFromJson(profile.args, json.value(), argsMap);

  profile.vmArgs = additionalVmArgs;
  addVmArgsFromJson(profile.vmArgs, json.value(), argsMap);

  return profile;
}

} // namespace lmm
