#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "nodes.hpp"
#include "simulation.hpp"
#include "helpers.hpp"
#include "reports.hpp"

using ::testing::Return;
using ::testing::_;

TEST(SimulationTest, Simulate) {

    Factory factory;
    factory.add_ramp(Ramp(1, 10));
    factory.add_worker(Worker(1, 1, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    factory.add_storehouse(Storehouse(1));

    ReceiverPreferences prefs;

    Ramp& r = *(factory.find_ramp_by_id(1));
    r.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(1)));

    Worker& w = *(factory.find_worker_by_id(1));
    w.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(1)));

    simulate(factory, 3, [](Factory&, TimeOffset) {});

    // Robotnik ma pustą kolejkę i bufor.
    EXPECT_EQ(w.cbegin(), w.cend());
    EXPECT_FALSE(w.get_sending_buffer().has_value());

    // Magazyn zawiera półprodukt.
    auto storehouse_it = factory.storehouse_cbegin();
    ASSERT_NE(storehouse_it->cbegin(), storehouse_it->cend());
    EXPECT_EQ(storehouse_it->cbegin()->get_id(), 1);
}
