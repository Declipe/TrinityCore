# External script modules

TrinityCore can build optional script modules from the repository's `modules/` directory. Each module is a directory containing a `src/` directory; module source files are discovered automatically. The included `mod-test` example registers a world script that logs a message at startup.

```text
modules/
  mod-test/
    CMakeLists.txt
    src/
      mod_test.cpp
      mod_test_loader.cpp
```

The module loader registers its scripts through TrinityCore's normal script API. `AC_ADD_SCRIPTS(src)` discovers the sources and `AC_ADD_SCRIPT_LOADER("mod_test" "")` registers the `Addmod_testScripts` loader. Module names containing `-` use `_` in the loader function name. The conventional AzerothCore CMake helpers `AC_ADD_SCRIPT`, `AC_ADD_SCRIPTS`, and `AC_ADD_SCRIPT_LOADER` are available in a module's optional `CMakeLists.txt`.

Enable and build the example module with:

```text
cmake -S . -B build -DMODULES=static
cmake --build build --target scripts
```

The test module writes `Test module loaded successfully.` to the worldserver startup log. Set `MODULES=dynamic` to build it as a loadable script library instead.

Configure the module build mode with `MODULES` (`none`, `static`, or `dynamic`). Individual modules can override the default with `MODULE_<NAME>` (`disabled`, `static`, or `dynamic`), where the module name is uppercased and `-` becomes `_`. For example:

```text
cmake -S . -B build -DMODULES=static -DMODULE_MOD_EXAMPLE=disabled
```

Static modules are linked into the server's script library. Dynamic modules are built as script shared libraries and use TrinityCore's existing script-module loader and hot-reload support. Existing TrinityCore script modules and the `SCRIPTS` build option are independent of this setting.
