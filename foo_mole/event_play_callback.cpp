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

static service_factory_single_t<BridgedGroup<PlayCallbackBridge>> g_play_callback("play_callback",
  EventNames{"on_playback_starting", "on_playback_new_track", "on_playback_stop", "on_playback_seek",
             "on_playback_pause", "on_playback_edited", "on_playback_dynamic_info",
             "on_playback_dynamic_info_track", "on_playback_time", "on_volume_change"});

} // namespace event
