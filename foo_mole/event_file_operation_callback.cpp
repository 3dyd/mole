#include "stdafx.h"
#include "event.h"

namespace event {

class FileOperationCallbackBridge: public file_operation_callback, public Bridge {
public:
  using Bridge::Bridge;

  BRIDGE_EVENT(on_files_deleted_sorted, t_pathlist);
  BRIDGE_EVENT(on_files_moved_sorted, t_pathlist, t_pathlist);
  BRIDGE_EVENT(on_files_copied_sorted, t_pathlist, t_pathlist);
};

static service_factory_single_t<BridgedGroup<FileOperationCallbackBridge>> g_file_operation_callback(
  "file_operation_callback", EventNames{"on_files_deleted_sorted", "on_files_moved_sorted", "on_files_copied_sorted"});

} // namespace event
