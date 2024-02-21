// Boost Graph Library Required. Reference: https://sourceforge.net/projects/boost/files/
// Json for Modern C++ Required. Reference: https://github.com/nlohmann/json

// There are 3 different files contain the implementation of member functions
// 1. AllTransactions.cpp // Include the constructor function
// 2. AllTransactions_Helper_Functions.cpp // Include all helper functions
// 3. AllTransactions_Main_Functions.cpp // Include all main functions which is the API for the user

#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "nlohmann/json.hpp"

#include "EnumeratedData.h"
#include "Structures.h"

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <functional>
#include <algorithm>
#include <cctype>
#include <limits>

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::vector;
using std::set;
using std::map;
using std::cerr;
using nlohmann::json;

class AllTransactions
{
public:
    // #Constructor
    AllTransactions();

    // #DEBUG
    // Print all transactions to json file with the given filename
    void printTransactionsToJsonFile(const string& filename = "All_Transactions.json");
    // #DEBUG
    // Print certain user's transactions to json file with the given filename
    void printUserTransactionsToJsonFile(const string& user_id, const string& filename = "User_transactions.json");


    // #DEBUG
    // Print all BP to json file with the given filename
    void printBPsToJsonFile(const string& filename = "All_BPs.json") const;
    // #DEBUG
    // Print certain user's BP to json file with the given filename
    void printUserBPToJsonFile(const string& user_id, const string& filename = "User's_BP.json") const;


    //--------------------------------------------------------------------------------
    // #Helper Functions
    // Return the certain user's transactions pointer
    // Users responsibility to ensure the user_id is valid
    vector<Transactions>& getUserTransactions(const string& user_id);

    // #Helper Functions
    // Return the certain user's BP
    // Users responsibility to ensure the user_id is valid
    BehaviorProfile& getUserBP(const string& user_id);
    
    //--------------------------------------------------------------------------------


    // #Constructor
    // Update every variables value
    void updateBP();

    // Read unknown transactions from the json file
    void readUnknownTransactionsFromFile(const string& filename = "Unknown_Transactions.json");

    // Determine if the unknown transactions are fraud
    void fraudDetection();

    // #TODO
    void printResultToJsonFile(const string& filename = "Result.json");


    //--------------------------------------------------------------------------------
    // #Helper Functions
private:

    // Help #Constructor
    // Initialize all transactions from the database
    // All precise data will be categorized into enumerated data
    void Initialize();

    void ToJson(json& j, const AllTransactionsInStringFile& at);

    void PreprocessAllTransactionsInString(AllTransactionsInStringFile* all_transactions_in_string_file);

    // String to enum
    Enumerators::Time StringToTime(const string& s) const;
    Enumerators::Location StringToLocation(const string& s) const;
    Enumerators::Category StringToCategory(const string& s) const;
    Enumerators::Amount StringToAmount(const string& s) const;
    Enumerators::PaymentMethod StringToPaymentMethod(const string& s) const;
    Enumerators::TipAmount StringToTipAmount(const string& s) const;
    Enumerators::DelayTime StringToDelayTime(const string& begin_time, const string& end_time) const;
    bool StringToBool(const string& s) const;

    // The UUID will have format:
    // user_id + time + order_address + shipping_address + store_address + category + 
    // amount + payment_method + tip_amount + delay_time + is_using_redeem + is_pickup
    //
    // Suppose we have       
    // "user_id": "ABC123",
    // "time": "08:00 15/03/2024", (morning)
    // "order_address" : "123 Main St, Burnaby, BC V0V0V0", (Burnaby)
    // "shipping_address" : "456 Elm St, Burnaby, BC V0V0V0", (Burnaby)
    // "store_address" : "789 Oak St, Burnaby, BC V0V0V0", (Burnaby)
    // "category" : "korean",
    // "amount" : "20.00",
    // "payment_method" : "credit_card",
    // "tip_amount" : "3.00",
    // "delay_time" : "10:00 16/03/2024", (morning)
    // "is_using_redeem" : true
    // "is_pickup" : false
    // "frequency" : 1 // Base-35: 1-9, A-Z // #NEED_CHANGE Up to 35
    // The UUID will be: ABC123MBBBKO011000101
    string UUIDGenerator(const Transactions& transaction);

    void RemoveDuplicateRecords();

    // #Deprecated
    // Sort user's transactions by time (morning, noon, afternoon, evening)
    void sortTransactionsByTime();

    // #TODO
    map<string, bool> InitializeUnkonwnTransactions();


    // ------------------------ Parameter Calculation ------------------------
    // 
    // Construct the LGBP(direct graph) for each user
    // Each user's LGBP will contain all the transactions
    // The graph vertex will be ordered by levels:
    // Time, order_address, store_address, category, is_pickup, is_using_redeem, amount, tip_amount, delay_time, payment_method
    void InitializeBP();

    // Passing parameters into functions
    void InitializeLGBP(const string& user_id);
    void InitializeOmega_u(const string& user_id);
    void InitializeM_v(const string& user_id);
    void InitializeT_u(const string& user_id);

    // #Helper Functions

    // Return the perpaths of one vertex
    vector<vector<DiGraph::vertex_descriptor>> getPerpaths(
        const BehaviorProfile* behavior_profile, DiGraph::vertex_descriptor v, int v_attribute_level);

    // Return the Postnodes of one vertex
    vector<DiGraph::vertex_descriptor> getPostnodes(
        const DiGraph& LGBP, DiGraph::vertex_descriptor v);

    // ------------------------------------------------------------------------


    string intToAttributeString(int i, int attribute_level) const;

    // Return the frequency of the attribute in user's all transactions
    int attributeFrequency(const string& user_id, int attribute_level, const string& attribute);

    // Return the frequency of continue attribute in user's all transactions
    int continueAttributeFrequency(const string& user_id, int attribute_level,
        const string& front_attribute, const string& back_attribute);

    // #Deprecated
    // Check if a vertex is already in LGBP
    // Judge by the attribute name
    // True if find
    // False if not
    bool isVertexInLGBP(const string& user_id, const string& attribute_name, int level);


    // Enum to string
    string AttributesToString(const Enumerators::Time& t) const;
    string AttributesToString(const Enumerators::Location& l) const;
    string AttributesToString(const Enumerators::Category& c) const;
    string AttributesToString(const Enumerators::Amount& a) const;
    string AttributesToString(const Enumerators::PaymentMethod& p) const;
    string AttributesToString(const Enumerators::TipAmount& t) const;
    string AttributesToString(const Enumerators::DelayTime& d) const;
    string AttributesToString(const bool& b) const;
    string AttributesToString(const int& b) const;
    

    // ---------------------------- Fraud Detection ----------------------------

    // #TODO
    map<string, double> CalculateRecognizationDegree();

    // #TODO
    map<string, double> CaclulateAcceptanceDegree();

    // #TODO
    // Calculate the mean transaction records for the given user
    double CalculateMeanAcceptanceDegree(const string& user_id);

    // #TODO
    // Initialize the threshold for all users in unknown transactions
    void InitializeThreshold();

    // --------------------------------------------------------------------------

// #Variables
private:
    // <Attribute_level, attribute_member_number>
    // For example: For type Time
    // The attribute_level is 1
    // The attribute_member_number is 4 (morning, noon, afternoon, evening)
    static const map<int, int> attributes_number;

    set<string> all_users;
    int total_users_number;

    // <user_id, threshold>
    map<string, double> all_users_threshold;

    // This k used to determine how many latest transactions record 
    // will be considered by the mean acceptance degree
    int k = 10;
    

    //--------------------------------------------------------------------------------
    // Known transactions
    // This set will be initialized directly from the database
    // May contain same records for a transaction
    TransactionsForUsers all_users_transactions;

    int total_transactions_number;

    int total_unique_transactions_number;


    //--------------------------------------------------------------------------------
    // Behavior Profile
    // All BP of every users
    BehaviorProfilesForAllUsers all_users_behavior_profiles;


    //--------------------------------------------------------------------------------
    set<string> all_users_for_unknown_transactions;

    // unknown transactions
    // This set contain all the transactions waiting to be processed
    
    TransactionsForUsers unknown_transactions;
};

