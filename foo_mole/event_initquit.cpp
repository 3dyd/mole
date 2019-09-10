#include "stdafx.h"
#include "event.h"

namespace event {

class InitquitBridge: public initquit, public Bridge {
public:
  using Bridge::Bridge;

  BRIDGE_EVENT(on_init);
  BRIDGE_EVENT(on_quit);
};

static service_factory_single_t<BridgedGroup<InitquitBridge>> g_initquit("initquit");

} // namespace event
