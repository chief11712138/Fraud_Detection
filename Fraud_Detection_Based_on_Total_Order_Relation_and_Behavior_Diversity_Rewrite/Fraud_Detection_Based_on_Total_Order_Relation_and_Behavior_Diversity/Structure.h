// Includes all self-defined structures and types

#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "EnumeratedData.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

using std::count;
using std::endl;
using std::cin;
using std::string;
using std::vector;
using std::map;
using std::set;
using boost::numeric::ublas::matrix;

typedef boost::property<boost::vertex_name_t, std::string> VertexName;

// Define the directed graph as logic graph of BP (LGBP)
typedef boost::adjacency_list<boost::setS, boost::setS, boost::directedS, VertexName, boost::no_property> DiGraph;


struct Transaction {
    // Generated in the program and only be used in this program
    // uuid will be unique. For the same transaction, frequency++
    string uuid;

    Enumerators::Time time;
    Enumerators::Amount amount;
    bool payment_status;
    string card_id;
    string user_id;

    int frequency;

    // Constructor
    Transaction() : 
        time(Enumerators::Time::TimeNum), 
        amount(Enumerators::Amount::AmountNum),
        payment_status(false), 
        card_id(""), 
        user_id(""), 
        frequency(0) {}

    // Constructor
    Transaction(const Transaction& t) :
        time(t.time),
        amount(t.amount),
        payment_status(t.payment_status),
        card_id(t.card_id),
        user_id(t.user_id),
        frequency(t.frequency) {}

    // Overload operators
    bool operator==(const Transaction& t) const {
        return (time == t.time) && (amount == t.amount) && (payment_status == t.payment_status) && (seller_message == t.seller_message) && (card_id == t.card_id) && (user_id == t.user_id);
    }

    bool operator!=(const Transaction& t) const {
        return !(*this == t);
    }

    bool operator<(const Transaction& t) const {
        return (time < t.time) || (amount < t.amount) || (payment_status < t.payment_status) || (seller_message < t.seller_message) || (card_id < t.card_id) || (user_id < t.user_id);
    }
};

// Behavior profile
struct BehaviorProfile
{
    // User ID
    string user_id;

    // LGBP
    DiGraph lgbp;

    // <attribute level, <attribute_name, vertex_id>>
    map<int, map<string, DiGraph::vertex_descriptor>> all_attributes;

    // Diversity coefficient
    double omega_u = 0;

    // Path-based transition probability matrix
    // M_v for every vertex
    //
    // Perpaths will determine the matrix row size
    // Postnodes will determine the matrix column size
    // <attribute level, <attribute_name, vertex_id>>
    map<int, map<string, matrix<double>>> M_v;

    // State transition probability matrix
    map<int, matrix<double>> T_u;
};