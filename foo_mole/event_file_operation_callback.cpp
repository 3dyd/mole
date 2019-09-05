#include "stdafx.h"
#include "event.h"

namespace event {

class FileOperationCallbackBridge: public file_operation_callback, public Bridge {
  BRIDGE_EVENT(on_files_deleted_sorted, t_pathlist);
  BRIDGE_EVENT(on_files_moved_sorted, t_pathlist, t_pathlist);
  BRIDGE_EVENT(on_files_copied_sorted, t_pathlist, t_pathlist);
};

static service_factory_single_t<FileOperationCallbackBridge> g_file_operation_callback_bridge;

static service_factory_single_t<GroupFromBridge> g_file_operation_callback(
  "file_operation_callback", g_file_operation_callback_bridge.get_static_instance());

} // namespace event
