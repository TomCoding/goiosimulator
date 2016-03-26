#ifndef __TIMEOBJ_H
#define __TIMEOBJ_H

#include "goioobj.h"
#include "goioactor.h"
#include <map>

namespace goio {

  class TimeObj {
    private:
      double time;
      struct FuncData {
        int id;
        GoioObj* registrar;  // key in registrars
        TimeDmgFunc timedmgfunc;
        GoioObj* obj;
        double time_prev;
        double time_next;    // key in events
        double time_cur;     // if set, not in events (on hold)
        TimeCheckFunc timecheckfunc;
      };

      std::multimap<double, FuncData*> events;
      std::multimap<GoioObj*, FuncData*> registrars;
      std::multimap<GoioObj*, FuncData*> recipients;

      void register_event(FuncData* funcdata, double time);
      bool recalc_next_event(FuncData* funcdata);

      static int max_id;

    public:
      TimeObj() : time(0), events(), registrars(), recipients() {}
      ~TimeObj();

      inline double get_time() const { return time; }
      bool next_event();
      int register_event(GoioActor* registrar, TimeDmgFunc timedmgfunc,
                          GoioObj* obj, TimeCheckFunc timecheckfunc,
                          double time = 0, bool rel = true);

      bool unregister_event(int id);

      void reset();
  };

}

#endif // __TIMEOBJ_H
