#pragma once

#define APP_NAME          "Mole"
#define APP_DESCRIPTION   "API-related diagnostics and basic scenario support"
#define APP_BINARY        "foo_mole"

#define APP_VERSION_MAJOR 0
#define APP_VERSION_MINOR 1
#define APP_VERSION_BUILD 0

#define __STR(text)  #text
#define _STR(text)  __STR(text)

#if APP_VERSION_BUILD != 0
#define APP_VERSION _STR(APP_VERSION_MAJOR) "." _STR(APP_VERSION_MINOR) "." _STR(APP_VERSION_BUILD)
#else
#define APP_VERSION _STR(APP_VERSION_MAJOR) "." _STR(APP_VERSION_MINOR)
#endif
