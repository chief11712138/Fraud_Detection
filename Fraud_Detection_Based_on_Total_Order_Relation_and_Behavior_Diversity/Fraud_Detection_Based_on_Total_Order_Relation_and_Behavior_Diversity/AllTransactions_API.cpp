#include "AllTransactions.h"

// #UNTEST
void AllTransactions::printTransactionsToJsonFile(const string& filename /*= "All_Transactions.json"*/)
{
    // Serialize the data
    json jAllTransactions;
    AllTransactionsInStringFile all_transactions_in_string_file;
    for (auto& user : all_users_transactions)
    {
        for (auto& transaction : user.second)
        {
            TransactionsInString transaction_in_string;
            transaction_in_string.user_id = transaction.user_id;
            transaction_in_string.time = AttributesToString(transaction.time);
            transaction_in_string.order_address = AttributesToString(transaction.order_address);
            transaction_in_string.shipping_address = AttributesToString(transaction.shipping_address);
            transaction_in_string.store_address = AttributesToString(transaction.store_address);
            transaction_in_string.category = AttributesToString(transaction.category);
            transaction_in_string.amount = AttributesToString(transaction.amount);
            transaction_in_string.payment_method = AttributesToString(transaction.payment_method);
            transaction_in_string.tip_amount = AttributesToString(transaction.tip_amount);
            transaction_in_string.delay_time = AttributesToString(transaction.delay_time);
            transaction_in_string.is_using_redeem = AttributesToString(transaction.is_using_redeem);
            transaction_in_string.is_pickup = AttributesToString(transaction.is_pickup);

            all_transactions_in_string_file.AllTransactionsInString.push_back(transaction_in_string);
        }
    }

    ToJson(jAllTransactions, all_transactions_in_string_file);

    // Write the json file
    std::ofstream outAllTransactions(filename);
    outAllTransactions << jAllTransactions;
    outAllTransactions.close();
}

// #UNTEST
void AllTransactions::printUserTransactionsToJsonFile(const string& user_id, const string& filename /*= "User_transactions.json"*/)
{
    // Serialize the data
    json jUserTransactions;
    AllTransactionsInStringFile user_transactions_in_string_file;
    for (auto& transaction : all_users_transactions[user_id])
    {
        TransactionsInString transaction_in_string;
        transaction_in_string.user_id = transaction.user_id;
        transaction_in_string.time = AttributesToString(transaction.time);
        transaction_in_string.order_address = AttributesToString(transaction.order_address);
        transaction_in_string.shipping_address = AttributesToString(transaction.shipping_address);
        transaction_in_string.store_address = AttributesToString(transaction.store_address);
        transaction_in_string.category = AttributesToString(transaction.category);
        transaction_in_string.amount = AttributesToString(transaction.amount);
        transaction_in_string.payment_method = AttributesToString(transaction.payment_method);
        transaction_in_string.tip_amount = AttributesToString(transaction.tip_amount);
        transaction_in_string.delay_time = AttributesToString(transaction.delay_time);
        transaction_in_string.is_using_redeem = AttributesToString(transaction.is_using_redeem);
        transaction_in_string.is_pickup = AttributesToString(transaction.is_pickup);

        user_transactions_in_string_file.AllTransactionsInString.push_back(transaction_in_string);
    }

    ToJson(jUserTransactions, user_transactions_in_string_file);

    // Write the json file
    std::ofstream outUserTransactions(filename);
    outUserTransactions << jUserTransactions;
    outUserTransactions.close();
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