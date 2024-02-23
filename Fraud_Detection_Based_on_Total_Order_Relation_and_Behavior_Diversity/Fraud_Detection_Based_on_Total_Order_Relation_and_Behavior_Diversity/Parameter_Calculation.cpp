#include "AllTransactions.h"

void AllTransactions::InitializeBP()
{
    // Calculate all necessary values
    for (auto& user_id : all_users)
    {
        InitializeLGBP(user_id);
        InitializeM_v(user_id);
        InitializeT_u(user_id);
        InitializeOmega_u(user_id);
    }
}

// Construct the LGBP(direct graph) for each user
// Each user's LGBP will contain all the transactions
// The graph vertex will be ordered by levels:
// Time, order_address, store_address, category, is_pickup, is_using_redeem, amount, tip_amount, delay_time, payment_method
// The graph contains two special vertex: started(start_node_user_id) and tail(tail_node_user_id)
//
// The start vertex will have name: "start_vertex"
// The tail vertex will have name: "tail_vertex"
void AllTransactions::InitializeLGBP(const string& user_id)
{
    // Check if the user_id is in the map
    if (all_users_behavior_profiles.find(user_id) == all_users_behavior_profiles.end())
    {
        // If the user_id is not in the map, then initialize the user_id
        BehaviorProfile behavior_profile;

        DiGraph::vertex_descriptor v_1;
        DiGraph::vertex_descriptor v_2;

        // Insert start vertex
        v_1 = add_vertex({"start_vertex"}, behavior_profile.LGBP);
        behavior_profile.all_attributes[0].insert({ "start_vertex", v_1 });

        // Insert tail vertex
        v_1 = add_vertex({ "tail_vertex" }, behavior_profile.LGBP);
        behavior_profile.all_attributes[12].insert({ "tail_vertex", v_1 });


        // Insert transactions attributes into graph
        for (auto& it : all_users_transactions[user_id])
        {
            // Insert time vertex into graph
            v_1 = add_vertex(AttributesToString(it.time), behavior_profile.LGBP);
            // Insert time attributes into all_attributes
            behavior_profile.all_attributes[1].insert({ AttributesToString(it.time), v_1 });

            // Add an edge between time and start_vertex
            add_edge(behavior_profile.all_attributes[0]["start_vertex"], v_1, behavior_profile.LGBP);

            // Insert store_address vertex into graph
            v_2 = add_vertex(AttributesToString(it.order_address), behavior_profile.LGBP);
            // Insert store_address attributes into all_attributes
            behavior_profile.all_attributes[2].insert({ AttributesToString(it.order_address), v_2 });

            // Add an edge between time and store_address
            add_edge(v_1, v_2, behavior_profile.LGBP);

            // Insert store_address vertex into graph
            v_1 = add_vertex(AttributesToString(it.store_address), behavior_profile.LGBP);
            // Insert store_address attributes into all_attributes
            behavior_profile.all_attributes[3].insert({ AttributesToString(it.store_address), v_1 });

            // Add an edge between store_address and category
            add_edge(v_2, v_1, behavior_profile.LGBP);

            // Insert category vertex into graph
            v_2 = add_vertex(AttributesToString(it.category), behavior_profile.LGBP);
            // Insert category attributes into all_attributes
            behavior_profile.all_attributes[4].insert({ AttributesToString(it.category), v_2 });

            // Add an edge between store_address and category
            add_edge(v_1, v_2, behavior_profile.LGBP);

            // Insert is_pickup vertex into graph
            v_1 = add_vertex(AttributesToString(it.is_pickup), behavior_profile.LGBP);
            // Insert is_pickup attributes into all_attributes
            behavior_profile.all_attributes[5].insert({ AttributesToString(it.is_pickup), v_1 });

            // Add an edge between category and is_pickup
            add_edge(v_2, v_1, behavior_profile.LGBP);

            // Insert is_using_redeem vertex into graph
            v_2 = add_vertex(AttributesToString(it.is_using_redeem), behavior_profile.LGBP);
            // Insert is_using_redeem attributes into all_attributes
            behavior_profile.all_attributes[6].insert({ AttributesToString(it.is_using_redeem), v_2 });

            // Add an edge between is_pickup and is_using_redeem
            add_edge(v_1, v_2, behavior_profile.LGBP);

            // Insert amount vertex into graph
            v_1 = add_vertex(AttributesToString(it.amount), behavior_profile.LGBP);
            // Insert amount attributes into all_attributes
            behavior_profile.all_attributes[7].insert({ AttributesToString(it.amount), v_1 });

            // Add an edge between is_using_redeem and amount
            add_edge(v_2, v_1, behavior_profile.LGBP);

            // Insert tip_amount vertex into graph
            v_2 = add_vertex(AttributesToString(it.tip_amount), behavior_profile.LGBP);
            // Insert tip_amount attributes into all_attributes
            behavior_profile.all_attributes[8].insert({ AttributesToString(it.tip_amount), v_2 });

            // Add an edge between amount and tip_amount
            add_edge(v_1, v_2, behavior_profile.LGBP);

            // Insert delay_time vertex into graph
            v_1 = add_vertex(AttributesToString(it.delay_time), behavior_profile.LGBP);
            // Insert delay_time attributes into all_attributes
            behavior_profile.all_attributes[9].insert({ AttributesToString(it.delay_time), v_1 });

            // Add an edge between tip_amount and delay_time
            add_edge(v_2, v_1, behavior_profile.LGBP);

            // Insert payment_method vertex into graph
            v_2 = add_vertex(AttributesToString(it.payment_method), behavior_profile.LGBP);
            // Insert payment_method attributes into all_attributes
            behavior_profile.all_attributes[10].insert({ AttributesToString(it.payment_method), v_2 });

            // Add an edge between delay_time and payment_method
            add_edge(v_1, v_2, behavior_profile.LGBP);

            // Insert frequency vertex into graph
            v_1 = add_vertex(AttributesToString(it.frequency), behavior_profile.LGBP);
            // Insert frequency attributes into all_attributes
            behavior_profile.all_attributes[11].insert({ AttributesToString(it.frequency), v_1 });

            // Add an edge between payment_method and frequency
            add_edge(v_2, v_1, behavior_profile.LGBP);

            // Add an edge between payment_method and tail_vertex
            add_edge(v_1, behavior_profile.all_attributes[11]["tail_vertex"], behavior_profile.LGBP);

        }

        // Insert behavior_profile into all_users_behavior_profiles
        all_users_behavior_profiles.insert({ user_id, behavior_profile });
    }
    else
    {
        cerr << "The user_id: " << user_id << " already exists in all_users_behavior_profiles" << endl;
        // #EXIT
        exit(17);
    }
}

void AllTransactions::InitializeOmega_u(const string& user_id)
{
    int total_transaction_number = all_users_transactions[user_id].size();

    // The most frequent transactions of this user
    int kappa = -1;
    for (auto& it : all_users_transactions[user_id])
    {
        if (kappa < it.frequency)
        {
            kappa = it.frequency;
        }
    }

    // Following the formula in article to calculate the omega_u
    for (auto& transaction : all_users_transactions[user_id])
    {
        double P_r = static_cast<double>(transaction.frequency) / static_cast<double>(total_transaction_number);
        all_users_behavior_profiles[user_id].omega_u -= P_r * (log(P_r) / log(kappa));
    }
}

void AllTransactions::InitializeM_v(const string& user_id)
{
    BehaviorProfile* behavior_profile = &all_users_behavior_profiles[user_id];

    vector<vector<DiGraph::vertex_descriptor>>perpaths;
    vector<DiGraph::vertex_descriptor> postnodes;

    matrix<double> m_v;
    

    // Recursively calculate the M_v for each vertexes of user LGBP
    // Doing by attributes levels
    for(int attribute_level = 1; attribute_level <= 11; attribute_level++)
    {
        for (auto& attributes_it : behavior_profile->all_attributes.at(attribute_level))
        {
            auto start_vertex = behavior_profile->all_attributes.at(attribute_level).find("start_vertex");

            // Get perpaths of the vertex
            perpaths = getPerpaths(behavior_profile,attributes_it.second, attribute_level);
            
            // "start_vertex" is necessary exist
            
            postnodes = getPostnodes(behavior_profile->LGBP,attributes_it.second);

            //behavior_profile->m_v must be empty before we calculate it
            // Resize the matrix to the size of perpaths.size() * postnodes.size()
            m_v.resize(perpaths.size(), postnodes.size());

            // Calculate m_v
            // m_v.size1() is the number of rows
            // m_v.size2() is the number of columns
            // v -> v_target (vertex to each_postnodes)
            // sigma is the frequency of the transaction only contain until the vertex
            // sigma_v_target is the frequency of the transaction contain the postnodes
            // m_rc = sigma_v_target / sigma * (1 - omega_u)
            for (int column = 0; column < m_v.size2(); column++)
            {
                double sigma = perpaths.size();

                for (int row = 0; row < m_v.size1(); row++)
                {
                    double sigma_v_target = getPerpaths(behavior_profile,postnodes[row], attribute_level).size();

                    // Insert the value into m_v
                    if(sigma == 0)
                        m_v(row, column) = 0;

                    else
                        m_v(row, column) = sigma_v_target / sigma * (1 - behavior_profile->omega_u);
                }
            }

            // Insert the matrix into behavior_profile->m_v
            behavior_profile->M_v.insert({ attribute_level, {{attributes_it.first, m_v} } });
	    }
    }

}

void AllTransactions::InitializeT_u(const string& user_id)
{
    BehaviorProfile* behavior_profile = &all_users_behavior_profiles[user_id];

    matrix<double> T_u;

    // T_u_row_column = f(A_i -> A_j) / f(A_i)
    // Attribute i's frequency in user's all transactions
    double f_Ai = 0;
    // Attribute i and next transaction attribute j's frequency in user's all transactions
    double f_Ai_Aj = 0;
    
    // Time determine the transaction order
    // Therefore, time will not be a attribute to describe the state transition probability matrix
    for(int attribute_level = 2; attribute_level <= 11; attribute_level++)
    {
        const int size = attributes_number.at(attribute_level);
        T_u.resize(size, size);

        for (int row = 0; row < size; row++)
        {
            for(int column = 0; column < size; column++)
            {
                if (row == column)
                {
                    T_u(row, column) = 0;
                    continue;
                }

                // if an attribute is not in the graph, then all column values of T_u is 0
                for (int attributes_it = 0; attributes_it < attributes_number.at(attribute_level); attributes_it++)
                {
                    auto search_result_it = behavior_profile->all_attributes.at(attribute_level).find(intToAttributeString(attributes_it, attribute_level));
                    if (search_result_it == behavior_profile->all_attributes.at(attribute_level).end())
                    {
                        for(int i = 0; i < size; i++)
                        {
                            T_u(row, i) = 0;
                        }

                        row++;
                        break;
                    }
                }
                
                // Calculate the T_u_row_column
                // T_u_row_column = f(A_i -> A_j) / f(A_i)
                // Calculate the f(A_i)
                for (auto& transaction_it : all_users_transactions.at(user_id))
                {
                    f_Ai += attributeFrequency(user_id, attribute_level, 
                        intToAttributeString(column, attribute_level));
                }
                
                // Calculate the f(A_i -> A_j)
                for (auto transaction_it = all_users_transactions.at(user_id).begin();
                    transaction_it != all_users_transactions.at(user_id).end();
                    transaction_it++)
                {
                    f_Ai_Aj += continueAttributeFrequency(user_id, attribute_level,
                        intToAttributeString(row, attribute_level), intToAttributeString(column, attribute_level));
                }

                // Insert the value into T_u_row_column
                if (f_Ai == 0)
                {
                    T_u(row, column) = 0;
                }
                else
                {
                    T_u(row, column) = f_Ai_Aj / f_Ai;
                }

                f_Ai = 0;
                f_Ai_Aj = 0;
            }
        }
    }
}
