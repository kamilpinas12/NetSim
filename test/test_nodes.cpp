#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "nodes.hpp"
#include "package.hpp"
#include "storage_types.hpp"
#include "types.hpp"

#include "nodes_mocks.hpp"
#include "global_functions_mock.hpp"

#include <iostream>

using ::std::cout;
using ::std::endl;

// -----------------

TEST(WorkerTest, HasBuffer) {
    // Test scenariusza opisanego na stronie:
    // http://home.agh.edu.pl/~mdig/dokuwiki/doku.php?id=teaching:programming:soft-dev:topics:net-simulation:part_nodes#bufor_aktualnie_przetwarzanego_polproduktu

    Worker w(1, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Time t = 1;

    w.receive_package(Package(1));
    w.do_work(t);
    ++t;
    w.receive_package(Package(2));
    w.do_work(t);
    auto& buffer = w.get_sending_buffer();

    ASSERT_TRUE(buffer.has_value());
    EXPECT_EQ(buffer.value().get_id(), 1);
}

// -----------------

TEST(RampTest, IsDeliveryOnTime) {

    Ramp r(1, 2);
    auto recv = std::make_unique<Storehouse>(1);

    r.receiver_preferences_.add_receiver(recv.get());

    r.deliver_goods(1);
    ASSERT_TRUE(r.get_sending_buffer().has_value());
    r.send_package();

    r.deliver_goods(2);
    ASSERT_FALSE(r.get_sending_buffer().has_value());

    r.deliver_goods(3);
    ASSERT_TRUE(r.get_sending_buffer().has_value());
}

// -----------------

TEST(ReceiverPreferencesTest, AddReceiversRescalesProbability) {
    // Upewnij się, że dodanie odbiorcy spowoduje przeskalowanie prawdopodobieństw.
    ReceiverPreferences rp;

    MockReceiver r1;
    rp.add_receiver(&r1);
    ASSERT_NE(rp.get_preferences().find(&r1), rp.get_preferences().end());
    EXPECT_EQ(rp.get_preferences().at(&r1), 1.0);

    MockReceiver r2;
    rp.add_receiver(&r2);
    EXPECT_EQ(rp.get_preferences().at(&r1), 0.5);
    ASSERT_NE(rp.get_preferences().find(&r2), rp.get_preferences().end());
    EXPECT_EQ(rp.get_preferences().at(&r2), 0.5);
}

TEST(ReceiverPreferencesTest, RemoveReceiversRescalesProbability) {
    // Upewnij się, że usunięcie odbiorcy spowoduje przeskalowanie pozostałych prawdopodobieństw.
    ReceiverPreferences rp;

    MockReceiver r1, r2;
    rp.add_receiver(&r1);
    rp.add_receiver(&r2);

    rp.remove_receiver(&r2);
    ASSERT_EQ(rp.get_preferences().find(&r2), rp.get_preferences().end());
    EXPECT_EQ(rp.get_preferences().at(&r1), 1.0);
}

// Przydatny alias, żeby zamiast pisać `::testing::Return(...)` móc pisać
// samo `Return(...)`.
using ::testing::Return;

// Dla czytelności nazewnictwa grupy scenariuszy testowych (oraz ponieważ być
// może zajdzie konieczność korzystania z więcej niż jednej "fixturki") tworzymy
// osobną klasę dziedziczącą po `GlobalFunctionFixture`.
class ReceiverPreferencesChoosingTest : public GlobalFunctionsFixture {
};

TEST_F(ReceiverPreferencesChoosingTest, ChooseReceiver) {
    // Upewnij się, że odbiorcy wybierani są z właściwym prawdopodobieństwem.

    EXPECT_CALL(global_functions_mock, generate_canonical()).WillOnce(Return(0.3)).WillOnce(Return(0.7));

    ReceiverPreferences rp;

    MockReceiver r1, r2;
    rp.add_receiver(&r1);
    rp.add_receiver(&r2);

    if (rp.begin()->first == &r1) {
        EXPECT_EQ(rp.choose_receiver(), &r1);
        EXPECT_EQ(rp.choose_receiver(), &r2);
    } else {
        EXPECT_EQ(rp.choose_receiver(), &r2);
        EXPECT_EQ(rp.choose_receiver(), &r1);
    }
}

// -----------------

using ::testing::Return;
using ::testing::_;

// Ponieważ `IPackageStockpile::const_iterator` to iterator na (niestandardowy)
// typ Package, który nie przeciąża operatora <<, Google Mock nie ma pojęcia
// w jaki sposób wypisać iterator w postaci tekstowej (w przypadku nie
// spełnienia asercji - w ramach komunikatu diagnostycznego).
// Aby rozwiązać ten problem, możemy ręcznie zdefiniować funkcję wywoływaną
// w sytuacji, gdy zachodzi potrzeba wypisania obiektu niestandardowego typu
// - w tym przypadku `IPackageStockpile::const_iterator`.
// zob. https://github.com/google/googlemock/blob/master/googlemock/docs/v1_5/CookBook.md#teaching-google-mock-how-to-print-your-values
void PrintTo(const IPackageStockpile::const_iterator& it, ::std::ostream* os) {
    *os << it->get_id();
}

class PackageSenderFixture : public PackageSender {
    // Nie sposób w teście wykorzystać prywetnej metody `PackageSender::push_package()`,
    // dlatego do celów testowych stworzona została implementacja zawierająca
    // metodę `push_package()` w sekcji publicznej.
public:
    void push_package(Package&& package) { PackageSender::push_package(std::move(package)); }
};


TEST(PackageSenderTest, SendPackage) {
    MockReceiver mock_receiver;
    // Oczekujemy, że metoda `receive_package()` obiektu `mock_receiver` zostanie
    // wywołana dwukrotnie, z dowolnym argumentem (symbol `_`).
    EXPECT_CALL(mock_receiver, receive_package(_)).Times(1);

    PackageSenderFixture sender;
    sender.receiver_preferences_.add_receiver(&mock_receiver);
    // Zwróć uwagę, że poniższa instrukcja korzysta z semantyki referencji do r-wartości.
    sender.push_package(Package());

    sender.send_package();

    EXPECT_FALSE(sender.get_sending_buffer());

    // Upewnij się, że proces wysyłania zachodzi tylko wówczas, gdy w bufor jest pełny.
    sender.send_package();
}
