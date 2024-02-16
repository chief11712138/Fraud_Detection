#include "AllTransactions.h"

using nlohmann::json;


AllTransactions::AllTransactions()
{
    total_transactions_number = 0;
    total_unique_transactions_number = 0;

    Initialize();
}

// #UNTEST
void const AllTransactions::printTransactionsToJsonFile(const string& filename = "All_Transactions.json")
{
    // Serialize the data
    json jAllTransactions;
    AllTransactionsInStringFile all_transactions_in_string_file;
    for (auto& user : all_transactions_include_same_records)
    {
        for (auto& transaction : user.second)
        {
            TransactionsInString transaction_in_string;
            transaction_in_string.user_id = transaction.user_id;
            transaction_in_string.time = TimeToString(transaction.time);
            transaction_in_string.order_address = LocationToString(transaction.order_address);
            transaction_in_string.shipping_address = LocationToString(transaction.shipping_address);
            transaction_in_string.store_address = LocationToString(transaction.store_address);
            transaction_in_string.category = CategoryToString(transaction.category);
            transaction_in_string.amount = AmountToString(transaction.amount);
            transaction_in_string.payment_method = PaymentMethodToString(transaction.payment_method);
            transaction_in_string.tip_amount = TipAmountToString(transaction.tip_amount);
            transaction_in_string.delay_time = DelayTimeToString(transaction.delay_time);
            transaction_in_string.is_using_redeem = BoolToString(transaction.is_using_redeem);
            transaction_in_string.is_pickup = BoolToString(transaction.is_pickup);

            all_transactions_in_string_file.AllTransactionsInString.push_back(transaction_in_string);
        }
    }
    jAllTransactions = all_transactions_in_string_file;

    // Write the json file
    std::ofstream outAllTransactions(filename);
    outAllTransactions << jAllTransactions;
    outAllTransactions.close();
}

// #UNTEST
void const AllTransactions::printUserTransactionsToJsonFile(const string& user_id, const string& filename = "User_transactions.json")
{
    // Serialize the data
    json jUserTransactions;
    AllTransactionsInStringFile user_transactions_in_string_file;
    for (auto& transaction : all_transactions_include_same_records[user_id])
    {
        TransactionsInString transaction_in_string;
        transaction_in_string.user_id = transaction.user_id;
        transaction_in_string.time = TimeToString(transaction.time);
        transaction_in_string.order_address = LocationToString(transaction.order_address);
        transaction_in_string.shipping_address = LocationToString(transaction.shipping_address);
        transaction_in_string.store_address = LocationToString(transaction.store_address);
        transaction_in_string.category = CategoryToString(transaction.category);
        transaction_in_string.amount = AmountToString(transaction.amount);
        transaction_in_string.payment_method = PaymentMethodToString(transaction.payment_method);
        transaction_in_string.tip_amount = TipAmountToString(transaction.tip_amount);
        transaction_in_string.delay_time = DelayTimeToString(transaction.delay_time);
        transaction_in_string.is_using_redeem = BoolToString(transaction.is_using_redeem);
        transaction_in_string.is_pickup = BoolToString(transaction.is_pickup);

        user_transactions_in_string_file.AllTransactionsInString.push_back(transaction_in_string);
    }
    jUserTransactions = user_transactions_in_string_file;

    // Write the json file
    std::ofstream outUserTransactions(filename);
    outUserTransactions << jUserTransactions;
    outUserTransactions.close();
}


vector<Transactions> const AllTransactions::getUserTransactions(const string& user_id)
{
    return all_transactions_include_same_records[user_id];
}

BehaviorProfile& const AllTransactions::getUserBP(const string& user_id)
{
    return all_users_behavior_profiles[user_id];
}

// Define how json object is transformed into TransactionsInString
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TransactionsInString, user_id, time, order_address, shipping_address, store_address, category, amount, payment_method, tip_amount, delay_time, is_using_redeem, is_pickup)
// Define how json object is transformed into AllTransactionsInString
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AllTransactionsInStringFile, AllTransactionsInString)

void AllTransactions::Initialize()
{
    // Read the json file
    // ./Fraud_Detection_Based_on_Total_Order_Relation_and_Behavior_Diversity/All_Known_Transactions.json
    std::ifstream inAllTransactions("Known_Transactions.json");
    // Check if the file is opened
    if (!inAllTransactions)
    {
        cerr << "Cannot open the file: " <<"Known_Transactions.json" << endl;
        // #EXIT
        exit(1);
    }

    json jAllTransactions;
    inAllTransactions >> jAllTransactions;

    // Close the file
    inAllTransactions.close();

    // Deserialize JSON data into a structure
    // #Memory remember to free the memory
    AllTransactionsInStringFile* all_transactions_in_string_file = new AllTransactionsInStringFile();
    *all_transactions_in_string_file = jAllTransactions.get<AllTransactionsInStringFile>();

    // Preprocess the data
    // From string to enum
    PreprocessAllTransactionsInString(all_transactions_in_string_file);

    // Remove duplicate records for each user
    RemoveDuplicateRecords();

    // Calculate the necessary values
    initializeBP();

    // Initialize the unknown transactions
    // Read the json file
    // ./Fraud_Detection_Based_on_Total_Order_Relation_and_Behavior_Diversity/All_Known_Transactions.json

    inAllTransactions.open("Unknown_Transactions.json");
    // Check if the file is opened
    if (!inAllTransactions)
    {
        cerr << "Cannot open the file: " << "Unknown_Transactions.json" << endl;
        // #EXIT
        exit(1);
    }

    jAllTransactions;
    inAllTransactions >> jAllTransactions;

    // Close the file
    inAllTransactions.close();

    // Deserialize JSON data into a structure
    // #Memory remember to free the memory
    AllTransactionsInStringFile* all_transactions_in_string_file = new AllTransactionsInStringFile();
    *all_transactions_in_string_file = jAllTransactions.get<AllTransactionsInStringFile>();
}

void AllTransactions::update()
{
    // Clean everything
    all_users.clear();
    total_users_number = 0;
    all_transactions_include_same_records.clear();
    total_transactions_number = 0;
    all_transactions_without_same_records.clear();
    total_unique_transactions_number = 0;
    all_users_behavior_profiles.clear();
    unknown_transactions.clear();

    // Re-initialize the data
    Initialize();
}

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
        transaction.delay_time = StringToDelayTime(transaction_in_string.delay_time);
        transaction.is_using_redeem = StringToBool(transaction_in_string.is_using_redeem);
        transaction.is_pickup = StringToBool(transaction_in_string.is_pickup);

        // Add the transaction to all_transactions_include_same_records
        auto it = all_transactions_include_same_records.find(transaction.user_id);
        // Check if the user_id is in the map
        if (it != all_transactions_include_same_records.end())
        {
            it->second.push_back(transaction);
        }
        else
        {
            all_transactions_include_same_records.insert({ transaction.user_id, {transaction} });
        }

        // Add the user_id to all_users
        all_users.insert(transaction.user_id);
    }

    // initialize the total number of transactions
    for (auto& user : all_transactions_include_same_records)
    {
        total_transactions_number += user.second.size();
    }
    // initialize the total number of users
    total_users_number = all_users.size();


    // Free memory use
    delete all_transactions_in_string_file;
}

// ------------------------ String to enum ------------------------
// #NEED_CHANGE
// Need to categorize the time
Time AllTransactions::StringToTime(const string& s)
{
    if (s == "morning")
    {
        return Time::morning;
    }
    else if (s == "noon")
    {
        return Time::noon;
    }
    else if (s == "afternoon")
    {
        return Time::afternoon;
    }
    else if (s == "evening")
    {
        return Time::evening;
    }
    else
    {
        cerr << "Invalid time data" << endl;
        // #EXIT
        exit(2);
    }
}

// #NEED_CHANGE
// Need to categorize the location
Location AllTransactions::StringToLocation(const string& s)
{
    if (s == "vancouver")
    {
        return Location::vancouver;
    }
    else if (s == "north_vancouver")
    {
        return Location::north_vancouver;
    }
    else if (s == "burnaby")
    {
        return Location::burnaby;
    }
    else if (s == "richmond")
    {
        return Location::richmond;
    }
    else if (s == "surrey")
    {
        return Location::surrey;
    }
    else if (s == "new_wedtmindter")
    {
        return Location::new_wedtmindter;
    }
    else if (s == "coquitlam")
    {
        return Location::coquitlam;
    }
    else
    {
        cerr << "Invalid location data" << endl;
        // #EXIT
        exit(3);
    }
}

Category AllTransactions::StringToCategory(const string& s)
{
    if (s == "korean")
    {
        return Category::korean;
    }
    else if (s == "japanese")
    {
        return Category::japanese;
    }
    else if (s == "chinese")
    {
        return Category::chinese;
    }
    else if (s == "western")
    {
        return Category::western;
    }
    else if (s == "asian")
    {
        return Category::asian;
    }
    else if (s == "chicken")
    {
        return Category::chicken;
    }
    else if (s == "vagan")
    {
        return Category::vagan;
    }
    else if (s == "dessert")
    {
        return Category::dessert;
    }
    else if (s == "grocery")
    {
        return Category::grocery;
    }
    else
    {
        cerr << "Invalid category data" << endl;
        // #EXIT
        exit(4);
    }
}

// #NEED_CHANGE
// Need to categorize the amount
Amount AllTransactions::StringToAmount(const string& s)
{
    if (s == "a0_10")
    {
        return Amount::a0_10;
    }
    else if (s == "a10_20")
    {
        return Amount::a10_20;
    }
    else if (s == "a20_30")
    {
        return Amount::a20_30;
    }
    else if (s == "a30_40")
    {
        return Amount::a30_40;
    }
    else if (s == "a40_50")
    {
        return Amount::a40_50;
    }
    else if (s == "a50_60")
    {
        return Amount::a50_60;
    }
    else if (s == "a60_70")
    {
        return Amount::a60_70;
    }
    else if (s == "a70_80")
    {
        return Amount::a70_80;
    }
    else if (s == "a80_90")
    {
        return Amount::a80_90;
    }
    else if (s == "a90_100")
    {
        return Amount::a90_100;
    }
    else
    {
        cerr << "Invalid amount data" << endl;
        // #EXIT
        exit(5);
    }
}

PaymentMethod AllTransactions::StringToPaymentMethod(const string& s)
{
    if (s == "credit_card")
    {
        return PaymentMethod::credit_card;
    }
    else if (s == "debit_card")
    {
        return PaymentMethod::debit_card;
    }
    else
    {
        cerr << "Invalid payment method data" << endl;
        // #EXIT
        exit(6);
    }
}

// #NEED_CHANGE
// Need to categorize the tip amount
TipAmount AllTransactions::StringToTipAmount(const string& s)
{
    if (s == "ta0_5")
    {
        return TipAmount::ta0_5;
    }
    else if (s == "ta5_10")
    {
        return TipAmount::ta5_10;
    }
    else if (s == "ta10_15")
    {
        return TipAmount::ta10_15;
    }
    else if (s == "ta15_20")
    {
        return TipAmount::ta15_20;
    }
    else
    {
        cerr << "Invalid tip amount data" << endl;
        // #EXIT
        exit(7);
    }
}

// #NEED_CHANGE
// Need to categorize the delay time
DelayTime AllTransactions::StringToDelayTime(const string& s)
{
    if (s == "dt0_2")
    {
        return DelayTime::dt0_2;
    }
    else if (s == "dt2_4")
    {
        return DelayTime::dt2_4;
    }
    else if (s == "dt4_6")
    {
        return DelayTime::dt4_6;
    }
    else
    {
        cerr << "Invalid delay time data" << endl;
        // #EXIT
        exit(8);
    }
}

bool AllTransactions::StringToBool(const string& s)
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


void AllTransactions::RemoveDuplicateRecords()
{
    for (auto& user : all_transactions_include_same_records)
    {
        std::vector<Transactions> unique_transactions;

        for (auto& transaction : user.second)
        {
            auto it = all_transactions_without_same_records[user.first].find(transaction);
            // When the transaction not found in the set
            if (it == all_transactions_without_same_records[user.first].end())
            {
                all_transactions_without_same_records.insert({ user.first, {transaction} });
                total_unique_transactions_number++;
            }

        }

        user.second = unique_transactions;
        total_unique_transactions_number += user.second.size();
    }
}

// -------------------------- Calculation --------------------------
void AllTransactions::initializeBP()
{
    // Calculate all necessary values
    for (auto& user_id : all_users) 
    {
        initializeLGBP(user_id);
        initializeM_u(user_id);
        initializeT_u(user_id);
        initializeOmega_u(user_id);
    }
}

// #TODO
void AllTransactions::initializeLGBP(const string& user_id)
{

}

void AllTransactions::initializeM_u(const string& user_id)
{

}

void AllTransactions::initializeT_u(const string& user_id)
{

}

void AllTransactions::initializeOmega_u(const string& user_id)
{

}

// ----------------------------------------------------------------

// ------------------------ Enum to string ------------------------
string AllTransactions::TimeToString(const Time& t)
{
    if (t == Time::morning)
    {
        return "morning";
    }
    else if (t == Time::noon)
    {
        return "noon";
    }
    else if (t == Time::afternoon)
    {
        return "afternoon";
    }
    else if (t == Time::evening)
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

string AllTransactions::LocationToString(const Location& l)
{
    if (l == Location::vancouver)
    {
        return "vancouver";
    }
    else if (l == Location::north_vancouver)
    {
        return "north_vancouver";
    }
    else if (l == Location::burnaby)
    {
        return "burnaby";
    }
    else if (l == Location::richmond)
    {
        return "richmond";
    }
    else if (l == Location::surrey)
    {
        return "surrey";
    }
    else if (l == Location::new_wedtmindter)
    {
        return "new_wedtmindter";
    }
    else if (l == Location::coquitlam)
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

string AllTransactions::CategoryToString(const Category& c)
{
    if (c == Category::korean)
    {
        return "korean";
    }
    else if (c == Category::japanese)
    {
        return "japanese";
    }
    else if (c == Category::chinese)
    {
        return "chinese";
    }
    else if (c == Category::western)
    {
        return "western";
    }
    else if (c == Category::asian)
    {
        return "asian";
    }
    else if (c == Category::chicken)
    {
        return "chicken";
    }
    else if (c == Category::vagan)
    {
        return "vagan";
    }
    else if (c == Category::dessert)
    {
        return "dessert";
    }
    else if (c == Category::grocery)
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

string AllTransactions::AmountToString(const Amount& a)
{
    if (a == Amount::a0_10)
    {
        return "a0_10";
    }
    else if (a == Amount::a10_20)
    {
        return "a10_20";
    }
    else if (a == Amount::a20_30)
    {
        return "a20_30";
    }
    else if (a == Amount::a30_40)
    {
        return "a30_40";
    }
    else if (a == Amount::a40_50)
    {
        return "a40_50";
    }
    else if (a == Amount::a50_60)
    {
        return "a50_60";
    }
    else if (a == Amount::a60_70)
    {
        return "a60_70";
    }
    else if (a == Amount::a70_80)
    {
        return "a70_80";
    }
    else if (a == Amount::a80_90)
    {
        return "a80_90";
    }
    else if (a == Amount::a90_100)
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

string AllTransactions::PaymentMethodToString(const PaymentMethod& p)
{
    if (p == PaymentMethod::credit_card)
    {
        return "credit_card";
    }
    else if (p == PaymentMethod::debit_card)
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

string AllTransactions::TipAmountToString(const TipAmount& t)
{
    if (t == TipAmount::ta0_5)
    {
        return "ta0_5";
    }
    else if (t == TipAmount::ta5_10)
    {
        return "ta5_10";
    }
    else if (t == TipAmount::ta10_15)
    {
        return "ta10_15";
    }
    else if (t == TipAmount::ta15_20)
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

string AllTransactions::DelayTimeToString(const DelayTime& d)
{
    if (d == DelayTime::dt0_2)
    {
        return "dt0_2";
    }
    else if (d == DelayTime::dt2_4)
    {
        return "dt2_4";
    }
    else if (d == DelayTime::dt4_6)
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

string AllTransactions::BoolToString(const bool& b)
{
    if (b)
    {
        return "true";
    }
    else
    {
        return "false";
    }
}

