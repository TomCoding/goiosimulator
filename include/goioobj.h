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

#include "./constants.h"


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

#define REGISTER_TYPE_FULL(T, STR) \
  template<> \
  ObjectFactory::CreatorFull_t* ObjectFactory::Register<T>::creator_full = \
                            ObjectFactory::Register<T>::init_creator_full(STR)
#define REGISTER_TYPE_FULLREP(T, STR) \
  template<> \
  ObjectFactory::CreatorFullRep_t* ObjectFactory::Register<T>::creator_fullrep = \
                            ObjectFactory::Register<T>::init_creator_fullrep(STR)
#define REGISTER_TYPE(T, STR) \
  template<> \
  ObjectFactory::Creator_t* ObjectFactory::Register<T>::creator = \
                            ObjectFactory::Register<T>::init_creator(STR)
#define REGISTER_TYPE_SHORT(T, STR) \
  template<> \
  ObjectFactory::CreatorShort_t* ObjectFactory::Register<T>::creator_short = \
                            ObjectFactory::Register<T>::init_creator_short(STR)


class GoioObj : public Object {
 private:
    const std::string name;
    const CmpType cmp_type;
    Health max_health;
    Health health;
    int fire_stacks;
    int rebuild_state;
    const double part_type_multiplier;
    GoioObj* hull;
    Time cooldown_end;
    Time immunity_end;

    bool temporary_immunity;

    bool connected;

    explicit GoioObj(Health max_health) : name(""), cmp_type(CmpType::HULL),
                max_health(max_health), health(max_health),
                fire_stacks(-1), rebuild_state(-1),
                part_type_multiplier(-1),
                hull(nullptr), cooldown_end(0), immunity_end(0),
                temporary_immunity(false), connected(false) {}
    GoioObj(const GoioObj& obj);
    GoioObj& operator=(const GoioObj& obj);

    bool set_health_int(Health health, GoioObj* obj);

 public:
    GoioObj(const std::string& name, CmpType cmp_type,
            double part_type_multiplier = -1,
            Health max_health = 0_hp, Health hull_max_health = -1_hp) :
            name(name), cmp_type(cmp_type), max_health(max_health),
            health(max_health), fire_stacks(-1), rebuild_state(-1),
            part_type_multiplier(part_type_multiplier),
            hull(new GoioObj(hull_max_health)),
            cooldown_end(0_s), immunity_end(0_s), temporary_immunity(false),
            connected(false) {}
    virtual ~GoioObj();

    static const     int    max_fire_stacks           = 20;
    static const     int    fire_stacks_unusable      =  8;

    static const     int    rebuild_base_hits         =  9;
    static constexpr double rebuild_health_multiplier =  0.05;
    static constexpr double health_after_rebuild      =  0.333333333333;

    inline int get_rebuild_value() const {
      return std::round((rebuild_base_hits +
                         max_health *
                         rebuild_health_multiplier/1_hp) *
                        part_type_multiplier);
    }

    inline const std::string get_name() const { return name; }
    inline CmpType get_cmp_type() const { return cmp_type; }
    inline Health  get_max_health() const { return max_health; }
    inline Health  get_health() const { return health; }
    inline int     get_fire_stacks() const { return fire_stacks; }
    inline double  get_rebuild_state() const { return rebuild_state; }
    inline GoioObj* get_hull() const { return hull; }
    inline Time    get_cooldown_end() const { return cooldown_end; }
    inline Time    get_immunity_end() const { return immunity_end; }
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

    void set_health(Health health);
    void set_hull_health(Health health);
    void set_fire(int fire);

    inline void reset_cooldown() { cooldown_end = 0_s; }
    inline void set_temporary_immunity(bool temporary_immunity) {
      this->temporary_immunity = temporary_immunity;
    }

    // Set obj as underlying hull component.
    void set_hull(GoioObj* obj);

    virtual void reset(bool hull = true);
};

typedef std::function<Time ()> TimeFunc;
typedef std::function<TimeFunc (const GoioObj*, Time, bool&)> TimeCheckFunc;

typedef std::function<DmgState::State (GoioObj*, Time)> TimeDmgFunc;

}  // namespace goio

#endif  // GOIOOBJ_H_
