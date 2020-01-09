// Cytosim was created by Francois Nedelec. Copyright 2007-2017 EMBL.

#include "event.h"
#include "exceptions.h"
#include "iowrapper.h"
#include "glossary.h"
#include "simul.h"
#include "parser.h"


void Event::clear()
{
    activity = "";
    delay = 0;
    rate = 0;
    nextTime = 0;
}


void Event::reset(real time)
{
    if ( rate > 0 )
        nextTime = time + RNG.exponential() / rate;
    else
        nextTime = time + delay;
}


Event::Event(real time, Glossary& opt)
{
    clear();
    opt.set(activity, "activity") || opt.set(activity, "code");
    opt.set(rate, "rate") || opt.set(delay, "delay");
    if ( rate < 0 )
        throw InvalidParameter("event:rate must be >= 0");
    if ( delay < 0 )
        throw InvalidParameter("event:delay must be >= 0");
    reset(time);
}


Event::~Event()
{
    //Cytosim::log("destroying Event %p\n", this);
}


void Event::step(Simul& sim)
{
    if ( sim.time() > nextTime )
    {
        sim.relax();
        do {
            reset(nextTime);
            Parser(sim, 1, 1, 1, 1, 1).evaluate(activity);
        } while ( sim.time() > nextTime );
        sim.unrelax();
    }
}


void Event::write(Outputter& out) const
{
}


void Event::read(Inputter& in, Simul& sim, ObjectTag tag)
{
}
