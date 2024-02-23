#include "AllTransactions.h"

bool AllTransactions::isVertexInLGBP(const string& user_id, const string& attribute_name, int level)
{
    // Check if the user_id is in the map
    auto all_users_behavior_profiles_it = all_users_behavior_profiles.find(user_id);
    if (all_users_behavior_profiles_it != all_users_behavior_profiles.end())
    {
        // Check if the attribute_name is in the all_attributes
        auto all_attributes_it = all_users_behavior_profiles_it->second.all_attributes[level].find(attribute_name);
        if (all_attributes_it != all_users_behavior_profiles_it->second.all_attributes[level].end())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        cerr << "The user_id: " << user_id << " does not exist in all_users_behavior_profiles" << endl;
        // #EXIT
        exit(18);
    }
}

vector<vector<DiGraph::vertex_descriptor>> AllTransactions::getPerpaths(const BehaviorProfile* behavior_profile, DiGraph::vertex_descriptor v, int v_attribute_level)
{
    vector<DiGraph::vertex_descriptor> sigle_perpath;

    vector<vector<DiGraph::vertex_descriptor>> perpaths;

    std::pair<boost::graph_traits<DiGraph>::edge_descriptor, bool> is_edge_exist;

    // Using reverse DFS algorithm
    // The path must be following the order of attributes
    // The start_vertex must connect to the all other vertexes in attribute level 1
    // From the given vertex v, reversely find all the vertexes that are reachable to the vertex v
    for(int attribute_level = v_attribute_level - 1; attribute_level > 0; attribute_level--)
    {
        for (auto& vertex_it : behavior_profile->all_attributes.at(attribute_level))
        {
            is_edge_exist = boost::edge(vertex_it.second, v, behavior_profile->LGBP);

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

vector<DiGraph::vertex_descriptor> AllTransactions::getPostnodes(const DiGraph& LGBP, DiGraph::vertex_descriptor v)
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

int AllTransactions::attributeFrequency(const string& user_id, int attribute_level, const string& attribute)
{
    for (auto& transaction_it : all_users_transactions.at(user_id))
    {
        switch (attribute_level)
        {
            case 1:
                if (AttributesToString(transaction_it.time) == attribute) return transaction_it.frequency;
                else return 0;
                break;
            case 2:
                if (AttributesToString(transaction_it.store_address) == attribute) return transaction_it.frequency;
                else return 0;
                break;
            case 3:
                if (AttributesToString(transaction_it.order_address) == attribute) return transaction_it.frequency;
                else return 0;
                break;
            case 4:
                if (AttributesToString(transaction_it.shipping_address) == attribute) return transaction_it.frequency;
                else return 0;
                break;
            case 5:
                if (AttributesToString(transaction_it.is_pickup) == attribute) return transaction_it.frequency;
                else return 0;
                break;
            case 6:
                if (AttributesToString(transaction_it.is_using_redeem) == attribute) return transaction_it.frequency;
                else return 0;
                break;
            case 7:
                if (AttributesToString(transaction_it.amount) == attribute) return transaction_it.frequency;
                else return 0;
                break;
            case 8:
                if (AttributesToString(transaction_it.tip_amount) == attribute) return transaction_it.frequency;
                else return 0;
                break;
            case 9:
                if (AttributesToString(transaction_it.delay_time) == attribute) return transaction_it.frequency;
                else return 0;
                break;
            case 10:
                if (AttributesToString(transaction_it.payment_method) == attribute) return transaction_it.frequency;
                else return 0;
                break;
            default:
                cerr << "The attribute_level: " << attribute_level << " is not supported" << endl;
                // #EXIT
                exit(19);
        }
    }
}

int AllTransactions::continueAttributeFrequency(const string& user_id, int attribute_level, const string& front_attribute, const string& back_attribute)
{
    int frequency = 0;

    auto postnodes = getPostnodes(all_users_behavior_profiles.at(user_id).LGBP,
        all_users_behavior_profiles.at(user_id).all_attributes.at(attribute_level).at(front_attribute));

    for(auto& postnodes_it : postnodes)
    {
        if (AttributesToString(postnodes_it) == back_attribute)
        {
            frequency += std::min(attributeFrequency(user_id, attribute_level, front_attribute),
                attributeFrequency(user_id, attribute_level, back_attribute));
        }
    }

    return frequency;
}


