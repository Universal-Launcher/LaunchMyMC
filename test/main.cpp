#include <iostream>
#include <launchmymc/LaunchMyMC.hpp>
#include <process.hpp>
#include <simdjson.h>

enum TestProfileType { VANILLA, FORGE, FABRIC };

struct TestProfile {
  std::string name;
  TestProfileType type;
  std::string version;
  std::string modLoaderVersion;
  std::filesystem::path directory;
  bool download;
  std::string java;
};

/**
 * Prints all profiles' names in a test config json document to the standard
 * output.
 *
 * @param json     The test config json document.
 *
 * @return Whether the function succeeded or an error occurred.
 */
bool printProfileNames(
    simdjson::ondemand::document &json) {
  auto profilesObj = json["profiles"].get_object();
  if (profilesObj.error()) {
    std::cerr << "Invalid profiles object" << std::endl;
    return false;
  }

  for (auto field : profilesObj) {
    std::cout << "- " << field.key().value() << std::endl;
  }

  return true;
}

/**
 * Finds a profile by id in a json document, and writes its info to a
 * TestProfile.
 *
 * @param json        The test config json document.
 * @param id          The id of the profile.
 * @param testProfile The TestProfile.
 *
 * @return Whether the function succeeded or an error occurred.
 */
bool getProfile(
    simdjson::ondemand::document &json,
    const std::string &id,
    TestProfile &testProfile) {
  // Get profile json object
  auto profileObj = json["profiles"][id];
  if (profileObj.error()) {
    std::cerr << "Invalid profile" << std::endl;
    return false;
  }

  // Get name
  auto name = profileObj["name"].get_string();
  if (name.error()) {
    std::cerr << "Missing name" << std::endl;
    return false;
  } else
    testProfile.name = name.value();

  // Get type
  auto type = profileObj["type"].get_string();
  if (type.error()) {
    std::cerr << "Missing type" << std::endl;
    return false;
  } else {
    if (type.value() == "vanilla") {
      testProfile.type = VANILLA;
    } else if (type.value() == "forge") {
      testProfile.type = FORGE;
    } else if (type.value() == "fabric") {
      testProfile.type = FABRIC;
    } else {
      std::cerr << "Invalid type" << std::endl;
      return false;
    }
  }

  // Get version
  auto version = profileObj["version"].get_string();
  if (version.error()) {
    std::cerr << "Missing version" << std::endl;
    return false;
  } else
    testProfile.version = version.value();

  // Get mod loader version
  if (testProfile.type != VANILLA) {
    auto modLoaderVersion = profileObj["mod_loader_version"].get_string();
    if (modLoaderVersion.error()) {
      std::cerr << "Missing mod loader version" << std::endl;
      return false;
    } else
      testProfile.modLoaderVersion = modLoaderVersion.value();
  }

  // Get directory
  auto directory = profileObj["directory"].get_string();
  if (directory.error())
    testProfile.directory = testProfile.name;
  else
    testProfile.directory = directory.value();

  // Get download
  auto download = profileObj["download"].get_bool();
  if (download.error())
    testProfile.download = true;
  else
    testProfile.download = download.value();

  // Get java executable
  auto java = profileObj["java"].get_string();
  if (java.error())
    testProfile.java = "java";
  else
    testProfile.java = java.value();

  return true;
}

/**
 * Parses user info from a json file and returns it.
 *
 * @param json     The test config json document.
 * @param userInfo The UserInfo instance to write to.
 *
 * @return Whether the function succeeded or an error occurred.
 */
bool getUserInfo(simdjson::ondemand::document &json, lmm::UserInfo &userInfo) {
  // Get user json object
  auto userObj = json["user"].get_object();
  if (userObj.error()) {
    std::cerr << "Missing user config" << std::endl;
    return false;
  }

  // Get user name
  auto userName = userObj["user_name"].get_string();
  if (userName.error()) {
    std::cerr << "Missing player user name" << std::endl;
    return false;
  }

  // Get UUID
  auto uuid = userObj["uuid"].get_string();
  if (uuid.error()) {
    std::cerr << "Missing player uuid" << std::endl;
    return false;
  }

  // Get access token
  auto accessToken = userObj["access_token"].get_string();
  if (accessToken.error()) {
    std::cerr << "Missing player access token" << std::endl;
    return false;
  }

  // Get client token
  auto clientToken = userObj["client_token"].get_string();
  if (clientToken.error()) {
    std::cerr << "Missing player client token" << std::endl;
    return false;
  }

  userInfo = lmm::UserInfo{
      std::string(userName.value()),
      std::string(uuid.value()),
      std::string(accessToken.value()),
      std::string(clientToken.value())};

  return true;
}

int main() {
  // Load json
  simdjson::ondemand::parser parser;
  const auto jsonStr = simdjson::padded_string::load("test_config.json");
  auto json = parser.iterate(jsonStr).value();

  // Print profile names
  std::cout << "Available test profiles : " << std::endl;
  if (!printProfileNames(json))
    return -1;

  // Ask the user for the profile to launch
  std::cout << "\nEnter profile name : ";
  std::string profileName;
  std::getline(std::cin, profileName);

  TestProfile testProfile;
  if (!getProfile(json, profileName, testProfile))
    return -1;

  // Update the game files
  if (testProfile.download) {
    auto process =
        TinyProcessLib::Process("java -jar autodl.jar " + profileName);

    int exitStatus{process.get_exit_status()};

    if (exitStatus == 0) {
      std::cout << "Successfully updated" << std::endl;
    } else {
      std::cout << "Updater exited with status " << exitStatus << std::endl;
    }
  } else {
    std::cout << "Skipped update" << std::endl;
  }

  // Launch the game
  lmm::GameConfig gameConfig{testProfile.name, testProfile.directory};

  lmm::UserInfo userInfo;
  if (!getUserInfo(json, userInfo))
    return -1;

  lmm::LauncherProfile profile;

  switch (testProfile.type) {
  case VANILLA:
    profile =
        lmm::VanillaProfileBuilder{gameConfig, userInfo, testProfile.version}
            .build();
    break;

  case FORGE:
    profile =
        lmm::ForgeProfileBuilder{
            gameConfig,
            userInfo,
            testProfile.version,
            testProfile.modLoaderVersion}
            .build();
    break;

  case FABRIC:
    profile =
        lmm::FabricProfileBuilder{gameConfig, userInfo, testProfile.version}
            .build();
    break;
  }

  auto launcher = lmm::ProcessBuilder{
      testProfile.java, profile, [](const char *bytes, size_t size) {
        std::cout << "[MC] " << std::string(bytes, size);
      }};

  std::cout << launcher.getCommand() << std::endl;

  auto process = launcher.launch();

  int exitStatus{process.get_exit_status()};
  std::cout << "Process exited with status " << exitStatus << std::endl;
}
