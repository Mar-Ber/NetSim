// Ben(302821), Bereznicki(302822), Dembrovskyi(302835)

#ifndef NETSIM_SIMULATION_HPP
#define NETSIM_SIMULATION_HPP

#include "factory.hpp"

void simulate(Factory& f, TimeOffset d, std::function<void (Factory&, Time)> rf);

#endif //NETSIM_SIMULATION_HPP
