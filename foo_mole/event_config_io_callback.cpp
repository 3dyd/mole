#include "stdafx.h"
#include "event.h"

namespace event {

class ConfigIoCallbackBridge: public config_io_callback_v3, public Bridge {
public:
  using Bridge::Bridge;

  BRIDGE_EVENT(on_read);
  BRIDGE_EVENT(on_write, bool);
  BRIDGE_EVENT(on_quicksave);
  BRIDGE_EVENT(on_reset_v3, filesystem::ptr);
  BRIDGE_EVENT(on_write_v3, filesystem::ptr);
  BRIDGE_EVENT(on_quicksave_v3, filesystem::ptr);
};

// {0B081F18-E299-403D-8600-02A0D16DC52C}
static const GUID guid_config_io_callback =
{0xb081f18, 0xe299, 0x403d, {0x86, 0x0, 0x2, 0xa0, 0xd1, 0x6d, 0xc5, 0x2c}};

static service_factory_single_t<BridgedGroup<ConfigIoCallbackBridge>> g_config_io_callback(
  guid_config_io_callback, "config_io_callback",
  EventNames{"on_read", "on_write", "on_quicksave", "on_reset_v3", "on_write_v3", "on_quicksave_v3"});

} // namespace event
