#include "Fraud_Detection_System.h"


void Fraud_Detection_System::InitializeBP()
{
    // Calculate all necessary values
    InitializeLGBP();
    InitializeM_v();
    InitializeT_u();
    InitializeOmega_u();
}

// Construct the LGBP(direct graph) for each user
// Each user's LGBP will contain all the transactions
// The graph vertex will be ordered by levels:
// time, amount, card_id, payment_status,  
// The graph contains two special vertex: started(start_node_user_id) and tail(tail_node_user_id)
//
// The start vertex will have name: "start_vertex"
// The tail vertex will have name: "tail_vertex"
void Fraud_Detection_System::InitializeLGBP()
{
    for(auto& user_id : users)
    {
		// Check if the user_id is in the map
	    if (behavior_profiles.find(user_id) == behavior_profiles.end())
	    {
	        // If the user_id is not in the map, then initialize the user_id
	        BehaviorProfile temp_behavior_profile;
	
	        DiGraph::vertex_descriptor v_1;
	        DiGraph::vertex_descriptor v_2;
	
	        // Insert start vertex
	        v_1 = boost::add_vertex({ "start_vertex" }, temp_behavior_profile.lgbp);
            temp_behavior_profile.all_attributes[0].insert({ "start_vertex", v_1 });
	
	        // Insert tail vertex
	        v_1 = boost::add_vertex({ "tail_vertex" }, temp_behavior_profile.lgbp);
            temp_behavior_profile.all_attributes[12].insert({ "tail_vertex", v_1 });
	
	
	        // Insert transactions attributes into graph
	        for (auto& it : transactions[user_id])
	        {
	            // Insert time vertex into graph
	            v_1 = add_vertex(AttributesToString(it.second.time), temp_behavior_profile.lgbp);
	            // Insert time attributes into all_attributes
                temp_behavior_profile.all_attributes[1].insert({ AttributesToString(it.second.time), v_1 });
	
	            // Add an edge between time and start_vertex
	            add_edge(temp_behavior_profile.all_attributes[0]["start_vertex"], v_1, temp_behavior_profile.lgbp);
				
	            // Insert amount vertex into graph
	            v_2 = add_vertex(AttributesToString(it.second.amount), temp_behavior_profile.lgbp);
	            // Insert amount attributes into all_attributes
                temp_behavior_profile.all_attributes[7].insert({ AttributesToString(it.second.amount), v_2 });
	
	            // Add an edge between is_using_redeem and amount
	            add_edge(v_1, v_2, temp_behavior_profile.lgbp);

                // Insert card_id vertex into graph
                v_1 = add_vertex(it.second.card_id, temp_behavior_profile.lgbp);
                // Insert time attributes into all_attributes
                temp_behavior_profile.all_attributes[1].insert({ it.second.card_id, v_1 });

                // Add an edge between amount and card_id
                add_edge(temp_behavior_profile.all_attributes[0]["start_vertex"], v_1, temp_behavior_profile.lgbp);

                // Insert payment_status vertex into graph
                v_2 = add_vertex(AttributesToString(it.second.payment_status), temp_behavior_profile.lgbp);
                // Insert amount attributes into all_attributes
                temp_behavior_profile.all_attributes[7].insert({ AttributesToString(it.second.payment_status), v_2 });

                // Add an edge between card_id and payment_status
                add_edge(v_1, v_2, temp_behavior_profile.lgbp);

	        }
	
	        // Insert temp_behavior_profile into behavior_profiles
            behavior_profiles.insert({ user_id, temp_behavior_profile });

	    }
	    else
	    {
	        cerr << "The user_id: " << user_id << " already exists in behavior_profiles" << endl;
	        // #EXIT code 3
	        exit(3);
	    }
    }
}

void Fraud_Detection_System::InitializeOmega_u()
{
    for(auto& user_id : users)
    {
	int total_transaction_number = transactions[user_id].size();
	
	    // The most frequent transactions of this user
	    int kappa = -1;
	    for (auto& it : transactions[user_id])
	    {
	        if (kappa < it.second.frequency)
	        {
	            kappa = it.second.frequency;
	        }
	    }
	
	    // Following the formula in article to calculate the omega_u
	    for (auto& transaction : transactions[user_id])
	    {
	        double P_r = static_cast<double>(transaction.second.frequency) / static_cast<double>(total_transaction_number);
	        behavior_profiles[user_id].omega_u -= P_r * (log(P_r) / log(kappa));
	    }
    }
}

void Fraud_Detection_System::InitializeM_v()
{
    for (auto& user_id : users)
    {
        BehaviorProfile* ptr_behavior_profile = &behavior_profiles[user_id];

        vector<vector<DiGraph::vertex_descriptor>>perpaths;
        vector<DiGraph::vertex_descriptor> postnodes;

        matrix<double> m_v;


        // Recursively calculate the M_v for each vertexes of user LGBP
        // Doing by attributes levels
        for (int attribute_level = 1; attribute_level <= 11; attribute_level++)
        {
            for (auto& attributes_it : ptr_behavior_profile->all_attributes.at(attribute_level))
            {
                auto start_vertex = ptr_behavior_profile->all_attributes.at(attribute_level).find("start_vertex");

                // Get perpaths of the vertex
                perpaths = getPerpaths(ptr_behavior_profile, attributes_it.second, attribute_level);

                // "start_vertex" is necessary exist

                postnodes = getPostnodes(ptr_behavior_profile->lgbp, attributes_it.second);

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
                        double sigma_v_target = getPerpaths(ptr_behavior_profile, postnodes[row], attribute_level).size();

                        // Insert the value into m_v
                        if (sigma == 0)
                            m_v(row, column) = 0;

                        else
                            m_v(row, column) = sigma_v_target / sigma * (1 - ptr_behavior_profile->omega_u);
                    }
                }

                // Insert the matrix into behavior_profile->m_v
                ptr_behavior_profile->M_v.insert({ attribute_level, {{attributes_it.first, m_v} } });
            }
        }
    }
}

void Fraud_Detection_System::InitializeT_u()
{
    // Initialize attributes_numebr
    const map<int, int> attributes_number = {
    {1, static_cast<int>(Enumerators::Time::TimeNum)},
    {2, static_cast<int>(Enumerators::Amount::AmountNum)},
    {4, 2},
    };

    for(auto& user_id : users)
    {
	    BehaviorProfile* ptr_behavior_profile = &behavior_profiles[user_id];
	
	    matrix<double> T_u;
	
	    // T_u_row_column = f(A_i -> A_j) / f(A_i)
	    // Attribute i's frequency in user's all transactions
	    double f_Ai = 0;
	    // Attribute i and next transaction attribute j's frequency in user's all transactions
	    double f_Ai_Aj = 0;
	
	    // Time determine the transaction order
	    // Therefore, time will not be a attribute to describe the state transition probability matrix
	    for (int attribute_level = 2; attribute_level <= 4; attribute_level++)
	    {
            int size = 0;
            if(attribute_level != 3)
            {
                size = attributes_number.at(attribute_level);
            }
            else
            {
                size = card_id_number[user_id];
            }
	        
	        T_u.resize(size, size);
	
	        for (int row = 0; row < size; row++)
	        {
	            for (int column = 0; column < size; column++)
	            {
	                if (row == column)
	                {
	                    T_u(row, column) = 0;
	                    continue;
	                }
	
	                // if an attribute is not in the graph, then all column values of T_u is 0
	                for (int attributes_it = 0; attributes_it < attributes_number.at(attribute_level); attributes_it++)
	                {
	                    auto search_result_it = ptr_behavior_profile->all_attributes.at(attribute_level).find(intToAttributeString(attributes_it, attribute_level));
	                    if (search_result_it == ptr_behavior_profile->all_attributes.at(attribute_level).end())
	                    {
	                        for (int i = 0; i < size; i++)
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
	                for (auto& transaction_it : transactions.at(user_id))
	                {
	                    f_Ai += attributeFrequency(user_id, attribute_level,
	                        intToAttributeString(column, attribute_level));
	                }
	
	                // Calculate the f(A_i -> A_j)
	                for (auto transaction_it = transactions.at(user_id).begin();
	                    transaction_it != transactions.at(user_id).end();
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
}

//--------------------------------------------------------------------------------

// Help function

string Fraud_Detection_System::AttributesToString(Enumerators::Time time)
{
    if(time == Enumerators::Time::morning)
    {
        return "morning";
    }
    else if (time == Enumerators::Time::noon)
    {
        return "noon";
    }
    else if(time == Enumerators::Time::afternoon)
    {
        return "afternoon";
    }
    else if(time == Enumerators::Time::night)
    {
        return "night";
    }
    else
    {
        cerr << "Enumerator time is not valid" << endl;
        // #EXIT code 1
        exit(1);
    }
}

string Fraud_Detection_System::AttributesToString(Enumerators::Amount amount)
{
    if (amount == Enumerators::Amount::a0_20)
    {
        return "a0_20";
    }
    else if (amount == Enumerators::Amount::a20_40)
    {
        return "a20_40";
    }
    else if (amount == Enumerators::Amount::a40_60)
    {
        return "a40_60";
    }
    else if (amount == Enumerators::Amount::a60_80)
    {
        return "a60_80";
    }
    else if (amount == Enumerators::Amount::a80_100)
    {
        return "a80_100";
    }
    else if (amount == Enumerators::Amount::aover_100)
    {
        return "aover_100";
    }
    else
    {
        cerr << "Enumerator amount is not valid" << endl;
        // #EXIT code 2
        exit(2);
    }
}

string Fraud_Detection_System::AttributesToString(bool payment_status)
{
    return payment_status ? "true" : "false";
}

vector<vector<DiGraph::vertex_descriptor>> Fraud_Detection_System::getPerpaths(const BehaviorProfile* behavior_profile, DiGraph::vertex_descriptor v, int v_attribute_level)
{
    vector<DiGraph::vertex_descriptor> sigle_perpath;

    vector<vector<DiGraph::vertex_descriptor>> perpaths;

    std::pair<boost::graph_traits<DiGraph>::edge_descriptor, bool> is_edge_exist;

    // Using reverse DFS algorithm
    // The path must be following the order of attributes
    // The start_vertex must connect to the all other vertexes in attribute level 1
    // From the given vertex v, reversely find all the vertexes that are reachable to the vertex v
    for (int attribute_level = v_attribute_level - 1; attribute_level > 0; attribute_level--)
    {
        for (auto& vertex_it : behavior_profile->all_attributes.at(attribute_level))
        {
            is_edge_exist = boost::edge(vertex_it.second, v, behavior_profile->lgbp);

            if (is_edge_exist.second)
            {
                sigle_perpath.push_back(vertex_it.second);

                vector<vector<DiGraph::vertex_descriptor>> temp_postnodes = getPerpaths(behavior_profile, vertex_it.second, attribute_level);
                for (int temp_postnodes_it = 0; temp_postnodes_it < temp_postnodes.size(); temp_postnodes_it++)
                {
                    vector<DiGraph::vertex_descriptor> temp_perpaths;

                    temp_perpaths.push_back(vertex_it.second);
                    temp_perpaths.insert(temp_perpaths.begin(), temp_postnodes[temp_postnodes_it].begin(), temp_postnodes[temp_postnodes_it].end());
                }
            }
        }
    }
    return perpaths;
}

vector<DiGraph::vertex_descriptor> Fraud_Detection_System::getPostnodes(const DiGraph& LGBP, DiGraph::vertex_descriptor v)
{
    vector<DiGraph::vertex_descriptor> postnodes;

    // Find all the vertexes that are reachable from the vertex v
    std::pair<DiGraph::adjacency_iterator, DiGraph::adjacency_iterator> neighbors
        = boost::adjacent_vertices(v, LGBP);

    for (auto it = neighbors.first; it != neighbors.second; it++)
    {
        postnodes.push_back(*it);
    }

    return postnodes;
}

int Fraud_Detection_System::attributeFrequency(const string& user_id, int attribute_level, const string& attribute)
{
    for (auto& it : transactions.at(user_id))
    {
        switch (attribute_level)
        {
        case 1:
            if (AttributesToString(it.second.time) == attribute) return it.second.frequency;
            else return 0;
            break;
        case 2:
            if (AttributesToString(it.second.amount) == attribute) return it.second.frequency;
            else return 0;
            break;
        case 4:
            if (it.second.payment_status && attribute == "true" || !it.second.payment_status && attribute != "true") return it.second.frequency;
            else return 0;
            break;
        default:
            cerr << "The attribute_level: " << attribute_level << " is not supported" << endl;
            // #EXIT
            exit(4);
        }
    }
}

int Fraud_Detection_System::continueAttributeFrequency(const string& user_id, int attribute_level, const string& front_attribute, const string& back_attribute)
{
    int frequency = 0;

    auto postnodes = getPostnodes(behavior_profiles.at(user_id).lgbp ,
        behavior_profiles.at(user_id).all_attributes.at(attribute_level).at(front_attribute));

    for (auto& postnodes_it : postnodes)
    {
        if (AttributesToString(postnodes_it) == back_attribute)
        {
            frequency += std::min(attributeFrequency(user_id, attribute_level, front_attribute),
                attributeFrequency(user_id, attribute_level, back_attribute));
        }
    }

    return frequency;
}

string Fraud_Detection_System::intToAttributeString(int i, int attribute_level) const
{
    switch (attribute_level)
    {
    case 1:
        return AttributesToString(static_cast<Enumerators::Time>(i));
    case 2:
        return AttributesToString(static_cast<Enumerators::Amount>(i));
    case 3:
        return std::to_string(i);
    case 4:
        return AttributesToString(static_cast<bool>(i));
    default:
        cerr << "Invalid attribute level" << endl;
        // #EXIT
        exit(20);
    }
}
