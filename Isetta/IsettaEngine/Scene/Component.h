/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <bitset>
#include <set>
#include <typeindex>
#include <unordered_map>
#include "ISETTA_API.h"

#define BEGIN_COMPONENT(NAME, BASE, UNIQUE)                    \
  template <bool Unique>                                       \
  class ISETTA_API_DECLARE                                     \
      Isetta::ComponentRegistry<class NAME, BASE, Unique> {    \
   protected:                                                  \
    static bool NAME##Registered;                              \
  };                                                           \
  class ISETTA_API_DECLARE NAME                                \
      : public BASE,                                           \
        public Isetta::ComponentRegistry<NAME, BASE, UNIQUE> { \
   protected:                                                  \
    static bool isRegistered() { return NAME##Registered; }    \
                                                               \
   private:

#define END_COMPONENT(NAME, BASE)                                        \
  }                                                                      \
  ;                                                                      \
  template <bool Unique>                                                 \
  bool Isetta::ComponentRegistry<NAME, BASE, Unique>::NAME##Registered = \
      Component::RegisterComponent(std::type_index(typeid(NAME)),        \
                                   std::type_index(typeid(BASE)), Unique);

namespace Isetta {

template <typename Curr, typename Base, bool Exclude>
struct ISETTA_API_DECLARE ComponentRegistry {};

class ISETTA_API Component {
  friend class Entity;

  std::bitset<7> attributes;

  static std::unordered_map<std::type_index, std::list<std::type_index>>&
  childrenTypes() {
    static std::unordered_map<std::type_index, std::list<std::type_index>>
        children{};
    return children;
  }

  static std::set<std::type_index>& excludeComponents() {
    static std::set<std::type_index> excludes{};
    return excludes;
  }

  static void FlattenComponentList();
  static void FlattenHelper(std::type_index parent, std::type_index curr);
  static bool isFlattened;

  inline static class Entity* curEntity{nullptr};
  inline static class Transform* curTransform{nullptr};

 protected:
  enum class ComponentAttributes {
    IS_ACTIVE,
    HAS_AWAKEN,
    NEED_DESTROY,
    NEED_GUI_UPDATE,
    NEED_UPDATE,
    NEED_LATE_UPDATE,
    NEED_FIXED_UPDATE,
  };

  Component();

  void SetAttribute(ComponentAttributes attr, bool value);

  bool GetAttribute(ComponentAttributes attr) const;

 public:
  virtual ~Component() = default;
  void SetActive(bool value);
  bool GetActive() const;
  class Transform* GetTransform() const;
  class Entity* const entity;
  class Transform* const transform;

  virtual void OnEnable() {}
  virtual void Awake() {}
  virtual void Start() {}
  virtual void GuiUpdate() {
    SetAttribute(ComponentAttributes::NEED_GUI_UPDATE, false);
  }
  virtual void Update() {
    SetAttribute(ComponentAttributes::NEED_UPDATE, false);
  }
  virtual void LateUpdate() {
    SetAttribute(ComponentAttributes::NEED_LATE_UPDATE, false);
  }
  virtual void FixedUpdate() {
    SetAttribute(ComponentAttributes::NEED_FIXED_UPDATE, false);
  }
  virtual void OnDestroy() {}
  virtual void OnDisable() {}

  static bool RegisterComponent(std::type_index curr, std::type_index base,
                                bool isExclude);
};

}  // namespace Isetta