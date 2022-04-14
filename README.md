# LaunchMyMC

## Cross-platform Minecraft launching library

## Example

```c++
#include <launchmymc/LaunchMyMC.hpp>

int main() {
  auto gameConfig = lmm::GameConfig{"Minecraft 1.16.5", ".minecraft"};

  auto userInfo = lmm::UserInfo
      {"MinecraftPlayer42", "uuid", "access-token", "client-token"};

  auto profile = lmm::ForgeProfileBuilder
      {gameConfig, userInfo, "1.16.5", "36.1.0"}.build();

  auto process = lmm::ProcessBuilder{"java", profile}.launch();
}
```

## Libraries

- [simdjson](https://github.com/simdjson/simdjson)
- [tiny-process-library](https://gitlab.com/eidheim/tiny-process-library/)

## Building the library

```shell
xmake
```

## Running the test

[More info about testing can be found here](/test)

```shell
# Configure the project with debug mode and enable "test" target
xmake f -m debug --test=true

# Build all targets
xmake

# Run test
xmake run -w ./test test ...arguments
```

## Generate project files

### VS Code (compile_commands.json)

```shell
xmake project -k compile_commands
```

### Clion (CMakeLists.txt)

```shell
xmake project -k cmake -y
```
