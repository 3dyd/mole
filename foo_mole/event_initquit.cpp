#include "stdafx.h"
#include "event.h"

namespace event {

class InitquitBridge: public initquit, public Bridge {
  BRIDGE_EVENT(on_init);
  BRIDGE_EVENT(on_quit);
};

static service_factory_single_t<InitquitBridge> g_initquit_bridge;
static service_factory_single_t<GroupFromBridge> g_initquit("initquit", g_initquit_bridge.get_static_instance());

} // namespace event
