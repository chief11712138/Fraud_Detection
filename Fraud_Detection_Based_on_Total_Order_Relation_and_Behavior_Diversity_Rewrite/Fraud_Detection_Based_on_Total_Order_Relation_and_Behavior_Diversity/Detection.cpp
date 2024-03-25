#include "Fraud_Detection_System.h"


void Fraud_Detection_System::FraudDetection()
{
    for (auto& unknown_transactions_users_it : unknown_transactions_users)
    {
        for (auto& unknown_transaction_it : unknown_transactions[unknown_transactions_users_it])
        {
            Transaction* unknown_transaction = &unknown_transaction_it.second;

            double recognization_degree = CalculateRecognizationDegree(unknown_transaction);

            double acceptance_degree = CalculateAcceptanceDegree(*unknown_transaction, recognization_degree);

            double mean_acceptance_degree = CalculateMeanAcceptanceDegree(unknown_transactions_users_it);

            if ((mean_acceptance_degree - acceptance_degree) / mean_acceptance_degree >= threshold)
            {
                fraud_detection_result[unknown_transaction_it.first] = false;
            }
            else
            {
                fraud_detection_result[unknown_transaction_it.first] = true;
            }
        }
    }
}

double Fraud_Detection_System::CalculateRecognizationDegree(Transaction* unknow_transaction)
{
    double recognization_degree = 0;

    if(transactions.find(unknow_transaction->user_id) == transactions.end())
    {
        return -std::numeric_limits<double>::max();
    }
    else
    {
        auto bp = behavior_profiles.find(unknow_transaction->user_id);

        string vertex = "start_vertex";
        int attribute_level_vertex = 0;

        string sigma = vertex;
        int attribute_level_sigma = attribute_level_vertex;

        for (int i = 1; i < 5; i++)
        {
            double M_ai_1 = bp[attribute_level_sigma]->find(sigma)->second;
            if (M_ai_1 != 0)
            {
                recognization_degree *= M_ai_1;
                sigma = attribute_level_to_string[i];
                attribute_level_sigma = i;
            }
            else
            {
                recognization_degree *= bp->second.omega_u;

                boost::property_map<DiGraph, boost::vertex_name_t>::type name_map = get(boost::vertex_name, bp);

                DiGraph::vertex_descriptor temp_vertex = getMaxTransitionNode(bp->second, sigma, i);
                
                sigma = name_map[temp_vertex];
            }
        }
    }
}


double Fraud_Detection_System::CalculateMeanAcceptanceDegree(const string& user_id)
{
    if(mean_acceptance_degree.find(user_id) == mean_acceptance_degree.end())
    {
        for(auto& transaction_it : transactions[user_id])
        {
            Transaction* transaction = &transaction_it.second;

            double recognization_degree = CalculateRecognizationDegree(transaction);

            double acceptance_degree = CalculateAcceptanceDegree(*transaction, recognization_degree);

            mean_acceptance_degree[user_id] += acceptance_degree;
        }

        mean_acceptance_degree[user_id] /= transactions[user_id].size();


        return mean_acceptance_degree[user_id];
    }

    return mean_acceptance_degree[user_id];
}


double Fraud_Detection_System::CalculateAcceptanceDegree(Transaction& transaction, double recognization degree)
{

}



DiGraph::vertex_descriptor Fraud_Detection_System::getMaxTransitionNode(const BehaviorProfile& bp, string current_vertex, int attribute_level)
{
    double max_probability = -1.0;
    DiGraph::vertex_descriptor max_vertex = current_vertex;

    auto postnodes = getPostnodes(bp.lgbp, current_vertex);

    for (auto& next_vertex : postnodes) {

        auto it = bp.M_v[attribute_level].find(current_vertex);
        if (it != bp.M_v[attribute_level].end()) {
            auto prob_it = it->second.find(next_vertex);
            if (prob_it != it->second.end()) {
                double transition_probability = prob_it->second;
                if (transition_probability > max_probability) {
                    max_probability = transition_probability;
                    max_vertex = next_vertex;
                }
            }
        }
    }

    return max_vertex;
}
