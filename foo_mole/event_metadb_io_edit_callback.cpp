#include "stdafx.h"
#include "event.h"

namespace event {

class MetadbIoEditCallbackBridge: public metadb_io_edit_callback, public Bridge {
public:
  using Bridge::Bridge;

  BRIDGE_EVENT(on_edited, metadb_handle_list_cref, t_infosref, t_infosref);
};

// {68353BAE-DF9C-43F0-8743-216902F55CC3}
static const GUID guid_metadb_io_edit_callback =
{0x68353bae, 0xdf9c, 0x43f0, {0x87, 0x43, 0x21, 0x69, 0x2, 0xf5, 0x5c, 0xc3}};

static service_factory_single_t<BridgedGroup<MetadbIoEditCallbackBridge>> g_metadb_io_edit_callback(
  guid_metadb_io_edit_callback, "metadb_io_edit_callback", EventNames{"on_edited"});

} // namespace event
