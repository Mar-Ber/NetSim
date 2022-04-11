// Ben(302821), Bereznicki(302822), Dembrovskyi(302835)
#ifndef NETSIM_TYPES_HPP
#define NETSIM_TYPES_HPP

#include <functional>

using ElementID = int;
using size_type = unsigned long long int;
using Time = unsigned int;
using TimeOffset = unsigned int;
using ProbabilityGenerator = std::function<double()>;

#endif //NETSIM_TYPES_HPP
