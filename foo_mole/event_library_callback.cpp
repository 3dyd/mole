#include "stdafx.h"
#include "event.h"

namespace event {

class LibraryCallbackBridge: public library_callback, public Bridge {
public:
  using Bridge::Bridge;

  BRIDGE_EVENT(on_items_added, const pfc::list_base_const_t<metadb_handle_ptr>&);
  BRIDGE_EVENT(on_items_removed, const pfc::list_base_const_t<metadb_handle_ptr>&);
  BRIDGE_EVENT(on_items_modified, const pfc::list_base_const_t<metadb_handle_ptr>&);
};

static service_factory_single_t<BridgedGroup<LibraryCallbackBridge>> g_library_callback(
  "library_callback", EventNames{"on_items_added", "on_items_removed", "on_items_modified"});

} // namespace event
