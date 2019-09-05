#include "stdafx.h"
#include "event.h"

namespace event {

class MetadbIoEditCallbackBridge: public metadb_io_edit_callback, public Bridge {
  BRIDGE_EVENT(on_edited, metadb_handle_list_cref, t_infosref, t_infosref);
};

static service_factory_single_t<MetadbIoEditCallbackBridge> g_metadb_io_edit_callback_bridge;

static service_factory_single_t<GroupFromBridge> g_metadb_io_edit_callback(
  "metadb_io_edit_callback", g_metadb_io_edit_callback_bridge.get_static_instance());

} // namespace event
