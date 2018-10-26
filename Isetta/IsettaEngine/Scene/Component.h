/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <bitset>
#include <typeindex>
#include <unordered_map>
#include "ISETTA_API.h"

#define CREATE_COMPONENT_BEGIN(NAME, BASE)                                     \
  template <typename Dummy>                                                    \
  class ISETTA_API_DECLARE ComponentRegistry<class NAME, BASE, Dummy> {        \
   protected:                                                                  \
    static bool NAME##Registered;                                              \
  };                                                                           \
  class ISETTA_API_DECLARE NAME : public BASE,                                 \
                                  public ComponentRegistry<NAME, BASE, void> { \
   protected:                                                                  \
    static bool isRegistered() { return NAME##Registered; }                    \
                                                                               \
   private:

#define CREATE_COMPONENT_END(NAME, BASE)                          \
  }                                                               \
  ;                                                               \
  template <typename Dummy>                                       \
  bool ComponentRegistry<NAME, BASE, Dummy>::NAME##Registered =   \
      Component::RegisterComponent(std::type_index(typeid(NAME)), \
                                   std::type_index(typeid(BASE)));

namespace Isetta {

template <typename Curr, typename Base, typename Dummy>
struct ISETTA_API_DECLARE ComponentRegistry {};

class ISETTA_API_DECLARE Component {
  friend class Entity;

  std::bitset<4> attributes;

  static std::unordered_map<std::type_index, std::list<std::type_index>>&
  childrenTypes() {
    static std::unordered_map<std::type_index, std::list<std::type_index>>
        children{};
    return children;
  }

 protected:
  class Entity* entity;

  enum class ComponentAttributes {
    IS_ACTIVE,
    HAS_STARTED,
    NEED_DESTROY,
    NEED_UPDATE
  };

  Component();

  void SetAttribute(ComponentAttributes attr, bool value);

  bool GetAttribute(ComponentAttributes attr) const;

 public:
  virtual ~Component() = default;
  void SetActive(bool value);
  bool GetActive() const;
  class Transform* GetTransform() const;
  class Entity* GetEntity() const;

  virtual void OnEnable() {}
  virtual void Start() {}
  virtual void GuiUpdate() {}
  virtual void Update() {}
  virtual void LateUpdate() {}
  virtual void FixedUpdate() {}
  virtual void OnDestroy() {}
  virtual void OnDisable() {}

  static bool RegisterComponent(std::type_index curr, std::type_index base);
};

}  // namespace Isetta