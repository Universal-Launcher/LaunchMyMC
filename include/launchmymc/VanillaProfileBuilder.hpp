#pragma once

#include <launchmymc/GameConfig.hpp>
#include <launchmymc/LauncherProfile.hpp>
#include <launchmymc/UserInfo.hpp>
#include <vector>

namespace lmm {

/**
 * LauncherProfile builder for vanilla minecraft.
 *
 * All information is left unprocessed until the build method is called.
 */
class VanillaProfileBuilder {
public:
  /**
   * Initializes a vanilla profile builder.
   *
   * @param gameConfig The game configuration.
   * @param userInfo The user info.
   * @param vanillaVersion The minecraft version number (e.g. "1.12.2").
   */
  VanillaProfileBuilder(
      GameConfig gameConfig,
      UserInfo userInfo,
      const std::string &vanillaVersion);

  /**
   * Adds program arguments.
   *
   * @param args List of arguments to add.
   *
   * @return Itself.
   */
  VanillaProfileBuilder &
  withAdditionalArgs(const std::vector<std::string> &args);

  /**
   * Adds VM arguments.
   *
   * @param args List of arguments to add.
   *
   * @return Itself.
   */
  VanillaProfileBuilder &
  withAdditionalVmArgs(const std::vector<std::string> &args);

  /**
   * Sets a custom minecraft version json.
   *
   * @param jsonPath The path of the json file.
   *
   * @return Itself.
   */
  VanillaProfileBuilder &
  setCustomVanillaJson(const std::filesystem::path &jsonPath);

  /**
   * Builds the profile using all provided info.
   *
   * @return Generic, ready-to-launch, LauncherProfile structure.
   */
  [[nodiscard]] LauncherProfile build() const;

private:
  GameConfig gameConfig;
  UserInfo userInfo;
  std::filesystem::path vanillaJsonPath;
  std::vector<std::string> additionalArgs, additionalVmArgs;
};

} // namespace lmm
