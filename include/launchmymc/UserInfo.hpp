#pragma once

#include <string>

namespace lmm {

/**
 * A structure containing minecraft account info.
 *
 * Needed by the minecraft client in order to connect to premium servers.
 */
struct UserInfo {
  std::string userName, uuid, accessToken, clientToken;
};

} // namespace lmm
