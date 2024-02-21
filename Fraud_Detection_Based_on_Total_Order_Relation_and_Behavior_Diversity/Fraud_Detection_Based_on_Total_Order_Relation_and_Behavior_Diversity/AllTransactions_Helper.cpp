#include "AllTransactions.h"

// ----------------------- Helper Functions -----------------------
// Preprocess the data
// Initialize the data into all_transactions_include_same_records
void AllTransactions::PreprocessAllTransactionsInString(AllTransactionsInStringFile* all_transactions_in_string_file)
{
    Transactions transaction;
    TransactionsForUsers transaction_for_single_user;

    for (auto& transaction_in_string : all_transactions_in_string_file->AllTransactionsInString)
    {
        transaction.user_id = transaction_in_string.user_id;
        transaction.time = StringToTime(transaction_in_string.time);
        transaction.order_address = StringToLocation(transaction_in_string.order_address);
        transaction.shipping_address = StringToLocation(transaction_in_string.shipping_address);
        transaction.store_address = StringToLocation(transaction_in_string.store_address);
        transaction.category = StringToCategory(transaction_in_string.category);
        transaction.amount = StringToAmount(transaction_in_string.amount);
        transaction.payment_method = StringToPaymentMethod(transaction_in_string.payment_method);
        transaction.tip_amount = StringToTipAmount(transaction_in_string.tip_amount);
        transaction.delay_time = StringToDelayTime(transaction_in_string.time, transaction_in_string.delay_time);
        transaction.is_using_redeem = StringToBool(transaction_in_string.is_using_redeem);
        transaction.is_pickup = StringToBool(transaction_in_string.is_pickup);

        // Add the UUID to the transaction
        transaction.UUID = UUIDGenerator(transaction);

        // Add the transaction to all_transactions_include_same_records
        auto it = all_users_transactions.find(transaction.user_id);
        // Add transaction to the map
        // Check if the user_id is in the map
        if (it != all_users_transactions.end())
        {
            // Check if there is the same transaction in the vector
            auto it_transaction = std::find(it->second.begin(), it->second.end(), transaction);
            if (it_transaction == it->second.end())
            {
                it->second.push_back(transaction);
            }
            else
            {
                it_transaction->frequency++;
            }
        }
        else
        {
            all_users_transactions.insert({ transaction.user_id, {transaction} });
        }

        // Add the user_id to all_users
        all_users.insert(transaction.user_id);
    }

    // initialize the total number of transactions
    for (auto& user : all_users_transactions)
    {
        total_transactions_number += user.second.size();
    }

    //initialize the total number of unique transactions
    for (auto& user_id : all_users)
    {
        total_unique_transactions_number += all_users_transactions[user_id].size();
    }

    // initialize the total number of users
    total_users_number = all_users.size();

    // Free memory use
    delete all_transactions_in_string_file;
    
    // Sort the transactions by time
    // If the time is same, then remain the relatively order
    // sortTransactionsByTime();
}

// ------------------------ String to enum ------------------------
// #TODO: Output the message to log instead of exit the program
// Categorize the time
// Time format: "HH:MM dd/mm/yyy"
Enumerators::Time AllTransactions::StringToTime(const string& s) const
{
    // Get the time from the string
    string time = s.substr(0, 5);
    int hour = stoi(time.substr(0, 2));
    int minute = stoi(time.substr(3, 2));

    if (hour >= 0 && hour < 12)
    {
        return Enumerators::Time::morning;
    }
    else if (hour >= 12 && hour < 14)
    {
        return Enumerators::Time::noon;
    }
    else if (hour >= 14 && hour < 17)
    {
        return Enumerators::Time::afternoon;
    }
    else if (hour >= 17 && hour < 24)
    {
        return Enumerators::Time::evening;
    }
    else
    {
        cerr << "Invalid time data" << endl;
        // #EXIT
        exit(2);
    }
}

// #TODO: Output the message to log instead of exit the program
// Categorize the location
// location format: "house_number, street_name, str/ave/blvd, city, province postal_code"
Enumerators::Location AllTransactions::StringToLocation(const string& s) const
{
    string city;
    int i = 0;
    while (s[i] != ',')
    {
        i++;
    }
    i += 2;
    while (s[i] != ',')
    {
        city += s[i];
        i++;
    }

    if (city == "vancouver")
    {
        return Enumerators::Location::vancouver;
    }
    else if (city == "north_vancouver")
    {
        return Enumerators::Location::north_vancouver;
    }
    else if (city == "burnaby")
    {
        return Enumerators::Location::burnaby;
    }
    else if (city == "richmond")
    {
        return Enumerators::Location::richmond;
    }
    else if (city == "surrey")
    {
        return Enumerators::Location::surrey;
    }
    else if (city == "new_wedtmindter")
    {
        return Enumerators::Location::new_westminster;
    }
    else if (city == "coquitlam")
    {
        return Enumerators::Location::coquitlam;
    }
    else
    {
        cerr << "Invalid location data" << endl;
        // #EXIT
        exit(3);
    }
}

// #TODO: Output the message to log instead of exit the program
Enumerators::Category AllTransactions::StringToCategory(const string& s) const
{
    if (s == "korean")
    {
        return Enumerators::Category::korean;
    }
    else if (s == "japanese")
    {
        return Enumerators::Category::japanese;
    }
    else if (s == "chinese")
    {
        return Enumerators::Category::chinese;
    }
    else if (s == "western")
    {
        return Enumerators::Category::western;
    }
    else if (s == "asian")
    {
        return Enumerators::Category::asian;
    }
    else if (s == "chicken")
    {
        return Enumerators::Category::chicken;
    }
    else if (s == "vagan")
    {
        return Enumerators::Category::vagan;
    }
    else if (s == "dessert")
    {
        return Enumerators::Category::dessert;
    }
    else if (s == "grocery")
    {
        return Enumerators::Category::grocery;
    }
    else
    {
        cerr << "Invalid category data" << endl;
        // #EXIT
        exit(4);
    }
}

// #TODO: Output the message to log instead of exit the program
// Need to categorize the amount
// Amount format: 0000.00
Enumerators::Amount AllTransactions::StringToAmount(const string& s) const
{
    double amount = stod(s);
    if (amount > 0 && amount <= 10)
    {
        return Enumerators::Amount::a0_10;
    }
    else if (amount > 10 && amount <= 20)
    {
        return Enumerators::Amount::a10_20;
    }
    else if (amount > 20 && amount <= 30)
    {
        return Enumerators::Amount::a20_30;
    }
    else if (amount > 30 && amount <= 40)
    {
        return Enumerators::Amount::a30_40;
    }
    else if (amount > 40 && amount <= 50)
    {
        return Enumerators::Amount::a40_50;
    }
    else if (amount > 50 && amount <= 60)
    {
        return Enumerators::Amount::a50_60;
    }
    else if (amount > 60 && amount <= 70)
    {
        return Enumerators::Amount::a60_70;
    }
    else if (amount > 70 && amount <= 80)
    {
        return Enumerators::Amount::a70_80;
    }
    else if (amount > 80 && amount <= 90)
    {
        return Enumerators::Amount::a80_90;
    }
    else if (amount > 90 && amount <= 100)
    {
        return Enumerators::Amount::a90_100;
    }
    else if (amount > 100)
    {
        return Enumerators::Amount::aover_100;
    }
    else
    {
        cerr << "Invalid amount data" << endl;
        // #EXIT
        exit(5);
    }
}

Enumerators::PaymentMethod AllTransactions::StringToPaymentMethod(const string& s) const
{
    if (s == "credit_card")
    {
        return Enumerators::PaymentMethod::credit_card;
    }
    else if (s == "debit_card")
    {
        return Enumerators::PaymentMethod::debit_card;
    }
    else
    {
        cerr << "Invalid payment method data" << endl;
        // #EXIT
        exit(6);
    }
}

// #TODO: Output the message to log instead of exit the program
// Categorize the tip amount
Enumerators::TipAmount AllTransactions::StringToTipAmount(const string& s) const
{
    double tip_amount = stod(s);
    if (tip_amount > 0 && tip_amount <= 5)
    {
        return Enumerators::TipAmount::ta0_5;
    }
    else if (tip_amount > 5 && tip_amount <= 10)
    {
        return Enumerators::TipAmount::ta5_10;
    }
    else if (tip_amount > 10 && tip_amount <= 15)
    {
        return Enumerators::TipAmount::ta10_15;
    }
    else if (tip_amount > 15 && tip_amount <= 20)
    {
        return Enumerators::TipAmount::ta15_20;
    }
    else if (tip_amount > 20)
    {
        return Enumerators::TipAmount::taover_20;
    }
    else
    {
        cerr << "Invalid tip amount data" << endl;
        // #EXIT
        exit(7);
    }
}

// #NEED_CHANGE
// Categorize the delay time
// DelayTime format: "hh:mm dd/mm/yyyy"
Enumerators::DelayTime AllTransactions::StringToDelayTime(const string& begin_time, const string& end_time) const
{
    string b_day = end_time.substr(9, 2);
    string e_day = end_time.substr(9, 2);
    int day_diff = stoi(e_day) - stoi(b_day);

    string b_time = begin_time.substr(0, 2);
    string e_time = end_time.substr(0, 2);
    int hour_diff = stoi(e_time) - stoi(b_time) + day_diff * 24;
    if (hour_diff >= 0 && hour_diff <= 2)
    {
        return Enumerators::DelayTime::dt0_2;
    }
    else if (hour_diff > 2 && hour_diff <= 4)
    {
        return Enumerators::DelayTime::dt2_4;
    }
    else if (hour_diff > 4 && hour_diff <= 6)
    {
        return Enumerators::DelayTime::dt4_6;
    }
    else if (hour_diff > 6 && hour_diff <= 8)
    {
        return Enumerators::DelayTime::dt6_8;
    }
    else if (hour_diff > 8 && hour_diff <= 10)
    {
        return Enumerators::DelayTime::dt8_10;
    }
    else if (hour_diff > 10 && hour_diff <= 12)
    {
        return Enumerators::DelayTime::dt10_12;
    }
    else if (hour_diff > 12 && hour_diff <= 14)
    {
        return Enumerators::DelayTime::dt12_14;
    }
    else if (hour_diff > 14 && hour_diff <= 16)
    {
        return Enumerators::DelayTime::dt14_16;
    }
    else if (hour_diff > 16 && hour_diff <= 18)
    {
        return Enumerators::DelayTime::dt16_18;
    }
    else if (hour_diff > 18 && hour_diff <= 20)
    {
        return Enumerators::DelayTime::dt18_20;
    }
    else if (hour_diff > 20 && hour_diff <= 22)
    {
        return Enumerators::DelayTime::dt20_22;
    }
    else if (hour_diff > 22 && hour_diff <= 24)
    {
        return Enumerators::DelayTime::dt22_24;
    }
    else if (hour_diff > 24 && hour_diff <= 26)
    {
        return Enumerators::DelayTime::dt24_26;
    }
    else if (hour_diff > 26 && hour_diff <= 28)
    {
        return Enumerators::DelayTime::dt26_28;
    }
    else if (hour_diff > 28 && hour_diff <= 30)
    {
        return Enumerators::DelayTime::dt28_30;
    }
    else if (hour_diff > 30 && hour_diff <= 32)
    {
        return Enumerators::DelayTime::dt30_32;
    }
    else if (hour_diff > 32 && hour_diff <= 34)
    {
        return Enumerators::DelayTime::dt32_34;
    }
    else if (hour_diff > 34 && hour_diff <= 36)
    {
        return Enumerators::DelayTime::dt34_36;
    }
    else if (hour_diff > 36 && hour_diff <= 38)
    {
        return Enumerators::DelayTime::dt36_38;
    }
    else if (hour_diff > 38 && hour_diff <= 40)
    {
        return Enumerators::DelayTime::dt38_40;
    }
    else if (hour_diff > 40 && hour_diff <= 42)
    {
        return Enumerators::DelayTime::dt40_42;
    }
    else if (hour_diff > 42 && hour_diff <= 44)
    {
        return Enumerators::DelayTime::dt42_44;
    }
    else if (hour_diff > 44 && hour_diff <= 46)
    {
        return Enumerators::DelayTime::dt44_46;
    }
    else if (hour_diff > 46 && hour_diff <= 48)
    {
        return Enumerators::DelayTime::dt46_48;
    }
    else if (hour_diff > 48 && hour_diff <= 50)
    {
        return Enumerators::DelayTime::dt48_50;
    }
    else if (hour_diff > 50 && hour_diff <= 52)
    {
        return Enumerators::DelayTime::dt50_52;
    }
    else if (hour_diff > 52 && hour_diff <= 54)
    {
        return Enumerators::DelayTime::dt52_54;
    }
    else if (hour_diff > 54 && hour_diff <= 56)
    {
        return Enumerators::DelayTime::dt54_56;
    }
    else if (hour_diff > 56 && hour_diff <= 58)
    {
        return Enumerators::DelayTime::dt56_58;
    }
    else if (hour_diff > 58 && hour_diff <= 60)
    {
        return Enumerators::DelayTime::dt58_60;
    }
    else if (hour_diff > 60 && hour_diff <= 62)
    {
        return Enumerators::DelayTime::dt60_62;
    }
    else if (hour_diff > 62 && hour_diff <= 64)
    {
        return Enumerators::DelayTime::dt62_64;
    }
    else if (hour_diff > 64 && hour_diff <= 66)
    {
        return Enumerators::DelayTime::dt64_66;
    }
    else if (hour_diff > 66 && hour_diff <= 68)
    {
        return Enumerators::DelayTime::dt66_68;
    }
    else if (hour_diff > 68 && hour_diff <= 70)
    {
        return Enumerators::DelayTime::dt68_70;
    }
    else if (hour_diff > 70 && hour_diff <= 72)
    {
        return Enumerators::DelayTime::dt70_72;
    }
    else
    {
        cerr << "Invalid delay time data" << endl;
        // #EXIT
        exit(8);
    }
}

bool AllTransactions::StringToBool(const string& s) const
{
    if (s == "true")
    {
        return true;
    }
    else if (s == "false")
    {
        return false;
    }
    else
    {
        cerr << "Invalid bool data" << endl;
        // #EXIT
        exit(9);
    }
}

// ----------------------------------------------------------------

string AllTransactions::UUIDGenerator(const Transactions& transaction)
{
    string UUID = "";

    // Transform the user_id to all upper case
    // Append the user_id to the UUID
    string user_id = transaction.user_id;
    std::transform(user_id.begin(), user_id.end(), user_id.begin(),
        [](unsigned char c) -> unsigned char { return std::toupper(c); });
    UUID += user_id;

    // Append the time to the UUID
    switch (transaction.time)
    {
    case Enumerators::Time::morning:
        UUID += "M";
        break;
    case Enumerators::Time::noon:
        UUID += "N";
        break;
    case Enumerators::Time::afternoon:
        UUID += "A";
        break;
    case Enumerators::Time::evening:
        UUID += "E";
        break;
    default:
        cerr << "Invalid time data" << endl;
        // #EXIT
        exit(2);
    }

    // Append the order_address to the UUID
    switch (transaction.order_address)
    {
    case Enumerators::Location::vancouver:
        UUID += "V";
        break;
    case Enumerators::Location::north_vancouver:
        UUID += "N";
        break;
    case Enumerators::Location::burnaby:
        UUID += "B";
        break;
    case Enumerators::Location::richmond:
        UUID += "R";
        break;
    case Enumerators::Location::surrey:
        UUID += "S";
        break;
    case Enumerators::Location::new_westminster:
        UUID += "W";
        break;
    case Enumerators::Location::coquitlam:
        UUID += "C";
        break;
    default:
        cerr << "Invalid order address data" << endl;
        // #EXIT
        exit(3);
    }

    // Append the shipping_address to the UUID
    switch (transaction.shipping_address)
    {
    case Enumerators::Location::vancouver:
        UUID += "V";
        break;
    case Enumerators::Location::north_vancouver:
        UUID += "N";
        break;
    case Enumerators::Location::burnaby:
        UUID += "B";
        break;
    case Enumerators::Location::richmond:
        UUID += "R";
        break;
    case Enumerators::Location::surrey:
        UUID += "S";
        break;
    case Enumerators::Location::new_westminster:
        UUID += "W";
        break;
    case Enumerators::Location::coquitlam:
        UUID += "C";
        break;
    default:
        cerr << "Invalid shipping address data" << endl;
        // #EXIT
        exit(3);
    }

    // Append the store_address to the UUID
    switch (transaction.store_address)
    {
    case Enumerators::Location::vancouver:
        UUID += "V";
        break;
    case Enumerators::Location::north_vancouver:
        UUID += "N";
        break;
    case Enumerators::Location::burnaby:
        UUID += "B";
        break;
    case Enumerators::Location::richmond:
        UUID += "R";
        break;
    case Enumerators::Location::surrey:
        UUID += "S";
        break;
    case Enumerators::Location::new_westminster:
        UUID += "W";
        break;
    case Enumerators::Location::coquitlam:
        UUID += "C";
        break;
    default:
        cerr << "Invalid store address data" << endl;
        // #EXIT
        exit(3);
    }

    // Append the category to the UUID
    switch (transaction.category)
    {
    case Enumerators::Category::korean:
        UUID += "KO";
        break;
    case Enumerators::Category::japanese:
        UUID += "JA";
        break;
    case Enumerators::Category::chinese:
        UUID += "CH";
        break;
    case Enumerators::Category::western:
        UUID += "WE";
        break;
    case Enumerators::Category::asian:
        UUID += "AS";
        break;
    case Enumerators::Category::chicken:
        UUID += "CH";
        break;
    case Enumerators::Category::vagan:
        UUID += "VA";
        break;
    case Enumerators::Category::dessert:
        UUID += "DE";
        break;
    case Enumerators::Category::grocery:
        UUID += "GR";
        break;
    default:
        cerr << "Invalid category data" << endl;
        // #EXIT
        exit(4);
    }

    // Append the is_pickup to the UUID
    switch (transaction.is_pickup)
    {
    case true:
        UUID += "1";
        break;
    case false:
        UUID += "0";
        break;
    default:
        cerr << "Invalid is pickup data" << endl;
        // #EXIT
        exit(9);
    }

    // Append the is_using_redeem to the UUID
    switch (transaction.is_using_redeem)
    {
    case true:
        UUID += "1";
        break;
    case false:
        UUID += "0";
        break;
    default:
        cerr << "Invalid is using redeem data" << endl;
        // #EXIT
        exit(9);
    }

    // Append the amount to the UUID
    switch (transaction.amount)
    {
    case Enumerators::Amount::a0_10:
        UUID += "00";
        break;
    case Enumerators::Amount::a10_20:
        UUID += "10";
        break;
    case Enumerators::Amount::a20_30:
        UUID += "20";
        break;
    case Enumerators::Amount::a30_40:
        UUID += "30";
        break;
    case Enumerators::Amount::a40_50:
        UUID += "40";
        break;
    case Enumerators::Amount::a50_60:
        UUID += "50";
        break;
    case Enumerators::Amount::a60_70:
        UUID += "60";
        break;
    case Enumerators::Amount::a70_80:
        UUID += "70";
        break;
    case Enumerators::Amount::a80_90:
        UUID += "80";
        break;
    case Enumerators::Amount::a90_100:
        UUID += "90";
        break;
    case Enumerators::Amount::aover_100:
        UUID += "01";
        break;
    default:
        cerr << "Invalid amount data" << endl;
        // #EXIT
        exit(5);
    }

    // Append the tip_amount to the UUID
    switch (transaction.tip_amount)
    {
    case Enumerators::TipAmount::ta0_5:
        UUID += "00";
        break;
    case Enumerators::TipAmount::ta10_15:
        UUID += "10";
        break;
    case Enumerators::TipAmount::ta15_20:
        UUID += "15";
        break;
    case Enumerators::TipAmount::taover_20:
        UUID += "20";
        break;
    default:
        cerr << "Invalid tip amount data" << endl;
        // #EXIT
        exit(7);
    }

    // Append the delay_time to the UUID
    // Following the enum DelayTime to append the delay_time to the UUID
    switch (transaction.delay_time)
    {
    case Enumerators::DelayTime::dt0_2:
        UUID += "00";
        break;
    case Enumerators::DelayTime::dt2_4:
        UUID += "02";
        break;
    case Enumerators::DelayTime::dt4_6:
        UUID += "04";
        break;
    case Enumerators::DelayTime::dt6_8:
        UUID += "06";
        break;
    case Enumerators::DelayTime::dt8_10:
        UUID += "08";
        break;
    case Enumerators::DelayTime::dt10_12:
        UUID += "10";
        break;
    case Enumerators::DelayTime::dt12_14:
        UUID += "12";
        break;
    case Enumerators::DelayTime::dt14_16:
        UUID += "14";
        break;
    case Enumerators::DelayTime::dt16_18:
        UUID += "16";
        break;
    case Enumerators::DelayTime::dt18_20:
        UUID += "18";
        break;
    case Enumerators::DelayTime::dt20_22:
        UUID += "20";
        break;
    case Enumerators::DelayTime::dt22_24:
        UUID += "22";
        break;
    case Enumerators::DelayTime::dt24_26:
        UUID += "24";
        break;
    case Enumerators::DelayTime::dt26_28:
        UUID += "26";
        break;
    case Enumerators::DelayTime::dt28_30:
        UUID += "28";
        break;
    case Enumerators::DelayTime::dt30_32:
        UUID += "30";
        break;
    case Enumerators::DelayTime::dt32_34:
        UUID += "32";
        break;
    case Enumerators::DelayTime::dt34_36:
        UUID += "34";
        break;
    case Enumerators::DelayTime::dt36_38:
        UUID += "36";
        break;
    case Enumerators::DelayTime::dt38_40:
        UUID += "38";
        break;
    case Enumerators::DelayTime::dt40_42:
        UUID += "40";
        break;
    case Enumerators::DelayTime::dt42_44:
        UUID += "42";
        break;
    case Enumerators::DelayTime::dt44_46:
        UUID += "44";
        break;
    case Enumerators::DelayTime::dt46_48:
        UUID += "46";
        break;
    case Enumerators::DelayTime::dt48_50:
        UUID += "48";
        break;
    case Enumerators::DelayTime::dt50_52:
        UUID += "50";
        break;
    case Enumerators::DelayTime::dt52_54:
        UUID += "52";
        break;
    case Enumerators::DelayTime::dt54_56:
        UUID += "54";
        break;
    case Enumerators::DelayTime::dt56_58:
        UUID += "56";
        break;
    case Enumerators::DelayTime::dt58_60:
        UUID += "58";
        break;
    case Enumerators::DelayTime::dt60_62:
        UUID += "60";
        break;
    case Enumerators::DelayTime::dt62_64:
        UUID += "62";
        break;
    case Enumerators::DelayTime::dt64_66:
        UUID += "64";
        break;
    case Enumerators::DelayTime::dt66_68:
        UUID += "66";
        break;
    case Enumerators::DelayTime::dt68_70:
        UUID += "68";
        break;
    case Enumerators::DelayTime::dt70_72:
        UUID += "70";
        break;
    default:
        cerr << "Invalid delay time data" << endl;
        exit(8);
    }

    // Append the payment_method to the UUID
    switch (transaction.payment_method)
    {
    case Enumerators::PaymentMethod::credit_card:
        UUID += "C";
        break;
    case Enumerators::PaymentMethod::debit_card:
        UUID += "D";
        break;
    default:
        cerr << "Invalid payment method data" << endl;
        // #EXIT
        exit(6);
    }

    // Append the frequency to the UUID
    if(transaction.frequency > 0 && transaction.frequency < 10)
    {
        UUID += std::to_string(transaction.frequency);
    }
    else if(transaction.frequency >= 10 && transaction.frequency <= 35)
    {
        UUID += (transaction.frequency + 31);
    }
    else
    {
        cerr << "Invalid frequency data" << endl;
        // #EXIT
        exit(19);
    }

    return UUID;
}

void AllTransactions::RemoveDuplicateRecords()
{
    for (auto& user : all_users_transactions)
    {
        std::vector<Transactions> unique_transactions;

        for (auto& transaction : user.second)
        {
            auto it = all_users_transactions[user.first].find(transaction);
            // When the transaction not found in the set
            if (it == all_users_transactions[user.first].end())
            {
                all_users_transactions.insert({ user.first, {transaction} });
                total_unique_transactions_number++;
            }
        }

        user.second = unique_transactions;
        total_unique_transactions_number += user.second.size();
    }
}

void AllTransactions::sortTransactionsByTime()
{
    vector<Transactions> morning_transactions;
    vector<Transactions> noon_transactions;
    vector<Transactions> afternoon_transactions;
    vector<Transactions> evening_transactions;
    for (auto& user_id : all_users)
    {
        for (auto& transaction_it : all_users_transactions.at(user_id))
        {
            if (transaction_it.time == Enumerators::Time::morning)
            {
                morning_transactions.push_back(transaction_it);
            }
            else if (transaction_it.time == Enumerators::Time::noon)
            {
                noon_transactions.push_back(transaction_it);
            }
            else if (transaction_it.time == Enumerators::Time::afternoon)
            {
                afternoon_transactions.push_back(transaction_it);
            }
            else if (transaction_it.time == Enumerators::Time::evening)
            {
                evening_transactions.push_back(transaction_it);
            }
        }

        vector<Transactions> result;
        result.insert(result.end(), morning_transactions.begin(), morning_transactions.end());
        result.insert(result.end(), noon_transactions.begin(), noon_transactions.end());
        result.insert(result.end(), afternoon_transactions.begin(), afternoon_transactions.end());
        result.insert(result.end(), evening_transactions.begin(), afternoon_transactions.end());

        all_users_transactions.at(user_id).clear();
        all_users_transactions.at(user_id) = result;
    }
}


string AllTransactions::intToAttributeString(int i, int attribute_level) const
{
    switch (attribute_level)
    {
        case 1:
            return AttributesToString(static_cast<Enumerators::Time>(i));
        case 2:
            return AttributesToString(static_cast<Enumerators::Location>(i));
        case 3:
            return AttributesToString(static_cast<Enumerators::Location>(i));
        case 4:
            return AttributesToString(static_cast<Enumerators::Location>(i));
        case 5:
            return AttributesToString(static_cast<Enumerators::Category>(i));
        case 6:
            return AttributesToString(static_cast<bool>(i));
        case 7:
            return AttributesToString(static_cast<bool>(i));
        case 8:
            return AttributesToString(static_cast<Enumerators::Amount>(i));
        case 9:
            return AttributesToString(static_cast<Enumerators::TipAmount>(i));
        case 10:
            return AttributesToString(static_cast<Enumerators::PaymentMethod>(i));
        case 11:
            return AttributesToString(static_cast<Enumerators::DelayTime>(i));
        default:
            cerr << "Invalid attribute level" << endl;
            // #EXIT
            exit(20);
    }
}


// ------------------------ Enum to string ------------------------
string AllTransactions::AttributesToString(const Enumerators::Time& t) const
{
    if (t == Enumerators::Time::morning)
    {
        return "morning";
    }
    else if (t == Enumerators::Time::noon)
    {
        return "noon";
    }
    else if (t == Enumerators::Time::afternoon)
    {
        return "afternoon";
    }
    else if (t == Enumerators::Time::evening)
    {
        return "evening";
    }
    else
    {
        cerr << "Invalid time data" << endl;
        // #EXIT
        exit(10);
    }
}

string AllTransactions::AttributesToString(const Enumerators::Location& l) const
{
    if (l == Enumerators::Location::vancouver)
    {
        return "vancouver";
    }
    else if (l == Enumerators::Location::north_vancouver)
    {
        return "north_vancouver";
    }
    else if (l == Enumerators::Location::burnaby)
    {
        return "burnaby";
    }
    else if (l == Enumerators::Location::richmond)
    {
        return "richmond";
    }
    else if (l == Enumerators::Location::surrey)
    {
        return "surrey";
    }
    else if (l == Enumerators::Location::new_westminster)
    {
        return "new_wedtmindter";
    }
    else if (l == Enumerators::Location::coquitlam)
    {
        return "coquitlam";
    }
    else
    {
        cerr << "Invalid location data" << endl;
        // #EXIT
        exit(11);
    }
}

string AllTransactions::AttributesToString(const Enumerators::Category& c) const
{
    if (c == Enumerators::Category::korean)
    {
        return "korean";
    }
    else if (c == Enumerators::Category::japanese)
    {
        return "japanese";
    }
    else if (c == Enumerators::Category::chinese)
    {
        return "chinese";
    }
    else if (c == Enumerators::Category::western)
    {
        return "western";
    }
    else if (c == Enumerators::Category::asian)
    {
        return "asian";
    }
    else if (c == Enumerators::Category::chicken)
    {
        return "chicken";
    }
    else if (c == Enumerators::Category::vagan)
    {
        return "vagan";
    }
    else if (c == Enumerators::Category::dessert)
    {
        return "dessert";
    }
    else if (c == Enumerators::Category::grocery)
    {
        return "grocery";
    }
    else
    {
        cerr << "Invalid category data" << endl;
        // #EXIT
        exit(12);
    }
}

string AllTransactions::AttributesToString(const Enumerators::Amount& a) const
{
    if (a == Enumerators::Amount::a0_10)
    {
        return "a0_10";
    }
    else if (a == Enumerators::Amount::a10_20)
    {
        return "a10_20";
    }
    else if (a == Enumerators::Amount::a20_30)
    {
        return "a20_30";
    }
    else if (a == Enumerators::Amount::a30_40)
    {
        return "a30_40";
    }
    else if (a == Enumerators::Amount::a40_50)
    {
        return "a40_50";
    }
    else if (a == Enumerators::Amount::a50_60)
    {
        return "a50_60";
    }
    else if (a == Enumerators::Amount::a60_70)
    {
        return "a60_70";
    }
    else if (a == Enumerators::Amount::a70_80)
    {
        return "a70_80";
    }
    else if (a == Enumerators::Amount::a80_90)
    {
        return "a80_90";
    }
    else if (a == Enumerators::Amount::a90_100)
    {
        return "a90_100";
    }
    else
    {
        cerr << "Invalid amount data" << endl;
        // #EXIT
        exit(13);
    }
}

string AllTransactions::AttributesToString(const Enumerators::PaymentMethod& p) const
{
    if (p == Enumerators::PaymentMethod::credit_card)
    {
        return "credit_card";
    }
    else if (p == Enumerators::PaymentMethod::debit_card)
    {
        return "debit_card";
    }
    else
    {
        cerr << "Invalid payment method data" << endl;
        // #EXIT
        exit(14);
    }
}

string AllTransactions::AttributesToString(const Enumerators::TipAmount& t) const
{
    if (t == Enumerators::TipAmount::ta0_5)
    {
        return "ta0_5";
    }
    else if (t == Enumerators::TipAmount::ta5_10)
    {
        return "ta5_10";
    }
    else if (t == Enumerators::TipAmount::ta10_15)
    {
        return "ta10_15";
    }
    else if (t == Enumerators::TipAmount::ta15_20)
    {
        return "ta15_20";
    }
    else
    {
        cerr << "Invalid tip amount data" << endl;
        // #EXIT
        exit(15);
    }
}

string AllTransactions::AttributesToString(const Enumerators::DelayTime& d) const
{
    if (d == Enumerators::DelayTime::dt0_2)
    {
        return "dt0_2";
    }
    else if (d == Enumerators::DelayTime::dt2_4)
    {
        return "dt2_4";
    }
    else if (d == Enumerators::DelayTime::dt4_6)
    {
        return "dt4_6";
    }
    else
    {
        cerr << "Invalid delay time data" << endl;
        // #EXIT
        exit(16);
    }
}

string AllTransactions::AttributesToString(const bool& b) const
{
    return std::to_string(b);
}


string AllTransactions::AttributesToString(const int& b) const
{
    return std::to_string(b);
}
