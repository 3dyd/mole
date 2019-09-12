#pragma once

#include "service_scanner.h"

class ServiceWnd: public CDialogImpl<ServiceWnd>, public CDialogResize<ServiceWnd> {
public:
  enum {IDD = IDD_SERVICES};

  BEGIN_DLGRESIZE_MAP(ServiceWnd)
    DLGRESIZE_CONTROL(IDC_SERVICE_TREE, DLSZ_SIZE_X | DLSZ_SIZE_Y)
  END_DLGRESIZE_MAP()

  BEGIN_MSG_MAP_EX(ServiceWnd)
    MSG_WM_DESTROY(OnDestroy)
    MSG_WM_INITDIALOG(OnInitDialog)
    COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
    CHAIN_MSG_MAP(CDialogResize<ServiceWnd>)
  END_MSG_MAP()

  static void Show();

private:
  ~ServiceWnd() {}

  void OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl);
  void OnDestroy();
  BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);

  void OnFinalMessage(HWND) override;
  void UpdateTree();

private:
  ServiceScanner scanner_;
};
