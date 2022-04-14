#include <iostream>
#include <launchmymc/ProcessBuilder.hpp>
#include <utility>

namespace lmm {

inline void ProcessBuilder::appendCommand(const std::string &arg) {
  command += arg + " ";
}

ProcessBuilder::ProcessBuilder(
    const std::string &java, const LauncherProfile &profile)
    : ProcessBuilder(java, profile, nullptr) {}

ProcessBuilder::ProcessBuilder(
    const std::string &java,
    const LauncherProfile &profile,
    OutputReaderFunc reader)
    : directory(profile.directory),
      outputReader(std::move(reader)) {
  appendCommand(java);

  for (const std::string &vmArg : profile.vmArgs)
    appendCommand(vmArg);

  appendCommand("-cp");
  appendCommand(profile.classPath);
  appendCommand(profile.mainClass);

  for (const std::string &arg : profile.args)
    appendCommand(arg);
}

const std::string &ProcessBuilder::getCommand() const { return command; }

TinyProcessLib::Process ProcessBuilder::launch() {
  return {command, directory, outputReader, outputReader};
}

} // namespace lmm
