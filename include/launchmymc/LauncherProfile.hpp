#pragma once

#include <launchmymc/GameConfig.hpp>
#include <launchmymc/UserInfo.hpp>
#include <string>
#include <vector>

namespace lmm {

/**
 * A structure holding information to launch a java program.
 */
struct LauncherProfile {
  /**
   * A name to display to users.
   */
  std::string name;

  /**
   * The working directory.
   */
  std::filesystem::path directory;

  /**
   * Fully qualified main class name.
   */
  std::string mainClass;

  /**
   * The class path, containing all libraries required by the program.
   */
  std::string classPath;

  /**
   * List of program arguments.
   */
  std::vector<std::string> args;

  /**
   * List of VM arguments.
   */
  std::vector<std::string> vmArgs;
};

} // namespace lmm
