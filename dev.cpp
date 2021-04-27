#include <iostream>
#include <chrono>
#include <iomanip>
#include <cmath>

class StopWatch
{
public: //Atributos
    bool counting = 0;
    std::chrono::_V2::steady_clock::time_point start_time, stop_time;
    std::chrono::duration<double> delta_t;
    bool SECONDS = 0;
    bool MILISECONDS = 0;
    bool MICROSECONDS = 0;
    bool NANOSECONDS = 0;

public: // Métodos
    void clear_settings()
    {
        SECONDS = 0;
        MILISECONDS = 0;
        MICROSECONDS = 0;
        NANOSECONDS = 0;
    }
    void set(bool &conversion)
    {
        clear_settings();
        conversion = 1;
    }

    double convert(std::chrono::duration<double> durationtime)
    {
        double in_seconds = static_cast<double>(durationtime.count());
        if (SECONDS)
        {
            return static_cast<double>(durationtime.count());
        }
        else if (MILISECONDS)
        {
            return in_seconds * pow(10, 3);
        }
        else if (MICROSECONDS)
        {
            return in_seconds * pow(10, 6);
        }
        else if (NANOSECONDS)
        {
            return in_seconds * pow(10, 9);
        }
        return in_seconds;
    }

    double show()
    {
        return convert(delta_t);
    }

    void start()
    {
        counting = 1;
        start_time = std::chrono::steady_clock::now();
    }

    void stop()
    {
        if (counting)
        {
            stop_time = std::chrono::steady_clock::now();
            delta_t = stop_time - start_time;
            counting = 0;
        }
    }
};

unsigned long long fib(int n)
{

    if (n <= 1)
        return 1;

    else
        return fib(n - 1) + fib(n - 2);
}

int main()
{

    StopWatch relogio_da_lara;
    relogio_da_lara.start();
    for (int i = 0; i < 40; i++)
    {
        std::cout << i << " -> " << fib(i) << std::endl;
    }

    std::cout << std::fixed << std::setprecision(30);
    relogio_da_lara.stop();
    relogio_da_lara.set(relogio_da_lara.SECONDS);
    std::cout << relogio_da_lara.show() << std::endl;

    relogio_da_lara.set(relogio_da_lara.MILISECONDS);
    std::cout << relogio_da_lara.show() << std::endl;
    relogio_da_lara.set(relogio_da_lara.MICROSECONDS);
    std::cout << relogio_da_lara.show() << std::endl;
    relogio_da_lara.set(relogio_da_lara.NANOSECONDS);
    std::cout << relogio_da_lara.show() << std::endl;
}