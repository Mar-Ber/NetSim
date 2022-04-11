// Ben(302821), Bereznicki(302822), Dembrovskyi(302835)

#include <gmock/gmock-spec-builders.h>
#include "gtest/gtest.h"
#include "storage_types.hpp"
#include "package.hpp"
#include "nodes.hpp"

//std::random_device rd;
//std::mt19937 rng(rd());
// SPYTAC JAK Z TYM POWYZEJ

/*
TEST(PackageSenderTest,empty_buffer_test){
    PackageQueueType qt = PackageQueueType::FIFO;
    Worker w1(1, 1, std::make_unique<PackageQueue>(qt));
    Package p1;

    Storehouse s1 (1, std::make_unique<PackageQueue>(qt));
    ReceiverPreferences stores;
    stores.add_receiver(&s1);
    w1.receiver_preferences_ = stores;

    w1.push_package(std::move(p1));
    w1.send_package();

    EXPECT_EQ(w1.receiver_preferences_.begin()->first, &s1);
    ASSERT_FALSE(w1.get_sending_buffer());
}
 //TEST WYMAGA PUBLICZNEGO PUSH_PACKAGE()
 */

TEST(ReceiverPreferencesTest,add_test){
    //ProbabilityGenerator pg_fun = [](){return std::generate_canonical<double, 10>(rng);};
    ReceiverPreferences workers;
    PackageQueueType qt = PackageQueueType::FIFO;

    Worker w1(1,1,std::make_unique<PackageQueue>(qt));
    Worker w2(2,2,std::make_unique<PackageQueue>(qt));
    Worker w3(3,1,std::make_unique<PackageQueue>(qt));
    workers.add_receiver(&w1);
    workers.add_receiver(&w2);
    workers.add_receiver(&w3);

    double x = 1;
    double y = 3;

    EXPECT_EQ(workers.begin()->second, x/y);
    //EXPECT_EQ(workers.begin()->first, &w3);
}

TEST(ReceiverPreferencesTest,clear_test){
    ProbabilityGenerator pg_fun = [](){return std::generate_canonical<double, 10>(rng);};
    ReceiverPreferences workers(pg_fun);
    PackageQueueType qt = PackageQueueType::FIFO;

    Worker w1(1,1,std::make_unique<PackageQueue>(qt));
    Worker w2(2,2,std::make_unique<PackageQueue>(qt));
    Worker w3(3,1,std::make_unique<PackageQueue>(qt));
    workers.add_receiver(&w1);
    workers.add_receiver(&w2);
    workers.add_receiver(&w3);
    workers.remove_receiver(&w3);

    double x = 1;
    double y = 2;

    EXPECT_EQ(workers.begin()->second, x/y);
}

TEST(ReceiverPreferencesTest,choose_test){
    ProbabilityGenerator pg_fun = [](){return std::generate_canonical<double, 10>(rng);};
    PackageQueueType qt = PackageQueueType::FIFO;
    ReceiverPreferences workers(pg_fun);

    Worker w1(1,1,std::make_unique<PackageQueue>(qt));
    Worker w2(2,2,std::make_unique<PackageQueue>(qt));
    Worker w3(3,1,std::make_unique<PackageQueue>(qt));
    workers.add_receiver(&w1);
    workers.add_receiver(&w2);
    workers.add_receiver(&w3);

    //EXPECT_EQ(&w2, workers.choose_receiver());
}

TEST(RampTest,delivery_test){
    PackageQueueType qt = PackageQueueType::FIFO;
    Worker worker_(0, 1, std::make_unique<PackageQueue>(qt));

    Ramp ramp_ (1, 2);
    for (unsigned int time = 1; time < 3; time++){
        ramp_.deliver_goods(time);
    }
    EXPECT_EQ(ramp_.get_sending_buffer()->get_id(), 1);
}

TEST(WorkerTest,receive_test){
    PackageQueueType qt = PackageQueueType::FIFO;
    Worker worker_(1, 1, std::make_unique<PackageQueue>(qt));

    Ramp ramp_ (1, 2);
    ReceiverPreferences workers;
    workers.add_receiver(&worker_);
    ramp_.receiver_preferences_ = workers;

    for (unsigned int time = 0; time < 5; time++){
        ramp_.deliver_goods(time);
        ramp_.send_package();
    }

    IPackageReceiver::const_iterator wk_it = worker_.cbegin();
    for(int n=1; n<3; n++) {
        EXPECT_EQ(wk_it->get_id(), n);
        wk_it++;
    }
}

TEST(WorkerTest,get_test){
    PackageQueueType qt = PackageQueueType::FIFO;
    Worker worker_(0, 1, std::make_unique<PackageQueue>(qt));

    Package p1;
    Package p2;
    Package p3;
    worker_.receive_package(std::move(p1));
    worker_.receive_package(std::move(p2));
    worker_.receive_package(std::move(p3));

    IPackageReceiver::const_iterator st_it = worker_.cbegin();
    for(int n=1; n<4; n++) {
        EXPECT_EQ(st_it->get_id(), n);
        st_it++;
    }
}

TEST(WorkerTest,work_test){
    PackageQueueType qt = PackageQueueType::FIFO;

    Worker worker_1 (1, 1, std::make_unique<PackageQueue>(qt));
    Worker worker_2 (2, 2, std::make_unique<PackageQueue>(qt));
    Storehouse store (1, std::make_unique<PackageQueue>(qt));

    ReceiverPreferences stores;
    stores.add_receiver(&store);
    worker_1.receiver_preferences_ = stores;
    worker_2.receiver_preferences_ = stores;

    Package p1;
    Package p2;
    Package p3;
    worker_2.receive_package(std::move(p1));
    //worker_2.receive_package(std::move(p2));
    worker_1.receive_package(std::move(p3));

    for (unsigned int time = 0; time < 4; time++){
        worker_2.do_work(time);
        //worker_2.send_package();
        if(time == 1){
            worker_2.receive_package(std::move(p2));
        }
    }
    EXPECT_EQ(worker_2.get_sending_buffer()->get_id(), 2);
}

TEST(StorehouseTest,receive_test){
    PackageQueueType qt = PackageQueueType::FIFO;
    Worker worker_1 (1, 2, std::make_unique<PackageQueue>(qt));
    Storehouse store (1, std::make_unique<PackageQueue>(qt));

    ReceiverPreferences stores;
    stores.add_receiver(&store);
    worker_1.receiver_preferences_ = stores;

    Package p1;
    Package p2;
    worker_1.receive_package(std::move(p1));
    worker_1.receive_package(std::move(p2));

    for (unsigned int time = 0; time < 4; time++){
        worker_1.do_work(time);
        worker_1.send_package();
    }

    IPackageReceiver::const_iterator st_it = store.cbegin();
    for(int n=1; n<3; n++){
        EXPECT_EQ(st_it->get_id(), n);
        st_it++;
    }
}

TEST(StorehouseTest,get_test){
    ElementID id = 0;
    PackageQueueType qt = PackageQueueType::FIFO;
    Storehouse storehouse_ (id, std::make_unique<PackageQueue>(qt));

    Package p1;
    Package p2;
    Package p3;
    storehouse_.receive_package(std::move(p1));
    storehouse_.receive_package(std::move(p2));
    storehouse_.receive_package(std::move(p3));

    IPackageReceiver::const_iterator st_it = storehouse_.cbegin();
    for(int n=1; n<4; n++){
        EXPECT_EQ(st_it->get_id(), n);
        st_it++;
    }
}
