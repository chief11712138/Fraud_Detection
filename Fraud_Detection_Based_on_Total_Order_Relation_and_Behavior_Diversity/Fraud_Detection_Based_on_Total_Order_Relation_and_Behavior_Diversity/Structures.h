// Includes all self-defined structures and types

#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include "nlohmann/json.hpp"

#include "EnumeratedData.h"

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::vector;
using std::set;
using std::map;
using std::cerr;
using nlohmann::json;
using boost::numeric::ublas::matrix;

// Transaction attributes
// Categories order:
// Time, order_address, shipping_address, store_address, category, is_pickup, is_using_redeem, 
// amount, tip_amount, delay_time, payment_method
struct Transactions
{
    // This UUID is limited in this system
    // Not represent the real UUID of the transaction from the database
    string UUID;

    string user_id;

    Enumerators::Time time;
    Enumerators::Location store_address;
    Enumerators::Location order_address;
    Enumerators::Location shipping_address;
    Enumerators::Category category;
    bool is_pickup;
    bool is_using_redeem;
    Enumerators::Amount amount;
    Enumerators::TipAmount tip_amount;
    Enumerators::DelayTime delay_time;
    Enumerators::PaymentMethod payment_method;
    
    int frequency;

    // Default Constructor
    Transactions() : user_id(""),
        time(Enumerators::Time::morning),
        order_address(Enumerators::Location::vancouver),
        shipping_address(Enumerators::Location::vancouver),
        store_address(Enumerators::Location::vancouver),
        category(Enumerators::Category::korean),
        amount(Enumerators::Amount::a0_10),
        payment_method(Enumerators::PaymentMethod::credit_card),
        tip_amount(Enumerators::TipAmount::ta0_5),
        delay_time(Enumerators::DelayTime::dt0_2),
        is_using_redeem(false),
        is_pickup(false),
        frequency(1) {}

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
        is_pickup(t.is_pickup),
        frequency(t.frequency) {}

    // Overload operator
    bool operator==(const Transactions& other) const {
        return UUID == other.UUID;
    }
    bool operator!=(const Transactions& other) const {
        return UUID != other.UUID;
    }
    bool operator<(const Transactions& other) const {
        return UUID < other.UUID;
    }
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

// Define the directed graph as logic graph of BP (LGBP)
typedef boost::adjacency_list<boost::setS, boost::setS, boost::directedS> DiGraph;

//Behavior Profile
struct BehaviorProfile
{
    // User ID
    string user_id;

    // LGBP
    DiGraph LGBP;

    // Diversity coefficient
    double omega_u = 0;

    // Contain all attributes. For each attributes contain all elements in the attribute
    // It will help us find a certain attribute (name)
    // 
    // <attributes, AttributesandLevel>
    // There will be 13 attributes in total
    // [0] is start node
    // [1]-[11] following attributes order
    // [12] is tail node
    // For example:
    // <attribute's level, <attribute_name, vertex_id>>
    map<int, map<string, DiGraph::vertex_descriptor>> all_attributes;

    // Path-based transition probability matrix
    // M_v for every vertex
    //
    // Perpaths will determine the matrix row size
    // Postnodes will determine the matrix column size
    map<int, map<string, matrix<double>>> M_v;

    // State transition probability matrix
    map<int, matrix<double>> T_u;
};

// Define the transaction map for all users
// Same transactions is allowed for a user
#ifndef Transaction_For_User
#define Transaction_For_User
// (user_id, user_transactions)
typedef map<string, vector<Transactions>> TransactionsForUsers;
#endif

// Define the behavior map for all users
// Each user has exactly one behavior profile
#ifndef Behavior_Profiles_For_All_Users
#define Behavior_Profiles_For_All_Users
typedef map<string, BehaviorProfile> BehaviorProfilesForAllUsers;
#endif

// Define how json object is transformed into TransactionsInString
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TransactionsInString, user_id, time, order_address, shipping_address, store_address, category, amount, payment_method, tip_amount, delay_time, is_using_redeem, is_pickup)

// Define how json object is transformed into AllTransactionsInString
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AllTransactionsInStringFile, AllTransactionsInString)