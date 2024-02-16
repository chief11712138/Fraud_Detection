// Boost Graph Library Required. Refference: https://sourceforge.net/projects/boost/files/

#pragma once

#include <boost/graph/adjacency_list.hpp>

#include "enumerated_data.h"
#include "nlohmann/json.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <cstdlib>


// #TODO
// Package Boost Graph Library with the project

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::vector;
using std::set;
using std::map;
using std::cerr;

// Transaction attributes
struct Transactions
{
    string user_id;

    Time time;
    Location order_address;
    Location shipping_address;
    Location store_address;
    Category category;
    Amount amount;
    PaymentMethod payment_method;
    TipAmount tip_amount;
    DelayTime delay_time;

    bool is_using_redeem;
    bool is_pickup;

    // Default Constructor
    Transactions() : user_id(""),
                    time(Time::morning),
                    order_address(Location::vancouver),
                    shipping_address(Location::vancouver),
                    store_address(Location::vancouver),
                    category(Category::korean),
                    amount(Amount::a0_10),
                    payment_method(PaymentMethod::credit_card),
                    tip_amount(TipAmount::ta0_5),
                    delay_time(DelayTime::dt0_2),
                    is_using_redeem(false),
                    is_pickup(false) {}

    // Constructors
    Transactions(const Transactions& t) : user_id(t.user_id),
                                          time(t.time),
                                          order_address(t.order_address),
                                          shipping_address(t.shipping_address),
                                          store_address(t.store_address),
                                          category(t.category),
                                          amount(t.amount),
                                          payment_method(t.payment_method),
                                          tip_amount(t.tip_amount),
                                          delay_time(t.delay_time),
                                          is_using_redeem(t.is_using_redeem),
                                          is_pickup(t.is_pickup) {}
};

// #Helper variable
struct TransactionsInString
{
    string user_id;
    string time;
    string order_address;
    string shipping_address;
    string store_address;
    string category;
    string amount;
    string payment_method;
    string tip_amount;
    string delay_time;
    string is_using_redeem;
    string is_pickup;
};

// #Helper variable
struct AllTransactionsInStringFile
{
    std::vector<TransactionsInString> AllTransactionsInString;
};


// Attributes structure
struct Attributes
{
    set<Time> time;
    set<Location> order_address;
    set<Location> shipping_address;
    set<Location> store_address;
    set<Category> category;
    set<Amount> amount;
    set<PaymentMethod> payment_method;
    set<TipAmount> tip_amount;
    set<DelayTime> delay_time;
    set<bool> is_using_redeem;
    set<bool> is_pickup;
};


// Define the directed graph as logic graph of BP (LGBP)
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> DiGraph;

//Behavior Profile
struct BehaviorProfile
{
    // User ID
    string user_id;

    // LGBP
    DiGraph LGBP;

    // Path-based transition probability matrix
    // M_v for every nodes
    vector<vector<vector<double>>> M_u;

    // State transition probability matrix
    vector<vector<vector<double>>> T_u;

    // Diversity coefficient
    double omega_u;
};


// Define the transaction map for the signal user
#ifndef Transaction_For_Signal_User
#define Transaction_For_Signal_User
// (user_id, user_transactions)
typedef map<string, vector<Transactions>> TransactionsForUsers;
typedef map<string, set<Transactions>> UinqueTransactionsForUsers;
typedef map<string, BehaviorProfile> BehaviorProfilesForAllUsers;
#endif

class AllTransactions
{
public:
    // #Constructor
    AllTransactions();

    // #DEBUG
    // Print all transactions to json file with the given filename
    void const printTransactionsToJsonFile(const string& filename = "All_Transactions.json");
    // #DEBUG
    // Print certain user's transactions to json file with the given filename
    void const printUserTransactionsToJsonFile(const string& user_id, const string& filename = "User_transactions.json");


    // #DEBUG
    // Print all BP to json file with the given filename
    void const printBPsToJsonFile(const string& filename = "All_BPs.json");
    // #DEBUG
    // Print certain user's BP to json file with the given filename
    void const printUserBPToJsonFile(const string& user_id, const string& filename = "User's_BP.json");


    //--------------------------------------------------------------------------------
    // #Helper Functions
    // Return the certain user's transactions
    vector<Transactions> const getUserTransactions(const string& user_id);

    // #Helper Functions
    // Return the certain user's BP
    BehaviorProfile& const getUserBP(const string& user_id);
    
    //--------------------------------------------------------------------------------


    // #Constructor
    // Initialize all transactions from the database
    // All precise data will be categorized into enumerated data
    void Initialize();

    // #Constructor
    // Update every variables value
    void update();


    //--------------------------------------------------------------------------------
    // #Helper Functions
private:
    void PreprocessAllTransactionsInString(AllTransactionsInStringFile* all_transactions_in_string_file);

    // String to enum
    Time StringToTime(const string& s);
    Location StringToLocation(const string& s);
    Category StringToCategory(const string& s);
    Amount StringToAmount(const string& s);
    PaymentMethod StringToPaymentMethod(const string& s);
    TipAmount StringToTipAmount(const string& s);
    DelayTime StringToDelayTime(const string& s);
    bool StringToBool(const string& s);

    void RemoveDuplicateRecords();

    void initializeBP();

    void initializeLGBP(const string& user_id);
    void initializeM_u(const string& user_id);
    void initializeT_u(const string& user_id);
    void initializeOmega_u(const string& user_id);


    // Enum to string
    string TimeToString(const Time& t);
    string LocationToString(const Location& l);
    string CategoryToString(const Category& c);
    string AmountToString(const Amount& a);
    string PaymentMethodToString(const PaymentMethod& p);
    string TipAmountToString(const TipAmount& t);
    string DelayTimeToString(const DelayTime& d);
    string BoolToString(const bool& b);
    
// #Variables
private:
    set<string> all_users;
    int total_users_number;
    

    //--------------------------------------------------------------------------------
    // Known transactions
    // This set will be initialized directly from the database
    // May contain same records for a transaction
    TransactionsForUsers all_transactions_include_same_records;

    int total_transactions_number;


    //--------------------------------------------------------------------------------
    // Known transactions
    // Pure from the above set after copy
    // Will not contain same records for a transaction
    UinqueTransactionsForUsers all_transactions_without_same_records;

    int total_unique_transactions_number;


    //--------------------------------------------------------------------------------
    // Behavior Profile
    // All BP of every users
    BehaviorProfilesForAllUsers all_users_behavior_profiles;


    //--------------------------------------------------------------------------------
    // unknown transactions
    // This set contain all the transactions waiting to be processed
    TransactionsForUsers unknown_transactions;
};

