#include "stdafx.h"
#include "../acfu-sdk/utils/github.h"

class AcfuRequest: public acfu::github_latest_release<AcfuRequest>, public acfu::github_conf {
 public:
  static const char* get_owner() {
    return "3dyd";
  }

  static const char* get_repo() {
    return "mole";
  }
};

class AcfuSource: public acfu::source {
  virtual GUID get_guid() {
    static const GUID guid =
    {0x7ea07f1, 0x2ae1, 0x41c9, {0xb2, 0xec, 0x4a, 0xfa, 0x7d, 0x20, 0x17, 0xd7}};

    return guid;
  }

  virtual void get_info(file_info& info) {
    info.meta_set("version", APP_VERSION);
    info.meta_set("name", APP_NAME);
    info.meta_set("module", APP_BINARY);
  }

  virtual bool is_newer(const file_info& info) {
    const char* version = info.meta_get("version", 0);
    return acfu::is_newer(version, APP_VERSION);
  }

  virtual acfu::request::ptr create_request() {
    acfu::request::ptr request = new service_impl_t<AcfuRequest>();
    return request;
  }
};

static service_factory_t<AcfuSource> g_acfu_source;
