# Module configuration files

Worldserver loads module configurations recursively from the `modules` subdirectory next to its main configuration file. A module can provide templates at `modules/<module>/conf/*.conf.dist`; CMake copies these templates to the worldserver configuration directory's `modules` subdirectory and installs them there when `COPY_CONF` is enabled.

Both `.conf` and `.conf.dist` files in that configuration directory are loaded. If a `.conf` file exists beside a `.conf.dist` file, the `.conf` file is loaded instead, so local settings can override the module defaults without editing the distributed template. Module configurations are applied after `worldserver.conf` and `worldserver.conf.d` files.

Keep template filenames unique across modules because CMake installs them together in the configuration directory's `modules` folder.
