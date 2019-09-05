#include "stdafx.h"
#include "event.h"

namespace event {

class MetadbIoCallbackBridge: public metadb_io_callback, public Bridge {
  BRIDGE_EVENT(on_changed_sorted, metadb_handle_list_cref, bool);
};

static service_factory_single_t<MetadbIoCallbackBridge> g_metadb_io_callback_bridge;

static service_factory_single_t<GroupFromBridge> g_metadb_io_callback(
  "metadb_io_callback", g_metadb_io_callback_bridge.get_static_instance());

} // namespace event
