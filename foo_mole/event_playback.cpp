#include "stdafx.h"
#include "event.h"

namespace event {

class PlayCallbackBridge: public play_callback_static, public Bridge {
  unsigned get_flags() override { return flag_on_playback_all; }

  void on_playback_starting(play_control::t_track_command p_command,bool p_paused) override { OnEvent("starting"); }
  void on_playback_new_track(metadb_handle_ptr p_track) override { OnEvent("new_track"); }
  void on_playback_stop(play_control::t_stop_reason p_reason) override { OnEvent("stop"); }
  void on_playback_seek(double p_time) override { OnEvent("seek"); }
  void on_playback_pause(bool p_state) override { OnEvent("pause"); }
  void on_playback_edited(metadb_handle_ptr p_track) override { OnEvent("edited"); }
  void on_playback_dynamic_info(const file_info & p_info) override { OnEvent("dynamic_info"); }
  void on_playback_dynamic_info_track(const file_info & p_info) override { OnEvent("dynamic_info_track"); }
  void on_playback_time(double p_time) override { OnEvent("time"); }
  void on_volume_change(float p_new_val) override { OnEvent("volume_change"); }
};

static service_factory_single_t<PlayCallbackBridge> g_play_callback_bridge;

class Playback: public Group {
public:
  const char* GetName() override {
    return "playback";
  }

  void Schedule(const char* event, const char* commands) override {
    g_play_callback_bridge.get_static_instance().Schedule(event, commands);
  }
};

static service_factory_single_t<Playback> g_playback;

} // namespace event
