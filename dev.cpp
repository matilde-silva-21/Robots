#include <iostream>
#include <chrono>

using namespace std::chrono;

class timer
{

public:
    auto start()
    {
        auto start = std::chrono::high_resolution_clock::now();
        return start;
    }

    auto stop()
    {
        auto stop = std::chrono::high_resolution_clock::now();
        return stop;
    }
    auto total()
    {
        auto total = std::chrono::duration_cast<std::chrono::microseconds>(stop() - start());
        return total;
    }
};

int main()
{
    timer t;

    std::cout << (t.start());

    return 0;
}
