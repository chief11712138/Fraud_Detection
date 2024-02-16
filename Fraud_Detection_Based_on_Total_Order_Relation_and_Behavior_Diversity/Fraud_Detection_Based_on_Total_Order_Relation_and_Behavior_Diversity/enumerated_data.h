// All the specific transaction data shuold be process to the enums below 

#pragma once
#include <iostream>
#include <string>

using std::string;

enum class Time
{
    morning, // 8:00 - 12:00
    noon, // 12:00 - 14:00
    afternoon, // 14:00 - 17:00
    evening, // 17:00 - 22:00
};

// Location will be used as login location and shipping location and store address
enum class Location
{
    vancouver,
    north_vancouver,
    burnaby,
    richmond,
    surrey,
    new_wedtmindter,
    coquitlam,
};

// Food category may various for one dishes
enum class Category
{
    // Will be decided by the food list
    korean,
    japanese,
    chinese,
    wedtern,
    asian,
    chicken,
    vagan,
    dessert,
    grocery,
};

enum class Amount
{
    // Amount (a,b]
    a0_10,
    a10_20,
    a20_30,
    a30_40,
    a40_50,
    a50_60,
    a60_70,
    a70_80,
    a80_90,
    a90_100,
};

enum class PaymentMethod
{
    credit_card,
    debit_card,
};

enum class TipAmount
{
    // Tip amount (a,b]
    ta0_5,
    ta5_10,
    ta10_15,
    ta15_20,
};

enum class DelayTime
{
    // hours dtA_B: (A,B] delay time since the payment
    dt0_2,
    dt2_4,
    dt4_6,
    dt6_8,
    dt8_10,
    dt10_12,
    dt12_14,
    dt14_16,
    dt16_18,
    dt18_20,
    dt20_22,
    dt22_24,
    dt24_26,
    dt26_28,
    dt28_30,
    dt30_32,
    dt32_34,
    dt34_36,
    dt36_38,
    dt38_40,
    dt40_42,
    dt42_44,
    dt44_46,
    dt46_48,
    dt48_50,
    dt50_52,
    dt52_54,
    dt54_56,
    dt56_58,
    dt58_60,
    dt60_62,
    dt62_64,
    dt64_66,
    dt66_68,
    dt68_70,
    dt70_72,
};