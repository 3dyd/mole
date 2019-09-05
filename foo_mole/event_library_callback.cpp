#include "stdafx.h"
#include "event.h"

namespace event {

class LibraryCallbackBridge: public library_callback, public Bridge {
  BRIDGE_EVENT(on_items_added, const pfc::list_base_const_t<metadb_handle_ptr>&);
  BRIDGE_EVENT(on_items_removed, const pfc::list_base_const_t<metadb_handle_ptr>&);
  BRIDGE_EVENT(on_items_modified, const pfc::list_base_const_t<metadb_handle_ptr>&);
};

static service_factory_single_t<LibraryCallbackBridge> g_library_callback_bridge;

static service_factory_single_t<GroupFromBridge> g_library_callback(
  "library_callback", g_library_callback_bridge.get_static_instance());

} // namespace event
