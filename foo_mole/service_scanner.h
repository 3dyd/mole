#pragma once

class ServiceScanner {
public:
  struct Origin {
    std::string module;
    t_uint32 fb2k_target_version = 0;

    Origin(const std::string& module): module(module) {}

    bool operator <(const Origin& r) const { return std::lexicographical_compare(module.begin(), module.end(), r.module.begin(), r.module.end()); }
  };

  struct Service {
    std::string name;
    GUID guid;
    std::string guid_str;

    bool operator <(const Service& r) const {
      if (!name.empty() && !r.name.empty()) {
        return std::lexicographical_compare(name.begin(), name.end(), r.name.begin(), r.name.end());
      }
      if (name.empty() && !r.name.empty()) {
        return true;
      }
      if (!name.empty() && r.name.empty()) {
        return false;
      }
      return guid_str < r.guid_str;
    }
  };

  using Instance = std::tuple<const Origin&, const Service&, service_factory_base&>;

  ServiceScanner();

  const std::list<Origin>& GetOrigins() const { return origins_; }
  const std::list<Service>& GetServices() const { return services_; }
  const std::vector<Instance> GetInstances() const { return instances_; }

  using OriginMap = std::map<const Origin*, std::multimap<Service, service_factory_base&>>;
  OriginMap ByOrigin() const;

private:
  struct Module {
    HMODULE handle = nullptr;
    MODULEINFO info = {0};
    std::string name;
  };

  const Origin& EstablishOrigin(service_factory_base* service, const std::vector<Module>& modules);
  const Service& EstablishService(const GUID& guid);
  void FillTargetVersion(const std::vector<Module>& modules);
  service_factory_base* FindInitialFactory() const;
  int GetChainSize(service_factory_base* service) const;
  std::string GetModuleName(HMODULE module) const;
  std::vector<Module> GetModules() const;

private:
  std::list<Origin> origins_;
  std::list<Service> services_;
  std::vector<Instance> instances_;
};
