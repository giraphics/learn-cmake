#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

std::mutex cout_mutex;

void worker(int id) {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "Worker " << id << " running on thread "
              << std::this_thread::get_id() << std::endl;
}

int main() {
    std::cout << "Imported Targets Example" << std::endl;
    std::cout << "Using modern CMake imported targets" << std::endl;

    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(worker, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "All workers finished!" << std::endl;
    return 0;
}
