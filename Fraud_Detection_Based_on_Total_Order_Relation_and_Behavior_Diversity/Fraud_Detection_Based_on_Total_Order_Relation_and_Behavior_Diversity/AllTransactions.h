// Boost Graph Library Required. Refference: https://sourceforge.net/projects/boost/files/

#pragma once

#include <boost/graph/adjacency_list.hpp>

#include "enumerated_data.h"

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>


// #TODO
// Package Boost Graph Library with the project

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::vector;

// Transaction attributes
struct Transaction
{
    string user_id;

    Time time;
    Location order_address;
    Location shipping_address;
    Location store_address;
    std::set<Category> category;
    Amount amount;
    PaymentMethod payment_method;
    TipAmount tip_amount;
    DelayTime delay_time;

    bool is_using_redeem;
    bool is_pickup;

    // Default Constructor
    Transaction() : user_id(""),
                    time(Time::morning),
                    order_address(Location::vancouver),
                    shipping_address(Location::vancouver),
                    store_address(Location::vancouver),
                    category({Category::korean}),
                    amount(Amount::a0_10),
                    payment_method(PaymentMethod::credit_card),
                    tip_amount(TipAmount::ta0_5),
                    delay_time(DelayTime::dt0_2),
                    is_using_redeem(false),
                    is_pickup(false) {}

    // Constructors
    Transaction(const Transaction& t) : user_id(t.user_id),
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


// Attributes structure
struct Attributes
{
    std::set<Time> time;
    std::set<Location> order_address;
    std::set<Location> shipping_address;
    std::set<Location> store_address;
    std::set<Category> category;
    std::set<Amount> amount;
    std::set<PaymentMethod> payment_method;
    std::set<TipAmount> tip_amount;
    std::set<DelayTime> delay_time;
    std::set<bool> is_using_redeem;
    std::set<bool> is_pickup;
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
typedef std::map<string, vector<Transaction>> TransactionForSignalUser;
#endif

class AllTransactions
{
public:
    // #Constructor : see #TODO
    // AllTransactions() { initializeAllTransactions(); }

    // #DEBUG
    // Print all transactions to json file with the given filename
    void const printTransactionsToJsonFile(const string& filename = "All_Transactions");
    // #DEBUG
    // Print certain user's transactions to json file with the given filename
    void const printUserTransactionsToJsonFile(const string& user_id, const string& filename = "User_transactions");

    // #DEBUG
    // Print all BP to json file with the given filename
    void const printBPsToJsonFile(const string& filename = "All_Transactions");
    // #DEBUG
    // Print certain user's BP to json file with the given filename
    void const printUserBPToJsonFile(const string& user_id, const string& filename = "User_BP");


    //--------------------------------------------------------------------------------
    // #Helper Functions
    // Return the certain user's transactions
    TransactionForSignalUser& const getUserTransactions(const string& user_id);

    // #Helper Functions
    // Return the certain user's BP
    BehaviorProfile& const getUserBP(const string& user_id);
    
    //--------------------------------------------------------------------------------


    // #Constructor : see #TODO
    // Initialize all transactions from the database
    // All precise data will be categorized into enumerated data
    // void InitializeAllTransactions();


    //--------------------------------------------------------------------------------
    // #Helper Functions
private:
    void initializeLGBP();
    void initializeM_u();
    void initializeT_u();
    void initializeOmega_u();
    void initializeBP();

// #Variables
private:
    std::set<string> all_users;
    int total_users_number;

    //--------------------------------------------------------------------------------
    // Known transactions
    // This set will be initialized directly from the database
    // May contain same records for a transaction
    std::set<TransactionForSignalUser> all_transactions_include_same_records;
    std::map<string, std::set<TransactionForSignalUser>*> pointer_to_all_transactions_include_same_records;

    int total_transactions_number;


    //--------------------------------------------------------------------------------
    // Known transactions
    // Pure from the above set after copy
    // Will not contain same records for a transaction
    std::set<TransactionForSignalUser> all_transactions_without_same_records;
    std::map<string, std::set<TransactionForSignalUser>*> pointer_to_all_transactions_without_same_records;

    int total_unique_transactions_number;


    //--------------------------------------------------------------------------------
    // Behavior Profile
    // All BP of every users
    std::set<BehaviorProfile> all_users_behavior_profiles;
    std::map<string, BehaviorProfile*> pointer_to_all_users_behavior_profiles;


    //--------------------------------------------------------------------------------
    // unknown transactions
    // This set contain all the transactions waiting to be processed
    std::set<TransactionForSignalUser> unknown_transactions;
};

