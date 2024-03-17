// All the specific transaction data shuold be process to the enums below 

#pragma once

namespace Enumerators
{
    // #Exit code: 1
    enum class Time
    {
        morning, // 04:00 - 12:00
        noon, // 12:00 - 14:00
        afternoon, // 14:00 - 17:00
        night, // 17:00 - 04:00 (next day)
        TimeNum,
    };

    // #Exit code: 2
    enum class Amount
    {
        // Amount (a,b]
        a0_20,
        a20_40,
        a40_60,
        a60_80,
        a80_100,
        aover_100,
        AmountNum,
    };
}