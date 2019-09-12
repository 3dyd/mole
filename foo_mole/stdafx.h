#define NOMINMAX

#include <sdkddkver.h>
#include "../foobar2000/SDK/foobar2000.h"
#include <psapi.h>

#include <atomic>
#include <deque>
#include <map>
#include <mutex>
#include <optional>

#include <atlbase.h>
#include <atlapp.h>
#include <atlcrack.h>
#include <atlctrls.h>
#include <atlframe.h>
#include <atlwin.h>

#include "version.h"
#include "synopsis.h"

#define mlog  console::formatter() << APP_BINARY << ": "
