#include "ClassPath.hpp"
#include "defs.hpp"

using namespace simdjson;

namespace lmm {

ClassPath &ClassPath::add(const std::string &file) {
  if (std::filesystem::exists(file) &&
      std::find(files.begin(), files.end(), file) == files.end())
    files.push_back(file);

  return *this;
}

ClassPath &ClassPath::addFromJson(
    ondemand::document &json, const std::filesystem::path &libsDir) {
  for (auto libObject : json["libraries"].get_array())
    add(libsDir /
        libObject["downloads"]["artifact"]["path"].get_string().value());

  return *this;
}

std::string ClassPath::get() const {
  std::string classPath;

  for (const auto &file : files)
    classPath += file + JAVA_PATH_SEP;

  return classPath;
}

} // namespace lmm
