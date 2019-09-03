# How to Build

    git clone --recurse-submodules https://github.com/3dyd/mole.git

Visual Studio solution file is `mole/foo_mole/foo_mole.sln`.

Switch all projects that are not yet switched to `v141_xp` toolset (project Properties -> General -> Platform Toolset). If `columns_ui_sdk` gives compilation error then add `CINTERFACE` to preprocessor definitions (project Properties -> C/C++ -> Preprocessor -> Preprocessor Definitions).
