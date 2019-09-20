#include "stdafx.h"
#include "event.h"

namespace event {

class InitquitBridge: public initquit, public Bridge {
public:
  using Bridge::Bridge;

  BRIDGE_EVENT(on_init);
  BRIDGE_EVENT(on_quit);
};

// {43276E3C-6390-4F89-A4D6-FD79F3A26968}
static const GUID guid_initquit =
{0x43276e3c, 0x6390, 0x4f89, {0xa4, 0xd6, 0xfd, 0x79, 0xf3, 0xa2, 0x69, 0x68}};

static service_factory_single_t<BridgedGroup<InitquitBridge>> g_initquit(
  guid_initquit, "initquit", EventNames{"on_init", "on_quit"});

} // namespace event
