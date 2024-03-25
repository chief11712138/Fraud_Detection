#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "EnumeratedData.h"
#include "Structure.h"

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <fstream>
#include <sstream>
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

// #TODO
// Global variables
static const vector<string> known_data_file = {"2019-01-01 to 2019-12-31 stripe unified_payments"};
static const vector<string> unknown_data_file = {"2019-01-01 to 2019-12-31 stripe unified_payments"};

static const map<int, string> attribute_level_to_string = {
    {0, "start_vertex"},
    {1, "time"},
    {2, "amount"},
    {3, "payment_status"},
    {4, "card_id"},
    {5, "user_id"},
};

class Fraud_Detection_System
{
// Constructor
public:
    Fraud_Detection_System();

//--------------------------------------------------------------------------------

// Fraud_Detection_System
public:
    // Help constructor
    // Initialize all transactions from the database
    // All possible precise data will be categorized into enumerated data
    void Initialize();

    // Update everything in the system
    // Initialize again
    // Recalculate everything
    void update();

//--------------------------------------------------------------------------------

// I/O
public:
    // #TODO
    // Write the result to .csv file
    bool OutputResult(const string& file_name = "Fraud Detection Result.csv");

private:
    // Read (known) .csv file with given name list
    void ReadKnownData();

    // Read (UnkownData) .csv file with given name list
    void ReadUnknownData();

    // #Helpe function
    // selection: 0 for known data, 1 for unknown data
    void PerProcessData(const vector<string>& transaction_in_string, const string& transaction_id, int selection);
    
    // selection: 0 for known data, 1 for unknown data
    string UUIDGenerator(const Transaction& transaction, int selection);

    // String to Enumerated Data
    Enumerators::Time  StringtToTime(const string& time);

    // String to Enumerated Data
    Enumerators::Amount StringToAmount(const string & amount);

//--------------------------------------------------------------------------------

// Detection
public:
    void FraudDetection();

private:
    double CalculateRecognizationDegree(Transaction* unkonw_transaction);

    double CalculateMeanAcceptanceDegree(const string& user_id);

    // #TODO
    double CalculateAcceptanceDegree(Transaction& transaction, double recognization degree);

    // #Help function 

    // #TODO
    DiGraph::vertex_descriptor getMaxTransitionNode(
        const BehaviorProfile& bp, 
        string current_vertex, 
        int attribute_level);

//--------------------------------------------------------------------------------

// Parameter Calculation
private:
    // Construct the LGBP(direct graph) for each user
    // Each user's LGBP will contain all the transactions
    // The graph vertex will be ordered by levels:
    // Time, order_address, store_address, category, is_pickup, is_using_redeem, amount, tip_amount, delay_time, payment_method
    void InitializeBP();

    void InitializeLGBP();

    void InitializeOmega_u();

    void InitializeM_v();

    void InitializeT_u();

    // #Help function

    string AttributesToString(Enumerators::Time time);
    string AttributesToString(Enumerators::Amount amount);
    string AttributesToString(bool payment_status);

    // Return the perpaths of one vertex
    vector<vector<DiGraph::vertex_descriptor>> getPerpaths(
        const BehaviorProfile* behavior_profile, DiGraph::vertex_descriptor v, int v_attribute_level);

    // Return the Postnodes of one vertex 
    vector<DiGraph::vertex_descriptor> getPostnodes(
        const DiGraph& LGBP, DiGraph::vertex_descriptor v);

    int attributeFrequency(const string& user_id, int attribute_level, const string& attribute);

    int continueAttributeFrequency(const string& user_id, int attribute_level, const string& front_attribute, const string& back_attribute);

    string intToAttributeString(int i, int attribute_level) const;

//--------------------------------------------------------------------------------

// Variables
private:
    // All users
    set<string> users;

    // Card_id numbers for each user
    // <user_id, card_id_number>
    map<string, int> card_id_number;

    // <user_id, <transaction_uuid, transaction>>
    map<string, map<string, Transaction>> transactions;

    // BP
    map<string, BehaviorProfile> behavior_profiles;

    // #NEEDCHANGE
    // This k used to determine how many latest transactions record 
    // will be considered by the mean acceptance degree
    int k = 10;

    // #NEEDCHANGE
    // This threshold used to determine whether the transaction is fraud
    double threshold = 0.5;

    //--------------------------------------------------------------------------------

    // Unknown transactions users
    set<string> unknown_transactions_users;

    // Unknown transactions
    // <transaction_id, <transaction_id, transaction>>
    map<string, map<string, Transaction>> unknown_transactions;

    // k Acceptance degree
    // Insert when can't find
    // <user_id, accept degree>
    map<string, double> mean_acceptance_degree;

    // Fraud detection result
    // <transaction_id, transaction>
    map<string, bool> fraud_detection_result;
};

