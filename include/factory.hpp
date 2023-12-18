//
// Created by Filip Połatyński on 18/12/2023.
//

#ifndef LAB_NETSIM_FACTORY_HPP
#define LAB_NETSIM_FACTORY_HPP

#include "types.hpp"
#include "storage_types.hpp"
#include "nodes.hpp"
#include "package.hpp"
#endif //LAB_NETSIM_FACTORY_HPP


class Factory{
public:
    // Ramp
    void add_ramp(Ramp&& ramp);
    void remove_ramp(ElementID id);

    bool is_consistent() const;
    void do_delivery(Time t);
    void do_package_passing();
    void do_work();
private:

};


template <class T>
class NodeCollection{
private:
    std::list<T> collection_;
};
