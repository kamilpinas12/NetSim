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


template <class T>
class NodeCollection{
public:
    using container_t = typename std::list<T>;
    using const_iterator = typename container_t::const_iterator;
    using iterator = typename container_t::iterator;

    const_iterator begin() const {return collection_.begin();}
    const_iterator end() const {return collection_.end();}
    const_iterator cbegin() const {return collection_.cbegin();}
    const_iterator cend() const {return collection_.cend();}

    iterator begin() {return collection_.begin();}
    iterator end() {return collection_.end();}
    iterator cbegin() {return collection_.cbegin();}
    iterator cend() {return collection_.cend();}

    void add(T&&  node) {collection_.insert(std::move(node));}

    //TODO: metody remove_by_id i find_by_id (Kamil)
    void remove_by_id(ElementID id);
    iterator find_by_id(ElementID id);
    //const_iterator  find_by_id const (Element id);

private:
    container_t collection_;
};




class Factory{
public:
    // TODO: Zrealizować funkcjonalności Rampy, Workera i Storehouse
    void add_ramp(Ramp&& ramp);
    void remove_ramp(ElementID id);
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id);
    // ...
    //TODO: Sprawdzenie spójności (Filip)
    bool is_consistent() const;

    //TODO: Metody wykonujące przekazania (Kamil)
    void do_delivery(Time t);
    void do_package_passing();
    void do_work(Time t);
private:
    NodeCollection<Storehouse>  storehouse_;
    NodeCollection<Worker> worker_;
    NodeCollection<Ramp> ramp_;
};



