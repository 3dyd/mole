#include "stdafx.h"
#include "event.h"

namespace event {

class ConfigIoCallbackBridge: public config_io_callback_v3, public Bridge {
  BRIDGE_EVENT(on_read);
  BRIDGE_EVENT(on_write, bool);
  BRIDGE_EVENT(on_quicksave);
  BRIDGE_EVENT(on_reset_v3, filesystem::ptr);
  BRIDGE_EVENT(on_write_v3, filesystem::ptr);
  BRIDGE_EVENT(on_quicksave_v3, filesystem::ptr);
};

static service_factory_single_t<ConfigIoCallbackBridge> g_config_io_callback_bridge;

static service_factory_single_t<GroupFromBridge> g_config_io_callback(
  "config_io_callback", g_config_io_callback_bridge.get_static_instance());

} // namespace event
