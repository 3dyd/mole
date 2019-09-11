#pragma once

class ServicesWnd: public CDialogImpl<ServicesWnd> {
public:
  enum {IDD = IDD_SERVICES};

  BEGIN_MSG_MAP_EX(ServicesWnd)
    MSG_WM_DESTROY(OnDestroy)
    MSG_WM_INITDIALOG(OnInitDialog)
    COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
  END_MSG_MAP()

  static void Show();

private:
  ~ServicesWnd() {}

  void OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl);
  void OnDestroy();
  BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
  void OnFinalMessage(HWND) override;
};
