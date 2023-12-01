//
// Created by kamil on 30/11/2023.
//

#ifndef LAB_NETSIM_STORAGE_TYPES_HPP
#define LAB_NETSIM_STORAGE_TYPES_HPP

#include "package.hpp"

enum PackageQueueType{
    FIFO,
    LIFO
};


class IPackageStockpile{
public:
    virtual void push(Package&&) = 0;
    virtual bool empty() = 0;
    virtual  std::size_t size() = 0;
    virtual ~IPackageStockpile() = 0;
    //TODO: Napisać konstruktory tej klasy

};


class IPackageQueue: public IPackageStockpile{
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() = 0;

};


class PackageQueue: public IPackageQueue{
public:
    //TODO: Zaimplementować metody, zmienne itp. tej klasy


};





#endif //LAB_NETSIM_STORAGE_TYPES_HPP
