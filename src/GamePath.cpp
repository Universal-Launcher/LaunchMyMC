#include <iostream>
#include <launchmymc/GamePath.hpp>

namespace lmm {

GamePath::GamePath(const std::filesystem::path &gameDir)
    : GamePath(
          gameDir,
          gameDir / "assets",
          gameDir / "libraries",
          gameDir / "natives",
          gameDir / "client.jar") {}

GamePath::GamePath(
    const std::filesystem::path &gameDir,
    const std::filesystem::path &assetsDir,
    const std::filesystem::path &libsDir,
    const std::filesystem::path &nativesDir,
    const std::filesystem::path &jarFile)
    : gameDir(std::filesystem::absolute(gameDir)),
      assetsDir(std::filesystem::absolute(assetsDir)),
      libsDir(std::filesystem::absolute(libsDir)),
      nativesDir(std::filesystem::absolute(nativesDir)),
      jarFile(std::filesystem::absolute(jarFile)) {

  if (!std::filesystem::exists(gameDir) ||
      !std::filesystem::is_directory(gameDir))
    throw std::invalid_argument("Invalid game directory");

  if (!std::filesystem::exists(assetsDir) ||
      !std::filesystem::is_directory(assetsDir))
    throw std::invalid_argument("Invalid assets directory");

  if (!std::filesystem::exists(libsDir) ||
      !std::filesystem::is_directory(libsDir))
    throw std::invalid_argument("Invalid libraries directory");

  if (!std::filesystem::exists(nativesDir) ||
      !std::filesystem::is_directory(nativesDir))
    throw std::invalid_argument("Invalid natives directory");

  if (!std::filesystem::exists(jarFile) ||
      !std::filesystem::is_regular_file(jarFile))
    throw std::invalid_argument("Invalid jar file");
}

const std::filesystem::path &GamePath::getAssetsDir() const {
  return assetsDir;
}

const std::filesystem::path &GamePath::getLibsDir() const { return libsDir; }

const std::filesystem::path &GamePath::getNativesDir() const {
  return nativesDir;
}

const std::filesystem::path &GamePath::getJarFile() const { return jarFile; }

const std::filesystem::path &GamePath::getGameDir() const { return gameDir; }

} // namespace lmm
