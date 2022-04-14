#pragma once

#include <filesystem>
#include <simdjson.h>
#include <string>
#include <vector>

namespace lmm {

/**
 * A class used internally to generate java classpath using minecraft version
 * files.
 *
 * Uses the builder pattern.
 */
class ClassPath {
public:
  /**
   * Adds a single file to the classpath.
   *
   * @param file The file to add.
   *
   * @return Itself.
   */
  ClassPath &add(const std::string &file);

  /**
   * Adds all libraries from a minecraft version file.
   *
   * @param json    The json document of the version file.
   * @param libsDir The game's libraries' directory (in order to make the paths
   * absolute)
   *
   * @return Itself.
   */
  ClassPath &addFromJson(
      simdjson::ondemand::document &json, const std::filesystem::path &libsDir);

  /**
   * Returns the classpath as a string.
   *
   * @return The file list, joined using java's path separator.
   */
  [[nodiscard]] std::string get() const;

private:
  /**
   * The list of files.
   */
  std::vector<std::string> files;
};

} // namespace lmm
