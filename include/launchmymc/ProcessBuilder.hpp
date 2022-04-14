#pragma once

#include <launchmymc/LauncherProfile.hpp>
#include <process.hpp>

namespace lmm {

/**
 * Type alias for output reading callbacks.
 */
typedef std::function<void(const char *bytes, size_t n)> OutputReaderFunc;

/**
 * A class for creating a new process from a LauncherProfile, using the
 * TinyProcessLibrary.
 */
class ProcessBuilder {
public:
  /**
   * Initializes a process builder using a java executable and a
   * LauncherProfile.
   *
   * @param java    The name of the java executable. "java" will use the default
   * one (or throw an error if there are none present in the path) ;
   * alternatively, you can ask the user for a JRE, or manually detect one and
   * pass the full path.
   * @param profile The launcher profile.
   *
   * @note As no callback is set, the program's output will not be redirected.
   */
  ProcessBuilder(const std::string &java, const LauncherProfile &profile);

  /**
   * Initializes a process builder using a java executable and a
   * LauncherProfile, and registers an stdout reader callback.
   *
   * @param java    The name of the java executable. "java" will use the default
   * one (or throw an error if there are none present in the path) ;
   * alternatively, you can ask the user for a JRE, or manually detect one and
   * pass the full path.
   * @param profile The launcher profile.
   * @param reader  An stdout reader callback.
   */
  ProcessBuilder(
      const std::string &java,
      const LauncherProfile &profile,
      OutputReaderFunc reader);

  /**
   * Returns the command string
   */
  [[nodiscard]] const std::string &getCommand() const;

  /**
   * Launches the process using the TinyProcessLibrary.
   *
   * @return A process instance.
   */
  TinyProcessLib::Process launch();

private:
  /**
   * Append an argument to the command string.
   *
   * @param arg The string to append.
   */
  inline void appendCommand(const std::string &arg);

  std::string command;
  std::filesystem::path directory;

  OutputReaderFunc outputReader;
};

} // namespace lmm
