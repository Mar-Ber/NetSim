// Ben(302821), Bereznicki(302822), Dembrovskyi(302835)

#include <reports.hpp>

bool IntervalReportNotifier::should_generate_report(Time t) {
    return t % to_ == 1;
}

bool SpecificTurnsReportNotifier::should_generate_report(Time t) {
    auto search = turns_.find(t);
    return search != turns_.end();
}
