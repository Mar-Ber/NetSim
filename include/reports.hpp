// Ben(302821), Bereznicki(302822), Dembrovskyi(302835)

#ifndef NETSIM_REPORTS_HPP
#define NETSIM_REPORTS_HPP

#include "simulation.hpp"

class IntervalReportNotifier{
public:
    IntervalReportNotifier(TimeOffset to): to_(to) {};
    bool should_generate_report(Time t);

private:
    TimeOffset to_;
};

class SpecificTurnsReportNotifier{
public:
    SpecificTurnsReportNotifier(std::set<Time> turns): turns_(turns) {};
    bool should_generate_report(Time t);

private:
    std::set<Time> turns_;
};

#endif //NETSIM_REPORTS_HPP
