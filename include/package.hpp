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
    explicit Package(ElementID id) : id_(id) {assigned_IDs.insert(id);}
    Package(Package &&package) noexcept : id_(package.id_) {}

    Package& operator=(Package &&package);

    ElementID get_id() const { return id_; }
    ~Package();


private:
    ElementID id_;
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;


};

#endif //LAB_NETSIM_PACKAGE_HPP
