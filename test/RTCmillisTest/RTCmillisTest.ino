#include <chrono>

using namespace std::chrono;

// get the number of oscillator periods since 1970-01-01
uint64_t get_RTC_periods()
{
    uint32_t hi1 = SNVS_HPRTCMR, lo1 = SNVS_HPRTCLR;
    while (true)
    {
        uint32_t hi2 = SNVS_HPRTCMR, lo2 = SNVS_HPRTCLR;
        if (lo1 == lo2 && hi1 == hi2)
        {
            return (uint64_t)hi2 << 32 | lo2;
        }
        hi1 = hi2;
        lo1 = lo2;
    }
}

// this is all you need to do to prepare the built in system_clock for usage with the RTC (resolution ~30.5µs)
system_clock::time_point system_clock::now()
{
    uint64_t ns = get_RTC_periods() * (1E9 / 32768); // system clock uses ns as time base (would be better to define dedicated clock with osc periods as base)
    return system_clock::time_point(system_clock::duration(ns));
}

using timePoint = system_clock::time_point; // alias to save typing...

//-----------------------------

void setup()
{
    while (!Serial) {}
}

void loop()
{
    constexpr uint32_t dt          = 100'000; // print timestamp every 'dt' microseconds
    static elapsedMicros stopwatch = dt;

    if (stopwatch > dt)
    {
        stopwatch -= dt;

        timePoint now = system_clock::now(); // get the current time from the system clock with a resolution of 30.5µs (1/32.768kHz)

        // currently (c++14) std::chrono  doesn't provide much support for transformation of chrono::timepoints to
        // years, day of month etc. c++20 would improve this a lot, but alas...
        // Meanwhile we can work around using the traditional time_t and struct tm to handle this

        time_t rawTime = system_clock::to_time_t(now); // convert the std::chrono time_point to a traditional time_t value
        tm t           = *gmtime(&rawTime);            // caluclate year, month... from the raw time_t value and store them in a tm struct
        int year       = t.tm_year + 1900;
        int month      = t.tm_mon + 1;
        int day        = t.tm_mday;
        int hour       = t.tm_hour;
        int minute     = t.tm_min;
        int second     = t.tm_sec;

        // since time_t is 'seconds since 1970-01-01', this of course can't handle smaller time units.
        // But we use the teensy_clock directly for this:

        int ms = duration_cast<milliseconds>(now.time_since_epoch()).count() % 1'000;
        Serial.printf("%d-%02d-%02d %02d:%02d:%02d %03u\n", year, month, day, hour, minute, second, ms);
    }
}
