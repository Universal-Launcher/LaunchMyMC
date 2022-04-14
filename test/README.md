# Testing

## Building and running tests

```shell
# Configure the project with debug mode and enable "test" target
xmake f -m debug --test=true

# Build all targets
xmake

# Run test
# -w option is needed to use relative paths
# The target name is only required for providing arguments
xmake run -w ./test test ...arguments
```

## Profiles

Test profiles can be configured in the `test_config.json` file.

[Three examples](test_config.json), and a [validation schema](test_config.schema.json) are provided.

## Notes

Minecraft/forge/fabric files, as well as mods, are downloaded using
[FlowUpdater](https://github.com/flowArg/FlowUpdater).


