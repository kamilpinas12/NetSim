//#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "factory.hpp"
#include "nodes.hpp"

// DEBUG
#include <iostream>

using ::std::cout;
using ::std::endl;

TEST(FactoryTest, IsConsistentCorrect) {
    // R -> W -> S

    Factory factory;
    factory.add_ramp(Ramp(1, 1));
    factory.add_worker(Worker(1, 1, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    factory.add_storehouse(Storehouse(1));

    Ramp& r = *(factory.find_ramp_by_id(1));
    r.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(1)));

    Worker& w = *(factory.find_worker_by_id(1));
    w.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(1)));

    EXPECT_TRUE(factory.is_consistent());
}

TEST(FactoryTest, IsConsistentMissingLink1) {
    // R -> W .. S
    //      W -> W

    Factory factory;
    factory.add_ramp(Ramp(1, 1));
    factory.add_worker(Worker(1, 1, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    factory.add_storehouse(Storehouse(1));

    Ramp& r = *(factory.find_ramp_by_id(1));
    r.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(1)));

    Worker& w = *(factory.find_worker_by_id(1));
    w.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(1)));

    EXPECT_FALSE(factory.is_consistent());
}

TEST(FactoryTest, IsConsistentMissingLink2) {
    // R -> W1 -> S
    //      W1 -> W2 -> W2

    Factory factory;
    factory.add_ramp(Ramp(1, 1));
    factory.add_worker(Worker(1, 1, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    factory.add_worker(Worker(2, 1, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    factory.add_storehouse(Storehouse(1));

    Ramp& r = *(factory.find_ramp_by_id(1));
    r.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(1)));

    Worker& w1 = *(factory.find_worker_by_id(1));
    w1.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(1)));
    w1.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(2)));

    Worker& w2 = *(factory.find_worker_by_id(2));
    w2.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(2)));

    EXPECT_FALSE(factory.is_consistent());
}

TEST(FactoryTest, RemoveWorkerNoSuchReceiver) {
    /* Próba usunięcia nieistniejącego odbiorcy - dopuszczalne. */

    Factory factory;
    factory.add_ramp(Ramp(1, 1));
    factory.add_worker(Worker(1, 1, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));

    Ramp& r = *(factory.find_ramp_by_id(1));
    Worker& w = *(factory.find_worker_by_id(1));
    r.receiver_preferences_.add_receiver(&w);

    Worker w2(2, 1, std::make_unique<PackageQueue>(PackageQueueType::FIFO));

    factory.remove_worker(w2.get_id());

    auto prefs = r.receiver_preferences_.get_preferences();
    ASSERT_EQ(prefs.size(), 1U);

    auto it = prefs.find(&w);
    ASSERT_NE(it, prefs.end());
    EXPECT_EQ(it->second, 1.0);
}

TEST(FactoryTest, RemoveWorkerOnlyOneReceiver) {

    Factory factory;
    factory.add_ramp(Ramp(1, 1));
    factory.add_worker(Worker(1, 1, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));

    Ramp& r = *(factory.find_ramp_by_id(1));
    Worker& w = *(factory.find_worker_by_id(1));
    r.receiver_preferences_.add_receiver(&w);

    factory.remove_worker(w.get_id());

    auto prefs = r.receiver_preferences_.get_preferences();
    ASSERT_TRUE(prefs.empty());
}

TEST(FactoryTest, RemoveWorkerTwoRemainingReceivers) {

    Factory factory;
    factory.add_ramp(Ramp(1, 1));
    factory.add_worker(Worker(1, 1, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    factory.add_worker(Worker(2, 1, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    factory.add_worker(Worker(3, 1, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));

    Ramp& r = *(factory.find_ramp_by_id(1));

    r.receiver_preferences_.add_receiver(&(*(factory.find_worker_by_id(1))));
    r.receiver_preferences_.add_receiver(&(*(factory.find_worker_by_id(2))));
    r.receiver_preferences_.add_receiver(&(*(factory.find_worker_by_id(3))));

//    ReceiverPreferences::preferences_t p;
//    p[&(*(factory.find_worker_by_id(1)))] = 1.0 / 3.0;
//    p[&(*(factory.find_worker_by_id(2)))] = 1.0 / 3.0;
//    p[&(*(factory.find_worker_by_id(3)))] = 1.0 / 3.0;
//    r.receiver_preferences_.set_preferences(p);

    factory.remove_worker(1);

    auto prefs = r.receiver_preferences_.get_preferences();
    ASSERT_EQ(prefs.size(), 2U);

    auto it = prefs.find(&(*(factory.find_worker_by_id(2))));
    ASSERT_NE(it, prefs.end());
    EXPECT_DOUBLE_EQ(it->second, 1.0 / 2.0);

    it = prefs.find(&(*(factory.find_worker_by_id(3))));
    ASSERT_NE(it, prefs.end());
    EXPECT_DOUBLE_EQ(it->second, 1.0 / 2.0);
}
