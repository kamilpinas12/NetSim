#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "factory.hpp"

#include <set>

//using ::testing::Return;
//using ::testing::_;
using ::testing::ContainerEq;

using ::std::cout;
using ::std::endl;


TEST(FactoryIOTest, ParseRamp) {
    std::istringstream iss("LOADING_RAMP id=1 delivery-interval=3");
    auto factory = load_factory_structure(iss);

    ASSERT_EQ(std::next(factory.ramp_cbegin(), 1), factory.ramp_cend());
    const auto& r = *(factory.ramp_cbegin());
    EXPECT_EQ(1, r.get_id());
    EXPECT_EQ(3, r.get_delivery_interval());
}

TEST(FactoryIOTest, ParseWorker) {
    std::istringstream iss("WORKER id=1 processing-time=2 queue-type=FIFO");
    auto factory = load_factory_structure(iss);

    ASSERT_EQ(std::next(factory.worker_cbegin(), 1), factory.worker_cend());
    const auto& w = *(factory.worker_cbegin());
    EXPECT_EQ(1, w.get_id());
    EXPECT_EQ(2, w.get_processing_duration());
    EXPECT_EQ(PackageQueueType::FIFO, w.get_queue()->get_queue_type());
}

TEST(FactoryIOTest, ParseStorehouse) {
    std::istringstream iss("STOREHOUSE id=1");
    auto factory = load_factory_structure(iss);

    ASSERT_EQ(std::next(factory.storehouse_cbegin(), 1), factory.storehouse_cend());
    const auto& s = *(factory.storehouse_cbegin());
    EXPECT_EQ(1, s.get_id());
}

TEST(FactoryIOTest, ParseLinkOneReceiver) {
    std::ostringstream oss;
    oss << "LOADING_RAMP id=1 delivery-interval=3" << "\n"
        << "STOREHOUSE id=1" << "\n"
        << "LINK src=ramp-1 dest=store-1" << "\n";
    std::istringstream iss(oss.str());
    auto factory = load_factory_structure(iss);

    ASSERT_EQ(std::next(factory.ramp_cbegin(), 1), factory.ramp_cend());
    const auto& r = *(factory.ramp_cbegin());

    ASSERT_EQ(std::next(factory.storehouse_cbegin(), 1), factory.storehouse_cend());
    const auto& s = *(factory.storehouse_cbegin());

    auto prefs = r.receiver_preferences_.get_preferences();
    ASSERT_EQ(1U, prefs.size());
    auto key = dynamic_cast<IPackageReceiver*>(const_cast<Storehouse*>(&s));
    ASSERT_NE(prefs.find(key), prefs.end());
    EXPECT_DOUBLE_EQ(prefs[key], 1.0);
}

//TEST(FactoryIOTest, ParseLinkOneReceiverWithDefinedProbability) {
//    std::ostringstream oss;
//    oss << "LOADING_RAMP id=1 delivery-interval=3" << "\n"
//        << "STOREHOUSE id=1" << "\n"
//        << "LINK src=ramp-1 dest=store-1 p=1.0" << "\n";
//    std::istringstream iss(oss.str());
//    auto factory = load_factory_structure(iss);
//
//    ASSERT_EQ(std::next(factory.ramp_cbegin(), 1), factory.ramp_cend());
//    const auto& r = *(factory.ramp_cbegin());
//
//    ASSERT_EQ(std::next(factory.storehouse_cbegin(), 1), factory.storehouse_cend());
//    const auto& s = *(factory.storehouse_cbegin());
//
//    auto prefs = r.receiver_preferences_.get_preferences();
//    ASSERT_EQ(1U, prefs.size());
//    auto key = dynamic_cast<IPackageReceiver*>(const_cast<Storehouse*>(&s));
//    ASSERT_NE(prefs.find(key), prefs.end());
//    EXPECT_DOUBLE_EQ(prefs[key], 1.0);
//}

TEST(FactoryIOTest, ParseLinkMultipleReceivers) {
    std::ostringstream oss;
    oss << "LOADING_RAMP id=1 delivery-interval=3" << "\n"
        << "STOREHOUSE id=1" << "\n"
        << "STOREHOUSE id=2" << "\n"
        << "LINK src=ramp-1 dest=store-1" << "\n"
        << "LINK src=ramp-1 dest=store-2" << "\n";
    std::istringstream iss(oss.str());
    auto factory = load_factory_structure(iss);

    ASSERT_EQ(std::next(factory.ramp_cbegin(), 1), factory.ramp_cend());
    const auto& r = *(factory.ramp_cbegin());

    ASSERT_EQ(std::next(factory.storehouse_cbegin(), 2), factory.storehouse_cend());
    const auto& s1 = *(factory.storehouse_cbegin());
    const auto& s2 = *(std::next(factory.storehouse_cbegin(), 1));

    auto prefs = r.receiver_preferences_.get_preferences();
    ASSERT_EQ(2U, prefs.size());
    auto key1 = dynamic_cast<IPackageReceiver*>(const_cast<Storehouse*>(&s1));
    auto key2 = dynamic_cast<IPackageReceiver*>(const_cast<Storehouse*>(&s2));
    ASSERT_NE(prefs.find(key1), prefs.end());
    ASSERT_NE(prefs.find(key2), prefs.end());
    EXPECT_DOUBLE_EQ(prefs[key1], 0.5);
    EXPECT_DOUBLE_EQ(prefs[key2], 0.5);
}

//TEST(FactoryIOTest, ParseLinkMultipleReceiversWithDefinedProbabilities) {
//    std::ostringstream oss;
//    oss << "LOADING_RAMP id=1 delivery-interval=3" << "\n"
//        << "STOREHOUSE id=1" << "\n"
//        << "STOREHOUSE id=2" << "\n"
//        << "LINK src=ramp-1 dest=store-1 p=0.3" << "\n"
//        << "LINK src=ramp-1 dest=store-2 p=0.7" << "\n";
//    std::istringstream iss(oss.str());
//    auto factory = load_factory_structure(iss);
//
//    ASSERT_EQ(std::next(factory.ramp_cbegin(), 1), factory.ramp_cend());
//    const auto& r = *(factory.ramp_cbegin());
//
//    ASSERT_EQ(std::next(factory.storehouse_cbegin(), 2), factory.storehouse_cend());
//    const auto& s1 = *(factory.storehouse_cbegin());
//    const auto& s2 = *(std::next(factory.storehouse_cbegin(), 1));
//
//    auto prefs = r.receiver_preferences_.get_preferences();
//    ASSERT_EQ(2U, prefs.size());
//    auto key1 = dynamic_cast<IPackageReceiver*>(const_cast<Storehouse*>(&s1));
//    auto key2 = dynamic_cast<IPackageReceiver*>(const_cast<Storehouse*>(&s2));
//    ASSERT_NE(prefs.find(key1), prefs.end());
//    ASSERT_NE(prefs.find(key2), prefs.end());
//    EXPECT_DOUBLE_EQ(prefs[key1], 0.3);
//    EXPECT_DOUBLE_EQ(prefs[key2], 0.7);
//}

TEST(FactoryIOTest, LoadAndSaveTest) {
    std::string r1 = "LOADING_RAMP id=1 delivery-interval=3";
    std::string r2 = "LOADING_RAMP id=2 delivery-interval=2";
    std::string w1 = "WORKER id=1 processing-time=2 queue-type=FIFO";
    std::string w2 = "WORKER id=2 processing-time=1 queue-type=LIFO";
    std::string s1 = "STOREHOUSE id=1";
    std::string l1 = "LINK src=ramp-1 dest=worker-1";
    std::string l2 = "LINK src=ramp-2 dest=worker-1";
    std::string l3 = "LINK src=ramp-2 dest=worker-2";
    std::string l4 = "LINK src=worker-1 dest=worker-1";
    std::string l5 = "LINK src=worker-1 dest=worker-2";
    std::string l6 = "LINK src=worker-2 dest=store-1";

    std::set<std::string> input_set = {r1, r2, w1, w2, s1, l1, l2, l3, l4, l5, l6};

    std::vector<std::string> input_lines{
            "; == LOADING RAMPS ==",
            "",
            r1,
            r2,
            "",
            "; == WORKERS ==",
            "",
            w1,
            w2,
            "",
            "; == STOREHOUSES ==",
            "",
            s1,
            "",
            "; == LINKS ==",
            "",
            l1,
            "",
            l2,
            l3,
            "",
            l4,
            l5,
            "",
            l6,
    };

    // ignore empty lines, ignore comments ("; ...")
    // resulting set must be empty

    std::string factoryStructureStr = std::accumulate(input_lines.begin(), input_lines.end(), std::string(""),
                                                      [](std::string accu, std::string line) {
                                                          return accu + line + "\n";
                                                      });

    std::istringstream iss(factoryStructureStr);
    Factory factory;
    try {
        factory = load_factory_structure(iss);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        FAIL();
    } catch (...) {
        std::cerr << "Unknown error..." << std::endl;
        FAIL();
    }

    std::ostringstream oss;
    save_factory_structure(factory, oss);

    std::vector<std::string> output_lines;
    //
    std::string structure_str = oss.str();

    iss.str(structure_str);
    iss.clear();
    std::string line;
    while (std::getline(iss, line, '\n')) {
        bool is_blank_line = line.empty() || isblank(line[0]);
        bool is_comment_line = !line.empty() && line[0] == ';';
        if (is_blank_line || is_comment_line) {
            continue;
        }

        output_lines.push_back(line);
    }

    std::set<std::string> output_set(output_lines.begin(), output_lines.end());
    ASSERT_EQ(output_set.size(), output_lines.size()) << "Duplicated lines in the output.";

    EXPECT_THAT(output_set, ContainerEq(input_set));

    // Elementy w pliku występującą w następującej kolejności: LOADING_RAMP, WORKER, STOREHOUSE, LINK.

    auto first_occurence = [&output_lines](std::string label) {
        return std::find_if(output_lines.begin(), output_lines.end(),
                            [label](const std::string s) { return s.rfind(label, 0) == 0; });
    };

    auto first_ramp_it = first_occurence("LOADING_RAMP");
    auto first_worker_it = first_occurence("WORKER");
    auto first_storehouse_it = first_occurence("STOREHOUSE");
    auto first_link_it = first_occurence("LINK");

    ASSERT_LT(first_ramp_it, first_worker_it);
    ASSERT_LT(first_worker_it, first_storehouse_it);
    ASSERT_LT(first_storehouse_it, first_link_it);
}
