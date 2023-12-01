//
// Created by Filip Połatyński on 30/11/2023.
//


#ifndef LAB_NETSIM_PACKAGE_HPP
#define LAB_NETSIM_PACKAGE_HPP

#include <set>

#include "types.hpp"

class Package{
public:
    Package();
    Package(ElementID);
    Package(Package&&);

    Package& operator=(Package &&package);

    ElementID get_id() const;
    ~Package();


private:
    ElementID id_;
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID > freed_IDs;


};

#endif //LAB_NETSIM_PACKAGE_HPP
