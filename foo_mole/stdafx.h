#define NOMINMAX

#include <sdkddkver.h>
#include "../foobar2000/SDK/foobar2000.h"
#include <atomic>
#include <string_view>
#include "version.h"

#define mlog  console::formatter() << APP_BINARY << ": "
