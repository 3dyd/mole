#include "stdafx.h"
#include "resource.h"
#include "service_wnd.h"
#include "../foobar2000/helpers/window_placement_helper.h"

// {9ECAF194-8870-4B9D-B2C3-F311EBABA14B}
static const GUID guid_cfg_services_wnd_placement =
{0x9ecaf194, 0x8870, 0x4b9d, {0xb2, 0xc3, 0xf3, 0x11, 0xeb, 0xab, 0xa1, 0x4b}};
static cfg_window_placement cfg_services_wnd_placement(guid_cfg_services_wnd_placement);

void ServiceWnd::OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl) {
  DestroyWindow();
}

void ServiceWnd::OnDestroy() {
  cfg_services_wnd_placement.on_window_destruction(m_hWnd);
  modeless_dialog_manager::g_remove(m_hWnd);

  SetMsgHandled(FALSE);
}

BOOL ServiceWnd::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
  modeless_dialog_manager::g_add(m_hWnd);
  DlgResize_Init();

  if (!cfg_services_wnd_placement.on_window_creation(m_hWnd, true)) {
    CenterWindow();
  }

  UpdateTree();

  ShowWindow(SW_SHOW);

  SetMsgHandled(FALSE);
  return TRUE;
}

void ServiceWnd::OnFinalMessage(HWND) {
  delete this;
}

void ServiceWnd::Show() {
  if (!core_api::is_quiet_mode_enabled()) {
    ServiceWnd* wnd = new ServiceWnd();
    if (!wnd->Create(core_api::get_main_window())) {
      wnd->OnFinalMessage(nullptr);
    }
  }
}

void ServiceWnd::UpdateTree() {
  CTreeViewCtrl tree(GetDlgItem(IDC_SERVICE_TREE));
  tree.SetRedraw(FALSE);
  tree.DeleteAllItems();

  auto by_origin = scanner_.ByOrigin();

  for (const auto& origin : scanner_.GetOrigins()) {
    const auto& instances = by_origin[&origin];

    auto name = origin.module + " (" + std::to_string(instances.size()) + ")";
    HTREEITEM parent = tree.InsertItem(pfc::stringcvt::string_os_from_utf8(name.c_str()), TVI_ROOT, TVI_LAST);

    for (auto it = instances.begin(); instances.end() != it;) {
      auto it_last = instances.upper_bound(it->first);
      auto count = std::distance(it, it_last);

      auto name = it->first.name.empty() ? it->first.guid_str : it->first.name;
      if (1 != count) {
        name.append(" (").append(std::to_string(count)).append(")");
      }

      tree.InsertItem(pfc::stringcvt::string_os_from_utf8(name.c_str()), parent, TVI_LAST);

      it = it_last;
    }
  }

  tree.SetRedraw(TRUE);
}
