#pragma once

namespace lmm {

/**
 * Platform dependent path separator used when building classpath
 */
#ifdef _WIN32
#define JAVA_PATH_SEP ";"
#else
#define JAVA_PATH_SEP ":"
#endif

} // namespace lmm
