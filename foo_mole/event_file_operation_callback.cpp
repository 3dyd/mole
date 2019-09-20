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

// {643DFAD6-97EC-42E0-B923-9C89206AB5AB}
static const GUID guid_file_operation_callback =
{0x643dfad6, 0x97ec, 0x42e0, {0xb9, 0x23, 0x9c, 0x89, 0x20, 0x6a, 0xb5, 0xab}};

static service_factory_single_t<BridgedGroup<FileOperationCallbackBridge>> g_file_operation_callback(
  guid_file_operation_callback, "file_operation_callback",
  EventNames{"on_files_deleted_sorted", "on_files_moved_sorted", "on_files_copied_sorted"});

} // namespace event
