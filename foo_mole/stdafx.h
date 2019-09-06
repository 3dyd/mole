#define NOMINMAX

#include <sdkddkver.h>
#include "../foobar2000/SDK/foobar2000.h"
#include <atomic>
#include <deque>
#include <map>
#include <mutex>
#include <optional>
#include "version.h"

#define mlog  console::formatter() << APP_BINARY << ": "
