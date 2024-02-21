#include "AllTransactions.h"

map<string, double> AllTransactions::CalculateRecognizationDegree()
{
    map<string, double> result;
    double beta = 1;

    for (auto& user_id : all_users_for_unknown_transactions)
    {
        // If a users is new, then we can't calculate the recognization degree
        // In this case, we will set the recognization degree to the smallest double value
        if(all_users.find(user_id) != all_users.end())
        {
            result.insert({ user_id, std::numeric_limits<double>::lowest() });
        }
        else
        {
            string start_vertex = "start_vertex";
            string state_list = "start_vertex";
        }

        BehaviorProfile* behavior_profile = &all_users_behavior_profiles.at(user_id);

        for (auto& unknown_transactions_it : unknown_transactions.at(user_id))
        {
            // For Time, the M_0 is the start_vertex matrix which is consider to be 0 matrix
            // Therefore, we directly do it and start the loop from the second level
            beta *= behavior_profile->omega_u;

            // #TODO
            // Select node v_max...

            for (int attribute_level = 2; attribute_level < 11; attribute_level++)
            {
               // #TODO
            }
        }
    }
}

map<string, double> AllTransactions::CaclulateAcceptanceDegree()
{

}

double AllTransactions::CalculateMeanAcceptanceDegree(const string& user_id)
{

}

// #NEED_CHANGE
void AllTransactions::InitializeThreshold()
{
    for(auto& user_id : all_users_for_unknown_transactions)
    {
        all_users_threshold.insert({ user_id, 1.0 });
    }
}



