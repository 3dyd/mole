#define NOMINMAX

#include <sdkddkver.h>
#include "../foobar2000/SDK/foobar2000.h"
#include <atomic>
#include <deque>
#include <map>
#include <mutex>
#include <optional>
#include <atlbase.h>
#include <atlapp.h>
#include <atlwin.h>
#include <atlcrack.h>
#include "version.h"
#include "synopsis.h"

#define mlog  console::formatter() << APP_BINARY << ": "
