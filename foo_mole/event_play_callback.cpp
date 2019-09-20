#include "stdafx.h"
#include "event.h"

namespace event {

class PlayCallbackBridge: public play_callback_static, public Bridge {
public:
  using Bridge::Bridge;

  unsigned get_flags() override {
    return flag_on_playback_all;
  }

  BRIDGE_EVENT(on_playback_starting, play_control::t_track_command, bool);
  BRIDGE_EVENT(on_playback_new_track, metadb_handle_ptr);
  BRIDGE_EVENT(on_playback_stop, play_control::t_stop_reason);
  BRIDGE_EVENT(on_playback_seek, double);
  BRIDGE_EVENT(on_playback_pause, bool);
  BRIDGE_EVENT(on_playback_edited, metadb_handle_ptr);
  BRIDGE_EVENT(on_playback_dynamic_info, const file_info&);
  BRIDGE_EVENT(on_playback_dynamic_info_track, const file_info&);
  BRIDGE_EVENT(on_playback_time, double);
  BRIDGE_EVENT(on_volume_change, float);
};

// {EC9E23A7-D26E-47AB-8854-03573F27F066}
static const GUID guid_play_callback =
{0xec9e23a7, 0xd26e, 0x47ab, {0x88, 0x54, 0x3, 0x57, 0x3f, 0x27, 0xf0, 0x66}};

static service_factory_single_t<BridgedGroup<PlayCallbackBridge>> g_play_callback(
  guid_play_callback, "play_callback",
  EventNames{"on_playback_starting", "on_playback_new_track", "on_playback_stop", "on_playback_seek",
             "on_playback_pause", "on_playback_edited", "on_playback_dynamic_info",
             "on_playback_dynamic_info_track", "on_playback_time", "on_volume_change"});

} // namespace event
