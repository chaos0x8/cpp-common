#include <GetCurrentYear.hpp>
#include <chrono>

namespace Common
{

uint32_t getCurrentYear()
{
    using namespace std::chrono;

    system_clock::time_point now = system_clock::now();
    time_t tt = system_clock::to_time_t(now);
    tm local_time = *localtime(&tt);

    return static_cast<uint32_t>(local_time.tm_year + 1900u);
}

}
