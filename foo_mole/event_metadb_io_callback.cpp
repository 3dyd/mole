#include "stdafx.h"
#include "event.h"

namespace event {

class MetadbIoCallbackBridge: public metadb_io_callback, public Bridge {
public:
  using Bridge::Bridge;

  BRIDGE_EVENT(on_changed_sorted, metadb_handle_list_cref, bool);
};

static service_factory_single_t<BridgedGroup<MetadbIoCallbackBridge>> g_metadb_io_callback(
  "metadb_io_callback", EventNames{"on_changed_sorted"});

} // namespace event
