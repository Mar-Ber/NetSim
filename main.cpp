#include <iostream>
#include "nodes.hpp"

int main() {

    Worker w1(1,1,std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    return 0;
}