#pragma once

#include <launchmymc/GameConfig.hpp>
#include <launchmymc/LauncherProfile.hpp>
#include <launchmymc/UserInfo.hpp>
#include <vector>

namespace lmm {

/**
 * LauncherProfile builder for minecraft forge.
 *
 * All information is left unprocessed until the build method is called.
 */
class ForgeProfileBuilder {
public:
  /**
   * Initializes a forge profile builder.
   *
   * @param gameConfig     The game configuration.
   * @param userInfo       The user info.
   * @param vanillaVersion The minecraft version number (e.g. "1.12.2").
   * @param forgeVersion   The minecraft forge version number (e.g.
   * "1.12.2-forge-14.23.5.2860").
   */
  ForgeProfileBuilder(
      GameConfig gameConfig,
      UserInfo userInfo,
      const std::string &vanillaVersion,
      const std::string &forgeVersion);

  /**
   * Adds program arguments.
   *
   * @param args List of arguments to add.
   *
   * @return Itself.
   */
  ForgeProfileBuilder &withAdditionalArgs(const std::vector<std::string> &args);

  /**
   * Adds VM arguments.
   *
   * @param args List of arguments to add.
   *
   * @return Itself.
   */
  ForgeProfileBuilder &
  withAdditionalVmArgs(const std::vector<std::string> &args);

  /**
   * Sets a custom minecraft version json.
   *
   * @param jsonPath The path of the json file.
   *
   * @return Itself.
   */
  ForgeProfileBuilder &
  withCustomVanillaJson(const std::filesystem::path &jsonPath);

  /**
   * Sets a custom minecraft forge version json.
   *
   * @param jsonPath The path of the json file.
   *
   * @return Itself.
   */
  ForgeProfileBuilder &
  withCustomForgeJson(const std::filesystem::path &jsonPath);

  /**
   * Builds the profile using all provided info.
   *
   * @return Generic, ready-to-launch, LauncherProfile structure.
   */
  [[nodiscard]] LauncherProfile build() const;

private:
  GameConfig gameConfig;
  UserInfo userInfo;
  std::filesystem::path vanillaJsonPath, forgeJsonPath;
  std::vector<std::string> additionalArgs, additionalVmArgs;
};

} // namespace lmm
