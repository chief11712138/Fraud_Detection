#include "AllTransactions.h"

// Initialize attributes_numebr
const map<int, int> AllTransactions::attributes_number = {
        {1, static_cast<int>(Enumerators::Time::TimeNum)},
        {2, static_cast<int>(Enumerators::Location::LocationNum)},
        {3, static_cast<int>(Enumerators::Location::LocationNum)},
        {4, static_cast<int>(Enumerators::Location::LocationNum)},
        {5, static_cast<int>(Enumerators::Category::CategoryNum)},
        {6, 2},
        {7, 2},
        {8, static_cast<int>(Enumerators::Amount::AmountNum)},
        {9, static_cast<int>(Enumerators::TipAmount::TipAmountNum)},
        {10, static_cast<int>(Enumerators::DelayTime::DelayTimeNum)},
        {11, static_cast<int>(Enumerators::PaymentMethod::PaymentMethodNum)},
};

AllTransactions::AllTransactions()
{
    total_transactions_number = 0;
    total_unique_transactions_number = 0;

    Initialize();
}
