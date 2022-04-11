// Ben(302821), Bereznicki(302822), Dembrovskyi(302835)

#include "simulation.hpp"

void simulate(Factory& f, TimeOffset d, std::function<void (Factory&, Time)> rf){
    if(!f.is_consistent()){
        throw std::logic_error("Siec jest niespojna.");
    }else {
        for (unsigned int tm = 1; tm <= d; tm++) {
        f.do_deliveries(tm);
        f.do_package_passing();
        f.do_work(tm);
        rf(f, tm);
        }
    }
}
