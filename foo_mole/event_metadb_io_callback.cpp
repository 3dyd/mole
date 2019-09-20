#include "stdafx.h"
#include "event.h"

namespace event {

class MetadbIoCallbackBridge: public metadb_io_callback, public Bridge {
public:
  using Bridge::Bridge;

  BRIDGE_EVENT(on_changed_sorted, metadb_handle_list_cref, bool);
};

// {906F9023-B1DA-4164-8023-310326C2A047}
static const GUID guid_metadb_io_callback =
{0x906f9023, 0xb1da, 0x4164, {0x80, 0x23, 0x31, 0x3, 0x26, 0xc2, 0xa0, 0x47}};

static service_factory_single_t<BridgedGroup<MetadbIoCallbackBridge>> g_metadb_io_callback(
  guid_metadb_io_callback, "metadb_io_callback", EventNames{"on_changed_sorted"});

} // namespace event
