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

class ConfigIoCallback: public Group {
public:
  const char* GetName() override {
    return "config_io_callback";
  }

  void Schedule(const char* event, const char* commands) override {
    g_config_io_callback_bridge.get_static_instance().Schedule(event, commands);
  }
};

static service_factory_single_t<ConfigIoCallback> g_config_io_callback;

} // namespace event
