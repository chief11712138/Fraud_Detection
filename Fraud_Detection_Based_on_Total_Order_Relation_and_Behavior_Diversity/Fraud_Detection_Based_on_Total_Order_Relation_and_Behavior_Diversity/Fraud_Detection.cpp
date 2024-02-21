#include "AllTransactions.h"

map<string, bool> AllTransactions::InitializeUnkonwnTransactions()
{
    
}

void AllTransactions::fraudDetection()
{
    // True is is a fraud, false is not a fraud
    map<string, bool> is_fraud_detection_result;

    map<string, double> recongnization_degree = CalculateRecognizationDegree();

    map<string, double> acceptance_degree = CaclulateAcceptanceDegree();

    map<string, double> mean_acceptance_degree;
    for (auto& user_id : all_users_for_unknown_transactions)
    {
        mean_acceptance_degree.insert({ user_id, CalculateMeanAcceptanceDegree(user_id) });

        double result =
            (mean_acceptance_degree.at(user_id) - acceptance_degree.at(user_id)) /
            mean_acceptance_degree.at(user_id);

        if (result >= all_users_threshold.at(user_id))
        {
            is_fraud_detection_result.insert({ user_id, true });
        }
        else
        {
            is_fraud_detection_result.insert({ user_id, false });
        }
    }
}

