#include <gmock/gmock-spec-builders.h>
#include "gtest/gtest.h"
#include "storage_types.hpp"
#include "package.hpp"

TEST(PackageQueueTest, get_element_test){
    PackageQueue queue_FIFO(PackageQueueType::FIFO);
    PackageQueue queue_LIFO(PackageQueueType::LIFO);
    Package p1;
    Package p2;
    Package p3;
    Package p4;
    Package p5;
    Package p6;
    queue_FIFO.push(std::move(p1));
    queue_FIFO.push(std::move(p2));
    queue_FIFO.push(std::move(p3));
    queue_LIFO.push(std::move(p4));
    queue_LIFO.push(std::move(p5));
    queue_LIFO.push(std::move(p6));
    Package item1 = queue_FIFO.pop();
    Package item2 = queue_FIFO.pop();
    Package item3 = queue_LIFO.pop();
    Package item4 = queue_LIFO.pop();

    ASSERT_EQ(item1.get_id(),1);
    ASSERT_EQ(item2.get_id(),2);
    ASSERT_EQ(item3.get_id(),6);
    ASSERT_EQ(item4.get_id(),5);
}

TEST(PackageTest, ID_check_test){
    Package item1;
    Package item2;
    Package item3;

    EXPECT_FALSE(item1.get_id() == item2.get_id());
    EXPECT_FALSE(item2.get_id() == item3.get_id());
    EXPECT_FALSE(item1.get_id() == item3.get_id());
}
