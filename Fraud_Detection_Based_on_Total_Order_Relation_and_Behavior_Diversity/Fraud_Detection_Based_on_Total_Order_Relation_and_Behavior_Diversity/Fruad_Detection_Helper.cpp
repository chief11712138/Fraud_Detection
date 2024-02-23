#include "AllTransactions.h"

#ifndef NAME_PROPERTY
#define NAME_PROPERTY
typedef boost::property_map<DiGraph, boost::vertex_name_t>::type NameProperty;
#endif
map<string, map<Transactions*, double>> AllTransactions::CalculateRecognizationDegree()
{
    map<string, map<Transactions*, double>> result;

    string start_vertex = "start_vertex";
    string sigma = start_vertex;
    int sigma_level = 0;
    double beta = 1;

    for (auto& user_id : all_users_for_unknown_transactions)
    {
        BehaviorProfile* behavior_profile = &all_users_behavior_profiles.at(user_id);
        for (auto& transactions : unknown_transactions.at(user_id))
        {
            for (int attribute_level = 1; attribute_level < 11; attribute_level++)
            {
                matrix<double> behavior_profile_matrix = behavior_profile->M_v.at(attribute_level - 1).at(transactions.attribute_with_level.at(attribute_level));
                if (behavior_profile_matrix(sigma_level, attribute_level) != 0)
                {
                    beta *= behavior_profile_matrix(sigma_level, attribute_level);
                    sigma = transactions.attribute_with_level.at(attribute_level);
                    sigma_level = attribute_level;
                }
                else
                {
                    beta *= behavior_profile->omega_u;

                    vector<DiGraph::vertex_descriptor> postnodes = getPostnodes(behavior_profile->LGBP,
                        behavior_profile->all_attributes.at(attribute_level - 1).at(transactions.attribute_with_level.at(attribute_level)));

                    double max = std::numeric_limits<double>::lowest();
                    for (auto& v_it : postnodes)
                    {
                        NameProperty name = boost::get(boost::vertex_name, behavior_profile->LGBP);
                        int v_level = AttributesStringToLevel(boost::get(name, v_it));
                        if (behavior_profile_matrix(sigma_level, v_level) > max)
                        {
                            max = behavior_profile_matrix(sigma_level, v_level);
                            sigma = boost::get(name, v_it);
                            sigma_level = v_level;
                        }
                    }
                }

                result[user_id].insert({ &transactions, beta });
            }
        }
    }

    return result;
}

double AllTransactions::CalculateMeanAcceptanceDegree(const string& user_id)
{
    double result = 0;
    int count = k;

    map<string, map<Transactions*, double>> acceptance_degree;

    string start_vertex = "start_vertex";
    string sigma = start_vertex;
    int sigma_level = 0;
    double beta = 1;

    BehaviorProfile* behavior_profile = &all_users_behavior_profiles.at(user_id);
    for (auto& transactions : unknown_transactions.at(user_id))
    {
        if (count == k)
        {
            break;
        }
        
        count++;

        for (int attribute_level = 1; attribute_level < 11; attribute_level++)
        {
            matrix<double> behavior_profile_matrix = behavior_profile->M_v.at(attribute_level - 1).at(transactions.attribute_with_level.at(attribute_level));
            if (behavior_profile_matrix(sigma_level, attribute_level) != 0)
            {
                beta *= behavior_profile_matrix(sigma_level, attribute_level);
                sigma = transactions.attribute_with_level.at(attribute_level);
                sigma_level = attribute_level;
            }
            else
            {
                beta *= behavior_profile->omega_u;

                vector<DiGraph::vertex_descriptor> postnodes = getPostnodes(behavior_profile->LGBP,
                    behavior_profile->all_attributes.at(attribute_level - 1).at(transactions.attribute_with_level.at(attribute_level)));

                double max = std::numeric_limits<double>::lowest();
                for (auto& v_it : postnodes)
                {
                    NameProperty name = boost::get(boost::vertex_name, behavior_profile->LGBP);
                    int v_level = AttributesStringToLevel(boost::get(name, v_it));
                    if (behavior_profile_matrix(sigma_level, v_level) > max)
                    {
                        max = behavior_profile_matrix(sigma_level, v_level);
                        sigma = boost::get(name, v_it);
                        sigma_level = v_level;
                    }
                }
            }

            acceptance_degree[user_id].insert({ &transactions, beta });
        }
    }

    for (auto& transaction : acceptance_degree.at(user_id))
    {
        result += transaction.second;
    }
    result /= k;

    return result;
}


void AllTransactions::InitializeThreshold()
{
    for (auto& user_id : all_users_for_unknown_transactions)
    {
        all_users_threshold.insert({ user_id, 1.0 });
    }
}