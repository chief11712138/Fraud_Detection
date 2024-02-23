#include "AllTransactions.h"

map<string, bool> AllTransactions::InitializeUnkonwnTransactions()
{
    // Read the json file
    // ./Fraud_Detection_Based_on_Total_Order_Relation_and_Behavior_Diversity/All_Known_Transactions.json
    std::ifstream inAllTransactions("Unknown_Transactions.json");
    // Check if the file is opened
    if (!inAllTransactions)
    {
        cerr << "Cannot open the file: " << "Unkown_Transactions.json" << endl;
        // #EXIT
        exit(21);
    }

    json jAllTransactions;
    inAllTransactions >> jAllTransactions;


    // Close the file
    inAllTransactions.close();

    // Deserialize JSON data into a structure
    // #Memory remember to free the memory
    AllTransactionsInStringFile* all_unknown_transactions_in_string_file = new AllTransactionsInStringFile();
    *all_unknown_transactions_in_string_file = jAllTransactions.get<AllTransactionsInStringFile>();

    // Preprocess the data
    // From string to enum
    PreprocessAllTransactionsInString(all_unknown_transactions_in_string_file);

    // Initialize the unknown transactions
    // Read the json file
    // ./Fraud_Detection_Based_on_Total_Order_Relation_and_Behavior_Diversity/All_Known_Transactions.json

    inAllTransactions.open("Unknown_Transactions.json");
    // Check if the file is opened
    if (!inAllTransactions)
    {
        cerr << "Cannot open the file: " << "Unknown_Transactions.json" << endl;
        // #EXIT
        exit(21);
    }

    jAllTransactions;
    inAllTransactions >> jAllTransactions;

    // Close the file
    inAllTransactions.close();

    // Deserialize JSON data into a structure
    // #Memory remember to free the memory
    all_unknown_transactions_in_string_file = new AllTransactionsInStringFile();
    *all_unknown_transactions_in_string_file = jAllTransactions.get<AllTransactionsInStringFile>();
}

void AllTransactions::fraudDetection()
{
    map<string, map<Transactions*, double>> recongnization_degree = CalculateRecognizationDegree();

    map<string, map<Transactions*, double>> mean_acceptance_degree;
    for (auto& user_id : all_users_for_unknown_transactions)
    {
        for(auto& transaction : recongnization_degree.at(user_id))
        {
            mean_acceptance_degree[user_id].insert({ transaction.first, CalculateMeanAcceptanceDegree(user_id) });
	
	        double result =
	            (mean_acceptance_degree.at(user_id).at(transaction.first) - recongnization_degree.at(user_id).at(transaction.first)) /
	            mean_acceptance_degree.at(user_id).at(transaction.first);
	
	        if (result >= all_users_threshold.at(user_id))
	        {
	            fraud_detection_result[user_id].insert({ transaction.first, true});
	        }
	        else
	        {
	            fraud_detection_result[user_id].insert({ transaction.first, false });
	        }
        }
    }
}

