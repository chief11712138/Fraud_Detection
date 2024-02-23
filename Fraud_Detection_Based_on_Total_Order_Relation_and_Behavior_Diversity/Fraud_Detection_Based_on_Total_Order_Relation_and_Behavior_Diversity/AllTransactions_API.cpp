#include "AllTransactions.h"

void AllTransactions::printTransactionsToJsonFile(const string& filename /*= "All_Transactions.json"*/)
{
    // Serialize the data
    std::ofstream outResult(filename);
    if (!outResult)
    {
        cerr << "Cannot open the file: " << filename << endl;
        // #EXIT
        exit(21);
    }

    json jResult;
    for (auto& user_id : all_users)
    {
        for (auto& transaction : all_users_transactions.at(user_id))
        {
            jResult.push_back({
                {"UUID", transaction.UUID},
                {"user_id", transaction.user_id},
                {"Time", transaction.time},
                {"Store Address", transaction.store_address},
                {"Order Address", transaction.order_address},
                {"Shipping Address", transaction.shipping_address}, 
                {"Category", transaction.category}, 
                {"Is pickup", transaction.is_pickup}, 
                {"Is using redeem", transaction.is_using_redeem}, 
                {"Amount", transaction.amount},
                {"Tip amount", transaction.tip_amount}, 
                {"Delay time", transaction.delay_time}, 
                {"Payment method", transaction.payment_method},  });
        }
    }

    outResult << jResult.dump(4);
    outResult.close();
}

void AllTransactions::printResultToJsonFile(const string& filename /*= "Result.json"*/)
{
    // Print fraud_detection_result to a json file with given name
    // The print format will be "user_id, transaction.UUID, result"
    std::ofstream outResult(filename);
    if (!outResult)
    {
        cerr << "Cannot open the file: " << filename << endl;
        // #EXIT
        exit(21);
    }

    json jResult;
    for (auto& user_id : all_users_for_unknown_transactions)
    {
        for (auto& transaction : fraud_detection_result.at(user_id))
        {
            jResult.push_back({ {"user_id", user_id}, {"UUID", transaction.first->UUID}, {"result", transaction.second} });
        }
    }

    outResult << jResult.dump(4);
    outResult.close();
}

vector<Transactions>& AllTransactions::getUserTransactions(const string& user_id)
{
    return all_users_transactions[user_id];
}

BehaviorProfile& AllTransactions::getUserBP(const string& user_id)
{
    return all_users_behavior_profiles[user_id];
}

void AllTransactions::updateBP()
{
    // Clean everything
    all_users.clear();
    total_users_number = 0;
    all_users_transactions.clear();
    total_transactions_number = 0;
    total_unique_transactions_number = 0;
    all_users_behavior_profiles.clear();
    unknown_transactions.clear();

    // Re-initialize the data
    Initialize();
}