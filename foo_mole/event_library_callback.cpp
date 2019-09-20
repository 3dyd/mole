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

// {FC98D20B-5EAA-4A82-984C-693D5B049C1F}
static const GUID guid_library_callback =
{0xfc98d20b, 0x5eaa, 0x4a82, {0x98, 0x4c, 0x69, 0x3d, 0x5b, 0x4, 0x9c, 0x1f}};

static service_factory_single_t<BridgedGroup<LibraryCallbackBridge>> g_library_callback(
  guid_library_callback, "library_callback",
  EventNames{"on_items_added", "on_items_removed", "on_items_modified"});

} // namespace event
