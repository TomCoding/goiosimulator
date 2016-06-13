/**
 * goiovalues - Guns of Icarus Online damage simulator
 * Copyright (C) 2016  Dominique Lasserre
 *
 * This file is part of goiovalues.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GOIOOBJ_H_
#define GOIOOBJ_H_

#include <functional>
#include <cmath>
#include <string>
#include <map>
#include <cassert>
#include <set>

#include "./constants.h"
#include "./utils.h"
#include "./tool.h"


namespace goio {

// Object registration from http://stackoverflow.com/a/6235500/770468
struct Object {
  virtual ~Object() {}
};

struct ObjectFactory {
 private:
    // full
    typedef Object* CreatorFull_t(const std::string&, CmpType,
                                  double, Health, Health);
    typedef std::map<std::string, CreatorFull_t*> CreatorsFull_t;

    static CreatorsFull_t& static_creators_full() {
      static CreatorsFull_t s_creators_full;
      return s_creators_full;
    }

    // fullrep
    typedef Object* CreatorFullRep_t(const std::string&, bool,
                                     RepairMode, ExtinguishMode);
    typedef std::map<std::string, CreatorFullRep_t*> CreatorsFullRep_t;

    static CreatorsFullRep_t& static_creators_fullrep() {
      static CreatorsFullRep_t s_creators_fullrep;
      return s_creators_fullrep;
    }

    // normal
    typedef Object* Creator_t(const std::string&);
    typedef std::map<std::string, Creator_t*> Creators_t;

    static Creators_t& static_creators() {
      static Creators_t s_creators;
      return s_creators;
    }

    // short
    typedef Object* CreatorShort_t();
    typedef std::map<std::string, CreatorShort_t*> CreatorsShort_t;

    static CreatorsShort_t& static_creators_short() {
      static CreatorsShort_t s_creators_short;
      return s_creators_short;
    }

    template<class T = int>
    struct Register {
      // full
      static Object* create_full(const std::string& name,
                                 CmpType cmp_type,
                                 double part_type_multiplier,
                                 Health max_health,
                                 Health hull_max_health) {
        return new T(name, cmp_type, part_type_multiplier, max_health,
                     hull_max_health);
      }
      static CreatorFull_t* init_creator_full(const std::string& id) {
        return static_creators_full()[id] = create_full;
      }
      static CreatorFull_t* creator_full;

      // fullrep
      static Object* create_fullrep(const std::string& name,
                                    bool extinguisher,
                                    RepairMode mode,
                                    ExtinguishMode extmode) {
        return new T(name, extinguisher, mode, extmode);
      }
      static CreatorFullRep_t* init_creator_fullrep(const std::string& id) {
        return static_creators_fullrep()[id] = create_fullrep;
      }
      static CreatorFullRep_t* creator_fullrep;

      // normal
      static Object* create(const std::string& name) {
        return new T(name);
      }
      static Creator_t* init_creator(const std::string& id) {
        return static_creators()[id] = create;
      }
      static Creator_t* creator;

      // short
      static Object* create_short() {
        return new T();
      }
      static CreatorShort_t* init_creator_short(const std::string& id) {
        return static_creators_short()[id] = create_short;
      }
      static CreatorShort_t* creator_short;
    };

 public:
    static Object* create(const std::string& id,
                          const std::string& name,
                          CmpType cmp_type,
                          double part_type_multiplier,
                          Health max_health,
                          Health hull_max_health) {
      const auto iter = static_creators_full().find(id);
      if (iter == static_creators_full().end())
        return nullptr;
      else
        return (*iter->second)(name, cmp_type, part_type_multiplier,
                               max_health, hull_max_health);
    }

    static Object* create(const std::string& id,
                          const std::string& name,
                          bool extinguisher,
                          RepairMode mode,
                          ExtinguishMode extmode) {
      const auto iter = static_creators_fullrep().find(id);
      if (iter == static_creators_fullrep().end())
        return nullptr;
      else
        return (*iter->second)(name, extinguisher, mode, extmode);
    }

    static Object* create(const std::string& id, const std::string& name) {
      const auto iter = static_creators().find(id);
      if (iter == static_creators().end())
        return nullptr;
      else
        return (*iter->second)(name);
    }

    static Object* create(const std::string& id) {
      const auto iter = static_creators_short().find(id);
      if (iter == static_creators_short().end())
        return nullptr;
      else
        return (*iter->second)();
    }
};

#define REGISTER_TYPE_FULL(T) \
  template<> \
  ObjectFactory::CreatorFull_t* ObjectFactory::Register<T>::creator_full = \
                            ObjectFactory::Register<T>::init_creator_full(STR(T))
#define REGISTER_TYPE_FULLREP(T) \
  template<> \
  ObjectFactory::CreatorFullRep_t* ObjectFactory::Register<T>::creator_fullrep = \
                            ObjectFactory::Register<T>::init_creator_fullrep(STR(T))
#define REGISTER_TYPE(T) \
  template<> \
  ObjectFactory::Creator_t* ObjectFactory::Register<T>::creator = \
                            ObjectFactory::Register<T>::init_creator(STR(T))
#define REGISTER_TYPE_SHORT(T) \
  template<> \
  ObjectFactory::CreatorShort_t* ObjectFactory::Register<T>::creator_short = \
                            ObjectFactory::Register<T>::init_creator_short(STR(T))


class GoioObj : public Object {
 private:
    const std::string name;
    const CmpType cmp_type;
    Health max_health;
    Health health;
    int fire_stacks;
    int rebuild_state;
    int buff_state;
    const double part_type_multiplier;
    GoioObj* hull;
    Time cooldown_end;
    Time immunity_end;
    Time buff_end;
    const Tool* buff_tool;

    bool temporary_immunity;

    bool connected;

    std::set<const Tool*> active_tools;

    GoioObj(const GoioObj& obj) = delete;
    GoioObj& operator=(const GoioObj& obj) = delete;

    bool set_health_int(Health health);

    class Hull;

    GoioObj(Health max_health) :
                name(""), cmp_type(CmpType::HULL),
                max_health(max_health), health(max_health),
                fire_stacks(-1), rebuild_state(-1), buff_state(0),
                part_type_multiplier(-1),
                hull(nullptr),
                cooldown_end(0_s), immunity_end(0_s), buff_end(0_s),
                buff_tool(nullptr),
                temporary_immunity(false), connected(false), active_tools() {}

 protected:
    virtual void accept(ToolDispatcher& dispatcher,
                        const Tool* tool,
                        bool activate) = 0;

 public:
    GoioObj(const std::string& name, CmpType cmp_type,
            double part_type_multiplier = -1,
            Health max_health = 0_hp, Health hull_max_health = -1_hp);
    virtual ~GoioObj();

    static const     int    max_fire_stacks           = 20;
    static const     int    fire_stacks_unusable      =  8;

    static const     int    rebuild_base_hits         =  9;
    static constexpr double rebuild_health_multiplier =  0.05;
    static constexpr double health_after_rebuild      =  0.333333333333;

    inline virtual int get_rebuild_value() const {
      return std::round((rebuild_base_hits +
                         max_health *
                         rebuild_health_multiplier/1_hp) *
                        part_type_multiplier);
    }

    virtual int get_buff_value() const = 0;

    inline const std::string get_name() const { return name; }
    inline CmpType get_cmp_type() const { return cmp_type; }
    inline Health  get_max_health() const { return max_health; }
    inline Health  get_health() const { return health; }
    inline int     get_fire_stacks() const { return fire_stacks; }
    inline double  get_rebuild_state() const { return rebuild_state; }
    inline int     get_buff_state() const { return buff_state; }
    inline GoioObj* get_hull() const { return hull; }
    inline Time    get_cooldown_end() const { return cooldown_end; }
    inline Time    get_immunity_end() const { return immunity_end; }
    inline Time    get_buff_end() const { return buff_end; }
    inline bool    get_temporary_immunity() const { return temporary_immunity; }

    static inline int get_max_fire_stacks() { return max_fire_stacks; }
    static inline int get_fire_stacks_unusable() { return fire_stacks_unusable; }

    /*
     * Return `false` if object gets destroyed, otherwise `true`
     */
    bool add_health(Health health, Time cooldown_end = -1_s);
    bool add_fire(int fire, Time immunity_end = -1_s,
                            Time cooldown_end = -1_s);
    bool add_rebuild(int rebuild_progress);
    bool add_buff(int buff_progress, Time buff_end = -1_s,
                  const Tool* tool = nullptr);

    void set_health(Health health);
    void set_hull_health(Health health);
    void set_fire(int fire);
    void set_buff(int buff);

    inline void reset_cooldown() { cooldown_end = 0_s; }
    inline void set_temporary_immunity(bool temporary_immunity) {
      this->temporary_immunity = temporary_immunity;
    }

    // Set obj as underlying hull component.
    void set_hull(GoioObj* obj);

    bool dead() const;
    inline bool buffed() const {
      return buff_tool != nullptr;
    }

    virtual void reset(bool hull = true);
    virtual void reset_modifiers()  = 0;

    void apply_tool(const Tool* tool);
    void remove_tool(const Tool* tool);

    void remove_buff();
};

class GoioObj::Hull : public GoioObj {
 protected:
    void accept(ToolDispatcher&, const Tool*, bool) override {
      assert(false);
    }

 public:
    explicit Hull(Health max_health) : GoioObj(max_health) {}

    int get_buff_value() const override { return -1; }
    void reset_modifiers() override {}
};

class FreeObject : public GoioObj {
 protected:
    void accept(ToolDispatcher&, const Tool*, bool) override {
      assert(false);
    }

 public:
    FreeObject(const std::string& name, CmpType cmp_type,
                double part_type_multiplier = -1,
                Health max_health = 0_hp, Health hull_max_health = -1_hp) :
                GoioObj(name, cmp_type, part_type_multiplier,
                        max_health, hull_max_health) {}

    int get_buff_value() const override {
      switch (get_cmp_type()) {
        case CmpType::COMPONENTS:
          return -1;
        case CmpType::ENGINES:
          return 4;
        case CmpType::GUNS:
          return 9;
        case CmpType::ARMOR:
          return 15;
        case CmpType::BALLOON:
          return 10;
        case CmpType::HULL:
          return -1;
        default:
          assert(false);
      }
    }

    void reset_modifiers() override {}
};

// class StaticObject : public GoioObj {
//  public:
//     StaticObject(const std::string& name, CmpType cmp_type,
//                  double part_type_multiplier = -1,
//                  Health max_health = 0_hp, Health hull_max_health = -1_hp) :
//                  GoioObj(name, cmp_type, part_type_multiplier,
//                          max_health, hull_max_health) {}
//
//     int get_buff_value() const override { return -1; }
//     void reset_modifiers() override {}
// };


typedef std::function<Time ()> TimeFunc;
typedef std::function<TimeFunc (const GoioObj*, Time, bool&)> TimeCheckFunc;

typedef std::function<DmgState (GoioObj*, Time)> TimeDmgFunc;

}  // namespace goio

#endif  // GOIOOBJ_H_
