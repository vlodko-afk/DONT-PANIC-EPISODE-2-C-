#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct Node
{
    int value;
    int sum_{};
    string direction{};
    int clone_died{};
    vector<Node*> children;
};
struct Info_about_path
{
    int sum_;
    vector<int> sum_all_node;

    vector<int> path;
};
class Tree : public Node
{
private:

    vector<int> mas{};
    map<int, vector<int>> elv_exist{};
    int exit_floor{}, exit_pos{}, max_elevator;
    string direction;
    int suma = {}, max_round{};

public:

    Tree(vector<int> m, map<int, vector<int>> elv_ext, int exit_f, int exit_p, int nb_elevators, string dir, int start_sum, int mx_rd)
        : mas(m), elv_exist(elv_ext), exit_floor(exit_f), exit_pos(exit_p),
        max_elevator(nb_elevators), direction(dir), suma(start_sum), max_round(mx_rd)
    {

    }

    Node* create_node(int value)
    {
        Node* node = new Node;
        node->value = value;
        node->direction = this->direction;
        node->sum_ = this->suma;
        return node;
    }
    int find_value(vector<Node*>& child, int value)
    {
        for (int i = 0; i < child.size(); i++)
        {
            if (child[i]->value == value)
            {
                return i;
            }
        }
        child.push_back(create_node(value));
        return child.size() - 1;
    }
    void check_sum(Node* curr_node, int val, int indx, int last_count)
    {
        if (curr_node->value < val)
        {
            if (curr_node->direction == "LEFT")
            {
                curr_node->children[indx]->direction = "RIGHT";
                curr_node->children[indx]->sum_ = curr_node->sum_ + 4 + abs(curr_node->value - val) + last_count * 3;
            }
            else
            {
                curr_node->children[indx]->direction = curr_node->direction;
                curr_node->children[indx]->sum_ = curr_node->sum_ + 1 + abs(curr_node->value - val) + last_count * 3;
            }
        }
        else if (curr_node->value > val)
        {
            if (curr_node->direction == "RIGHT")
            {
                curr_node->children[indx]->direction = "LEFT";
                curr_node->children[indx]->sum_ = curr_node->sum_ + 4 + abs(curr_node->value - val) + last_count * 3;
            }
            else
            {
                curr_node->children[indx]->direction = curr_node->direction;
                curr_node->children[indx]->sum_ = curr_node->sum_ + 1 + abs(curr_node->value - val) + last_count * 3;
            }
        }
        else
        {
            curr_node->children[indx]->direction = curr_node->direction;
            curr_node->children[indx]->sum_ = curr_node->sum_ + 1 + abs(curr_node->value - val) + last_count * 3;
        }
    }
    vector<int>find_neighbors(map<int, vector<int>> elevator_exist, int element, int pos, bool& flag)
    {
        vector <int> arr;
        if (elevator_exist.find(pos) == elevator_exist.end()) return {};
        int min_ = -1, max_ = -1;
        for (auto m : elevator_exist[pos])
        {
            if (m <= element)
            {
                min_ = m;
            }
            if (element < m)
            {
                max_ = m;
                break;
            }
        }

        if (min_ < 0 && max_ > 0)
        {
            if (exit_pos < max_) flag = true;
            return { max_};
        }
        else if (max_ < 0 && min_ > 0)
        {
            if (exit_pos > min_) flag = true;
            return{ min_ };
        }
        else if (min_ > 0 && max_ > 0)
        {
            if (exit_pos > min_ && max_ > exit_pos) flag = true;
            return { min_, max_ };
        }
        return {};
    }

    void create_tree_with_array(Node* curr_node, int pos, int count_elevator)
    {
        pos++;
        if (pos <= exit_floor && count_elevator <= max_elevator && curr_node->sum_ <= max_round)
        {
            bool flag = false, check_range = false;
            vector<int> arr = find_neighbors(elv_exist, curr_node->value, pos, check_range);
            for (auto elem : arr)
            {
                int ch_idx = find_value(curr_node->children, elem);
                if (arr[0] == curr_node->value)
                {
                    flag = true;
                    check_sum(curr_node, elem, ch_idx, 0);
                    create_tree_with_array(curr_node->children[ch_idx], pos, count_elevator);
                    break;
                }
                else
                {
                    check_sum(curr_node, elem, ch_idx, 0);
                    create_tree_with_array(curr_node->children[ch_idx], pos, count_elevator);
                }
            }
            if (flag != true)
            {
                int index = find(mas.begin(), mas.end(), curr_node->value) - mas.begin(), last_count = 0;
               
                if (arr.empty() || check_range == true)
                {
                    int ch_idx = find_value(curr_node->children, exit_pos);
                    check_sum(curr_node, exit_pos, ch_idx, 1);
                    count_elevator++;
                    last_count = 1;
                    create_tree_with_array(curr_node->children[ch_idx], pos, count_elevator);
                }
                count_elevator = (count_elevator - last_count) + 1;
                int ch_idx = find_value(curr_node->children, curr_node->value);
                check_sum(curr_node, curr_node->value, ch_idx, 1);
                create_tree_with_array(curr_node->children[ch_idx], pos, count_elevator);
            }
        }
    }

    void all_paths(Node* curr_node, vector<int>& curr_path, vector<int>& s_r, vector<Info_about_path>& info, int s_)
    {
        if (curr_node == nullptr) return;

        curr_path.push_back(curr_node->value);
        s_r.push_back(curr_node->sum_);
        if (curr_node->children.empty())
        {

            if (curr_path.size() == exit_floor + 1 && curr_path[curr_path.size() - 1] == exit_pos)
            {
                info.push_back(Info_about_path{ curr_node->sum_, s_r,curr_path });
            }
        }
        for (auto child : curr_node->children)
        {
            all_paths(child, curr_path, s_r, info, curr_node->sum_);
        }
        s_r.pop_back();
        curr_path.pop_back();
    }

};


int main()
{
    int nb_floors; // number of floors
    int width; // width of the area
    int nb_rounds; // maximum number of rounds
    int exit_floor; // floor on which the exit is found
    int exit_pos; // position of the exit on its floor
    int nb_total_clones; // number of generated clones
    int nb_additional_elevators; // number of additional elevators that you can build
    int nb_elevators; // number of elevators
    cin >> nb_floors >> width >> nb_rounds >> exit_floor >> exit_pos >> nb_total_clones >> nb_additional_elevators >> nb_elevators; cin.ignore();
   
    cerr  << "number flors: " << nb_floors << endl;
    cerr  << "number elevator for build: " << nb_additional_elevators << endl;
    cerr  << "number elevator: " << nb_elevators << endl;
    cerr  << "total clone:  " << nb_total_clones << endl;
    cerr  << "exit pos: " << exit_pos << endl;
    cerr  << "exit floors: " << exit_floor << endl;
    cerr  << "rounds:  " << nb_rounds << endl;
    cerr  << "width: " << width << endl;
    cerr  << "nb floors: " << nb_floors << endl;

    vector<int> elevator{exit_pos};

    map<int, vector<int>> elevator_exist;

    elevator_exist[exit_floor].push_back(exit_pos);
    for (int i = 0; i < nb_elevators; i++) {
        int elevator_floor; // floor on which this elevator is found
        int elevator_pos; // position of the elevator on its floor
        cin >> elevator_floor >> elevator_pos; cin.ignore();
        elevator_exist[elevator_floor].push_back(elevator_pos);
        sort(elevator_exist[elevator_floor].begin(), elevator_exist[elevator_floor].end());
        if (find(elevator.begin(), elevator.end(), elevator_pos) == elevator.end())
            elevator.push_back(elevator_pos);
    }
    sort(elevator.begin(), elevator.end());

    Tree tree(elevator, elevator_exist, exit_floor, exit_pos, nb_additional_elevators, "RIGHT", 0, nb_rounds);

    Node* root = tree.create_node(6);
    vector<int> curr_path;
    vector<Info_about_path> information;
    vector<int> s_r;

    vector<int> arr;
    bool check_flag = false; 
    

    while (1) {
        int clone_floor; // floor of the leading clone
        int clone_pos; // position of the leading clone on its floor
        string direction, dir; // direction of the leading clone: LEFT or RIGHT
        cin >> clone_floor >> clone_pos >> direction; cin.ignore();
        if (check_flag == false)
        {
            check_flag = true;
            for (auto& s: elevator)
            {
                int difer_dir{};
                if (s < clone_pos)
                {
                    difer_dir = 3;
                    dir = "LEFT";
                }
                Tree tree(elevator, elevator_exist, exit_floor, exit_pos, nb_additional_elevators, dir, difer_dir + abs(s - clone_pos) + 1, nb_rounds);
                Node* root = tree.create_node(s);
                vector<int> curr_path;
                vector<Info_about_path> information;
                vector<int> s_r;

                int count = find(elevator_exist[0].begin(), elevator_exist[0].end(), s) == elevator_exist[0].end()? 1: 0;

                tree.create_tree_with_array(root, 0, count);

                tree.all_paths(root, curr_path, s_r, information, 0);

                if (information.size() > 0 && information[0].sum_ <= nb_rounds)
                {   
                    cerr << information[0].sum_ << endl;
                    arr = information[0].path;
                    break;
                }
            }
        }
        string Flag;
        for( int i = 0; i < arr.size(); i ++ )
        {
            if (clone_floor == i)
            {
                if ((clone_pos < arr[i] && direction == "LEFT") || (arr[i] < clone_pos && direction == "RIGHT"))
                {
                    Flag = "BLOCK";
                }
                else if (clone_pos == arr[i])
                {
                    if (find(elevator_exist[i].begin(), elevator_exist[i].end(), arr[i]) == elevator_exist[i].end())
                    {
                        
                        elevator_exist[i].push_back(clone_pos);
                        Flag = "ELEVATOR";
                    }
                }
                
            }
            cerr << "arr: " << arr[i] << endl; 
        }
        cerr <<"--------------"<< endl;
        Flag.empty()? cout << "WAIT" << endl: cout << Flag << endl;
    }
}
