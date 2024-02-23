#include "AllTransactions.h"

void AllTransactions::Initialize()
{
    // Read the json file
    // ./Fraud_Detection_Based_on_Total_Order_Relation_and_Behavior_Diversity/All_Known_Transactions.json
    std::ifstream inAllTransactions("Known_Transactions.json");
    // Check if the file is opened
    if (!inAllTransactions)
    {
        cerr << "Cannot open the file: " << "Known_Transactions.json" << endl;
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

    // Calculate the necessary values
    InitializeBP();

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
    all_transactions_in_string_file = new AllTransactionsInStringFile();
    *all_transactions_in_string_file = jAllTransactions.get<AllTransactionsInStringFile>();
}
