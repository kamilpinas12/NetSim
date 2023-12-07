//
// Created by kamil on 30/11/2023.
//

#ifndef LAB_NETSIM_STORAGE_TYPES_HPP
#define LAB_NETSIM_STORAGE_TYPES_HPP


#include <list>
#include "package.hpp"


enum class PackageQueueType{
    FIFO,
    LIFO
};


class IPackageStockpile{
public:
    virtual void push(Package&& package) = 0;
    virtual bool empty() const = 0;
    virtual  std::size_t size() const = 0;
    virtual ~IPackageStockpile() = default;
    using const_iterator = std::list<Package>::const_iterator;

    virtual const_iterator begin() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cend() const = 0;

};

class IPackageQueue: public IPackageStockpile{
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() = 0;
};


class PackageQueue: public IPackageQueue{
public:

    explicit PackageQueue(PackageQueueType type) : queue_(), queue_type_(type) {}


    const_iterator begin() const override {return queue_.cbegin();}
    const_iterator cbegin() const override {return queue_.cbegin();}
    const_iterator end() const override {return queue_.end();}
    const_iterator cend() const override {return queue_.cend();}


    Package pop() override;
    PackageQueueType get_queue_type() override {return queue_type_;}
    void push(Package&& package) override  {queue_.emplace_back(std::move(package));}
    std::size_t size() const override {return queue_.size();}
    bool empty() const override {return queue_.empty();}


private:
    std::list<Package> queue_;
    PackageQueueType queue_type_;
};

#endif //LAB_NETSIM_STORAGE_TYPES_HPP

