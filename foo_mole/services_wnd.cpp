#include "stdafx.h"
#include "resource.h"
#include "services_wnd.h"
#include "../foobar2000/helpers/window_placement_helper.h"

// {9ECAF194-8870-4B9D-B2C3-F311EBABA14B}
static const GUID guid_cfg_services_wnd_placement =
{0x9ecaf194, 0x8870, 0x4b9d, {0xb2, 0xc3, 0xf3, 0x11, 0xeb, 0xab, 0xa1, 0x4b}};
static cfg_window_placement cfg_services_wnd_placement(guid_cfg_services_wnd_placement);

void ServicesWnd::OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl) {
  DestroyWindow();
}

void ServicesWnd::OnDestroy() {
  cfg_services_wnd_placement.on_window_destruction(m_hWnd);
  modeless_dialog_manager::g_remove(m_hWnd);

  SetMsgHandled(FALSE);
}

BOOL ServicesWnd::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
  modeless_dialog_manager::g_add(m_hWnd);

  if (!cfg_services_wnd_placement.on_window_creation(m_hWnd, true)) {
    CenterWindow();
  }

  ShowWindow(SW_SHOW);

  SetMsgHandled(FALSE);
  return TRUE;
}

void ServicesWnd::OnFinalMessage(HWND) {
  delete this;
}

void ServicesWnd::Show() {
  if (!core_api::is_quiet_mode_enabled()) {
    ServicesWnd* wnd = new ServicesWnd();
    if (!wnd->Create(core_api::get_main_window())) {
      wnd->OnFinalMessage(nullptr);
    }
  }
}
