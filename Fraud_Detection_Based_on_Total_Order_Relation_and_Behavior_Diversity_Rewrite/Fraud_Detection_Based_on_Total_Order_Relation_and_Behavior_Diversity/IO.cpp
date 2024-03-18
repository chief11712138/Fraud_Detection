#include "Fraud_Detection_System.h"

#include <fstream>
#include <sstream>

using std::ifstream;
using std::ofstream;
using std::stringstream;

bool Fraud_Detection_System::OutputResult(const string& file_name /*= "Fraud Detection Result.csv"*/)
{
    ofstream file(file_name);

    // Check if open successfully
    if (!file.is_open()) {
        cout << "Unable to open file: \"" << file_name << "\"" << endl;
        // #Exit code -3
        exit(-3);
    }

    file << "Transaction ID,Result\n";

    for (auto& user_id : unknown_transactions_users)
    {
        for(auto& transaction_id : unknown_transactions[user_id])
        {
             file << transaction_id.first << "," << (fraud_detection_result[transaction_id] ? "Valid" : "inValid") << "\n";
        }
    }

    file.close();
}

void Fraud_Detection_System::ReadKnownData()
{
    // time, amount, payment_status, card_id, user_id;
    vector<string> transaction_in_string;
    string transaction_id;

    // <user_id, card_id>
    map<string, vector<string>> card_id_list;

    // Read file from know data list file_name
    for (auto& file_name : known_data_file)
    {
        // Open file
        ifstream file(file_name);
        string line;

        if (file.is_open())
        {
            while(getline(file, line))
            {
                transaction_in_string.clear();
                transaction_id.clear();

                stringstream ss(line);

                for (int i = 0; i < 21; i++)
                {
                    string temp;
                    if (i == 2 || i == 12 || i == 15 || i == 16)
                    {
                        getline(ss, temp, ',');
                        transaction_in_string.push_back(temp);
                    }

                    if (i == 6)
                    {
                        string temp_1, temp_2;
                        getline(ss, temp_1, ',');
                        getline(ss, temp_2, ',');

                        double temp_1_double = stod(temp_1);
                        double temp_2_double = stod(temp_2);

                        transaction_in_string.push_back(std::to_string(temp_1_double - temp_2_double));
                    }

                    if(i == 20)
                    {
                        getline(ss, temp, ',');
                        transaction_id = temp;
                    }
                }

                // Insert to card_id_list
                if (card_id_list.find(transaction_in_string[3]) != card_id_list.end())
                {
                    card_id_list[transaction_in_string[3]].push_back(transaction_in_string[3]);
                }

                PerProcessData(transaction_in_string, transaction_id, 0);
            }

            file.close();
        }
        else
        {
            cout << "Unable to open file: \"" << file_name <<"\"" << endl;
            // #Exit code -1
            exit(-1);
        }
    }

    for (auto& user_id : card_id_list)
    {
        card_id_number[user_id.first] = user_id.second.size();
    }
}

void Fraud_Detection_System::ReadUnknownData()
{
    // time, amount, payment_status, seller_message, card_id, user_id;
    vector<string> transaction_in_string;
    string transaction_id;

    // Read file from know data list file_name
    for (auto& file_name : unknown_data_file)
    {
        // Open file
        ifstream file(file_name);
        string line;

        if (file.is_open())
        {
            while (getline(file, line))
            {
                transaction_in_string.clear();
                transaction_id.clear();

                stringstream ss(line);

                for (int i = 0; i < 21; i++)
                {
                    transaction_in_string.clear();
                    transaction_id.clear();

                    string temp;
                    if (i == 2 || i == 12 || i == 13 || i == 15 || i == 16)
                    {
                        getline(ss, temp, ',');
                        transaction_in_string.push_back(temp);
                    }

                    if (i == 6)
                    {
                        string temp_1, temp_2;
                        getline(ss, temp_1, ',');
                        getline(ss, temp_2, ',');

                        double temp_1_double = stod(temp_1);
                        double temp_2_double = stod(temp_2);

                        transaction_in_string.push_back(std::to_string(temp_1_double - temp_2_double));
                    }

                    if (i == 20)
                    {
                        getline(ss, temp, ',');
                        transaction_id = temp;
                    }
                }

                PerProcessData(transaction_in_string, transaction_id, 1);
            }
            
            file.close();
        }
        else
        {
            cout << "Unable to open file: \"" << file_name << "\"" << endl;
            // #Exit code -1
            exit(-1);
        }
    }
}


void Fraud_Detection_System::PerProcessData(const vector<string>& transaction_in_string, const string& transaction_id, int selection)
{
    // time, amount, payment_status, card_id, user_id;
    Transaction temp;

    temp.time = StringtToTime(transaction_in_string[0].substr(10, 3));
    temp.amount = StringToAmount(transaction_in_string[1]);
    if (transaction_in_string[2] == "paid")
        temp.payment_status = true;
    else
        temp.payment_status = false;

    temp.card_id = transaction_in_string[3];
    temp.user_id = transaction_in_string[4];

    users.insert(temp.user_id);
    temp.uuid = UUIDGenerator(temp, selection);
    
    if (!selection)
    {
        if (transactions.find(temp.user_id) != transactions.end() && 
            transactions[temp.user_id].find(temp.uuid) != transactions[temp.user_id].end())
        {
            transactions[temp.user_id][temp.uuid].frequency++;
        }
        else if (transactions.find(temp.user_id) != transactions.end() &&
            transactions[temp.user_id].find(temp.uuid) == transactions[temp.user_id].end())
        {
            transactions[temp.user_id].insert({ temp.uuid, temp });
        }
        else
        {
            users.insert(temp.user_id);

            // Add to known transactions
            transactions[temp.user_id].insert({ temp.uuid, temp });
        }
    }
    else
    {
        unknown_transactions_users.insert(temp.user_id);

        // Add to unknown transactions
        unknown_transactions[temp.user_id].insert({ transaction_id, temp });
        
    }
}

//--------------------------------------------------------------------------------------------

// Help function

Enumerators::Time Fraud_Detection_System::StringtToTime(const string& time)
{
    string temp_time = time;

    if (temp_time.size() == 3)
        temp_time = "0" + temp_time;

    if (temp_time >= "04:0" && temp_time < "12:0")
        return Enumerators::Time::morning;
    else if (temp_time >= "12:0" && temp_time < "14:0")
        return Enumerators::Time::noon;
    else if (temp_time >= "14:0" && temp_time < "17:0")
        return Enumerators::Time::afternoon;
    else
        return Enumerators::Time::night;
}

Enumerators::Amount Fraud_Detection_System::StringToAmount(const string& amount)
{
    int temp_amount = stoi(amount);
    
    if (temp_amount >=0 && temp_amount < 20)
        return Enumerators::Amount::a0_20;
    else if (temp_amount >= 20 && temp_amount < 40)
        return Enumerators::Amount::a20_40;
    else if (temp_amount >= 40 && temp_amount < 60)
        return Enumerators::Amount::a40_60;
    else if (temp_amount >= 60 && temp_amount < 80)
        return Enumerators::Amount::a60_80;
    else if (temp_amount >= 80 && temp_amount < 100)
        return Enumerators::Amount::a80_100;
    else
        return Enumerators::Amount::aover_100;
}


string Fraud_Detection_System::UUIDGenerator(const Transaction& transaction, int selection)
{
    string uuid = "";

    uuid += std::to_string(static_cast<int>(transaction.time));
    uuid += std::to_string(static_cast<int>(transaction.amount));
    if(transaction.payment_status)
        uuid += "1";
    else
        uuid += "0";
    uuid += transaction.card_id;
    uuid += transaction.user_id;

    return uuid;
}

