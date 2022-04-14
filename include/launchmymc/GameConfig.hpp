#pragma once

#include <launchmymc/GamePath.hpp>
#include <string>
#include <utility>

namespace lmm {

/**
 * A structure containing info about the game's directories and displayed name.
 */
struct GameConfig {
  /**
   * Creates a GameConfig using a custom name and GamePath.
   *
   * @param name     The configuration name.
   * @param gamePath The GamePath object.
   */
  GameConfig(std::string name, GamePath gamePath)
      : name(std::move(name)),
        gamePath(std::move(gamePath)) {}

  /**
   * Creates a GameConfig using a custom name and default GamePath (follows
   * FlowUpdater's directories format)
   *
   * @param name    The configuration name.
   * @param gameDir The root game directory.
   */
  GameConfig(std::string name, const std::filesystem::path &gameDir)
      : name(std::move(name)),
        gamePath(GamePath(gameDir)) {}

  /**
   * Name of the configuration. May be displayed in MacOS's dock for example.
   *
   * Note : the name isn't used as a unique identifier by LMM.
   */
  std::string name;

  /**
   * Various directories containing the game's files.
   */
  GamePath gamePath;
};

} // namespace lmm
