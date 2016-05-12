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

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <iostream>

#include "./dmg_types.h"
#include "./ai_behavior.h"


namespace goio {

namespace DmgState {
enum State : uint32_t {
  NONE              = 0,            // nothing changed

  TARGET            =  1ull <<  0,  // damaged/healed target
  REBUILD           =  1ull <<  1,  // rebuild progress on target
  IMMUNITY          =  1ull <<  2,  // changed fire immunity on target
  FIRE              =  1ull <<  3,  // changed fire stacks on target
  TRANSITIONED      =  1ull <<  4,  // destroyed/rebuild armor/hull on target
  START_TARGET      = (1ull <<  5) + (1ull <<  0),
  START_REBUILD     = (1ull <<  6) + (1ull <<  1),
  START_IMMUNITY    = (1ull <<  7) + (1ull <<  2),
  START_FIRE        = (1ull <<  8) + (1ull <<  3),

  START_TARGET_O    =  1ull <<  5,
  START_REBUILD_O   =  1ull <<  6,
  START_IMMUNITY_O  =  1ull <<  7,
  START_FIRE_O      =  1ull <<  8,

  TARGET_ALL        = (1ull <<  0) +
                      (1ull <<  1) +
                      (1ull <<  2) +
                      (1ull <<  3) +
                      (1ull <<  4) +
                      (1ull <<  5) +
                      (1ull <<  6) +
                      (1ull <<  7) +
                      (1ull <<  8),

  SELF              =  1ull << 16,  // damaged/healed self
  REBUILD_S         =  1ull << 17,  // rebuild progress on self
  IMMUNITY_S        =  1ull << 18,  // changed fire immunity on self
  FIRE_S            =  1ull << 19,  // changed fire stacks on self
  TRANSITIONED_S    =  1ull << 20,  // destroyed/rebuild armor/hull on self
  START_SELF        = (1ull << 21) + (1ull << 16),
  START_REBUILD_S   = (1ull << 22) + (1ull << 17),
  START_IMMUNITY_S  = (1ull << 23) + (1ull << 18),
  START_FIRE_S      = (1ull << 24) + (1ull << 19),

  START_TARGET_SO   =  1ull << 21,
  START_REBUILD_SO  =  1ull << 22,
  START_IMMUNITY_SO =  1ull << 23,
  START_FIRE_SO     =  1ull << 24,

  SELF_ALL          = (1ull << 16) +
                      (1ull << 17) +
                      (1ull << 18) +
                      (1ull << 19) +
                      (1ull << 20) +
                      (1ull << 21) +
                      (1ull << 22) +
                      (1ull << 23) +
                      (1ull << 24),

  ALL               = 0xffffffff
};

inline constexpr State operator|(State a, State b) {
  return static_cast<State>(
                static_cast<std::underlying_type_t<State>>(a) |
                static_cast<std::underlying_type_t<State>>(b));
}
inline constexpr State operator&(State a, State b) {
  return static_cast<State>(
                static_cast<std::underlying_type_t<State>>(a) &
                static_cast<std::underlying_type_t<State>>(b));
}
inline
#ifndef GCC_4_9
       constexpr
#endif
                 State& operator|=(State& a, State b) {
  a = a | b;
  return a;
}
}  // namespace DmgState

inline DmgState::State get_dmg_state_self_reversed(DmgState::State dmg_state) {
  return static_cast<DmgState::State>(
                static_cast<std::underlying_type_t<DmgState::State>>(dmg_state)
                >> 16);
}


#ifdef STRONG_TYPE_COMPILE
template<int M, int K, int S, int H, int A> struct Unit {
  enum { m = M, kg = K, s = S, h = H, a = A };
};

template<typename Unit>
struct Value {
  double val;
  constexpr Value() : val() {}
  constexpr explicit Value(double d) : val(d) {}
  constexpr Value(const Value& obj) : val(obj.val) {}
#ifndef GCC_4_9
  constexpr
#endif
            Value& operator=(const Value& obj) { val = obj.val; return *this; }

  constexpr Value operator+(const Value& obj) const { return Value(val + obj.val); }
  constexpr Value operator-(const Value& obj) const { return Value(val - obj.val); }
  constexpr bool operator==(const Value& obj) const { return val == obj.val; }
  constexpr bool operator!=(const Value& obj) const { return val != obj.val; }
  constexpr bool operator>(const Value& obj) const { return val > obj.val; }
  constexpr bool operator<(const Value& obj) const { return val < obj.val; }
  constexpr bool operator>=(const Value& obj) const { return val >= obj.val; }
  constexpr bool operator<=(const Value& obj) const { return val <= obj.val; }
  constexpr Value operator-() const { return Value(-val); }
  constexpr Value operator*(double d) const { return Value(val * d); }
  constexpr Value operator*(int i) const { return Value(val * i); }
  constexpr Value operator/(double d) const { return Value(val / d); }
  constexpr Value operator/(int d) const { return Value(val / d); }

  operator double();

  friend std::ostream& operator<<(std::ostream& os, const Value& obj) {
    os << obj.val;
    return os;
  }
};

template<>
inline Value<Unit<0, 0, 0, 0, 0>>::operator double() { return val; }

using Distance      = Value<Unit<1, 0,  0, 0, 0>>;
using Time          = Value<Unit<0, 0,  1, 0, 0>>;
using P_Time        = Value<Unit<0, 0, -1, 0, 0>>;
using Health        = Value<Unit<0, 0,  0, 1, 0>>;
using Speed         = Value<Unit<1, 0, -1, 0, 0>>;
using Acceleration  = Value<Unit<1, 0, -2, 0, 0>>;
using Weight        = Value<Unit<0, 1,  0, 0, 0>>;
using Force         = Value<Unit<1, 1, -2, 0, 0>>;
using Angle         = Value<Unit<0, 0,  0, 0, 1>>;
using Angular_Speed = Value<Unit<0, 0, -1, 0, 1>>;
using Angular_Acceleration = Value<Unit<0, 0, -2, 0, 1>>;

// a f-p literal suffixed by '_s'
constexpr Time operator"" _s(long double d) {
  return Time(d);
}
// an integral literal suffixed by'_s'
constexpr Time operator"" _s(unsigned long long d) {
  return Time(d);
}
constexpr Time operator"" _min(long double d) {
  return Time(d*60);
}
constexpr Time operator"" _min(unsigned long long d) {
  return Time(d*60);
}
constexpr Distance operator"" _m(long double d) {
  return Distance(d);
}
constexpr Distance operator"" _m(unsigned long long d) {
  return Distance(d);
}
constexpr Health operator"" _hp(long double d) {
  return Health(d);
}
constexpr Health operator"" _hp(unsigned long long d) {
  return Health(d);
}
constexpr Acceleration operator"" _m_s2(long double d) {
  return Acceleration(d);
}
constexpr Acceleration operator"" _m_s2(unsigned long long d) {
  return Acceleration(d);
}
constexpr Weight operator"" _kg(long double d) {
  return Weight(d);
}
constexpr Weight operator"" _kg(unsigned long long d) {
  return Weight(d);
}
constexpr Force operator"" _N(long double d) {
  return Force(d);
}
constexpr Force operator"" _N(unsigned long long d) {
  return Force(d);
}
constexpr Angle operator"" _deg(long double d) {
  return Angle(d);
}
constexpr Angle operator"" _deg(unsigned long long d) {
  return Angle(d);
}
constexpr Angular_Acceleration operator"" _deg_s2(long double d) {
  return Angular_Acceleration(d);
}
constexpr Angular_Acceleration operator"" _deg_s2(unsigned long long d) {
  return Angular_Acceleration(d);
}

template<int m1, int k1, int s1, int h1, int a1,
         int m2, int k2, int s2, int h2, int a2>
constexpr Value<Unit<m1 - m2, k1 - k2, s1 - s2, h1 - h2, a1 - a2>> operator/
                                      (Value<Unit<m1, k1, s1, h1, a1>> a,
                                       Value<Unit<m2, k2, s2, h2, a2>> b) {
  return Value<Unit<m1 - m2,
                    k1 - k2,
                    s1 - s2,
                    h1 - h2,
                    a1 - a2>>(a.val / b.val);
}
template<int m1, int k1, int s1, int h1, int a1,
         int m2, int k2, int s2, int h2, int a2>
constexpr Value<Unit<m1 + m2, k1 + k2, s1 + s2, h1 + h2, a1 + a2>> operator*
                                      (Value<Unit<m1, k1, s1, h1, a1>> a,
                                       Value<Unit<m2, k2, s2, h2, a2>> b) {
  return Value<Unit<m1 + m2,
                    k1 + k2,
                    s1 + s2,
                    h1 + h2,
                    a1 + a2>>(a.val * b.val);
}

template<int m, int k, int s, int h, int a>
constexpr Value<Unit<m, k, s, h, a>> operator*(double d,
                                      const Value<Unit<m, k, s, h, a>>& obj) {
  return Value<Unit<m, k, s, h, a>>(d * obj.val);
}
template<int m, int k, int s, int h, int a>
constexpr Value<Unit<m, k, s, h, a>> operator*(int i,
                                      const Value<Unit<m, k, s, h, a>>& obj) {
  return Value<Unit<m, k, s, h, a>>(i * obj.val);
}

template<int m, int k, int s, int h, int a>
constexpr Value<Unit<-m, -k, -s, -h, -a>> operator/(double d,
                                      const Value<Unit<m, k, s, h, a>>& obj) {
  return Value<Unit<-m, -k, -s, -h, -a>>(d / obj.val);
}
template<int m, int k, int s, int h, int a>
constexpr Value<Unit<-m, -k, -s, -h, -a>> operator/(int i,
                                      const Value<Unit<m, k, s, h, a>>& obj) {
  return Value<Unit<-m, -k, -s, -h, -a>>(i / obj.val);
}

#else
typedef double Distance;
typedef double Time;
typedef double P_Time;
typedef double Health;
typedef double Speed;
typedef double Acceleration;
typedef double Weight;
typedef double Force;
typedef double Angle;
typedef double Angular_Speed;
typedef double Angular_Acceleration;

constexpr double operator"" _s(long double d) {
  return d;
}
constexpr double operator"" _s(unsigned long long d) {
  return d;
}
constexpr double operator"" _min(long double d) {
  return 60*d;
}
constexpr double operator"" _min(unsigned long long d) {
  return 60*d;
}
constexpr double operator"" _m(long double d) {
  return d;
}
constexpr double operator"" _m(unsigned long long d) {
  return d;
}
constexpr double operator"" _hp(long double d) {
  return d;
}
constexpr double operator"" _hp(unsigned long long d) {
  return d;
}
constexpr double operator"" _m_s2(long double d) {
  return d;
}
constexpr double operator"" _m_s2(unsigned long long d) {
  return d;
}
constexpr double operator"" _kg(long double d) {
  return d;
}
constexpr double operator"" _kg(unsigned long long d) {
  return d;
}
constexpr double operator"" _N(long double d) {
  return d;
}
constexpr double operator"" _N(unsigned long long d) {
  return d;
}
constexpr double operator"" _deg(long double d) {
  return d;
}
constexpr double operator"" _deg(unsigned long long d) {
  return d;
}
constexpr double operator"" _deg_s2(long double d) {
  return d;
}
constexpr double operator"" _deg_s2(unsigned long long d) {
  return d;
}
#endif
typedef Force Thrust;

}  // namespace goio

#endif  // CONSTANTS_H_
