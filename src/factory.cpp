//
// Created by kamil on 18/12/2023.
//

#include "factory.hpp"
#include <algorithm>


void Factory::do_work(Time t) {
    std::for_each(worker_.begin(), worker_.end(),
                  [t](auto &elem){elem.do_work(t);});
}


void Factory::do_delivery(Time t) {
    std::for_each(ramp_.begin(), ramp_.end(),
                  [t](auto &elem){elem.deliver_goods(t);});
}


void Factory::do_package_passing() {
    std::for_each(worker_.begin(), worker_.end(),
                  [](auto &elem){elem.send_package();});

    std::for_each(ramp_.begin(), ramp_.end(),
                  [](auto &elem){elem.send_package();});
}
