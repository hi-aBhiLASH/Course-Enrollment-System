#include <bits/stdc++.h>
using namespace std;

class student
{
public:
    string name;
    int year;
    int no_of_cour_comp;
    vector<string> comp_cour;
    vector<string> enr_cour;
    vector<char> slots;

    student()
    {
        name = "";
        year = 0;
        no_of_cour_comp = 0;
    }
    student(string sname, int syear, int no_cour, vector<string> v)
    {
        name = sname;
        year = syear;
        no_of_cour_comp = no_cour;
        comp_cour.resize(no_cour);
        comp_cour = v;
    }
};
class course
{
public:
    string name;
    int credits;
    int cap;
    int no_of_pre;
    vector<string> pre;
    char slot;
    vector<string> enr_students;
    queue<string> waiting_list;

    course()
    {
        name = "";
        credits = 0;
        cap = 0;
        no_of_pre = 0;
        slot = '0';
    }
    course(string cname, int cr, int c, char sl, int npre, vector<string> p)
    {
        name = cname;
        credits = cr;
        cap = c;
        slot = sl;
        no_of_pre = npre;
        pre.resize(npre);
        pre = p;
    }

    void print()
    {
        for (int i = 0; i < enr_students.size(); ++i)
        {
            cout << enr_students[i] << " ";
        }
        cout << endl;
    }

    bool isavaliable()
    {
        return enr_students.size() < cap;
    }
};

int main()
{
    unordered_map<string, student> ms;
    unordered_map<string, course> mc;
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i)
    {
        string s;
        cin >> s;
        if (s == "add_student")
        {
            string id;
            string name;
            int year;
            int no_cour;
            cin >> id >> name >> year >> no_cour;
            vector<string> v(no_cour);
            for (int i = 0; i < no_cour; ++i)
            {
                cin >> v[i];
            }
            ms[id] = student(name, year, no_cour, v);
        }
        else if (s == "add_course")
        {
            string id;
            string name;
            int credits;
            int cap;
            char slot;
            int no_of_pre;
            cin >> id >> name >> credits >> cap >> slot >> no_of_pre;
            vector<string> pre(no_of_pre);
            for (int i = 0; i < no_of_pre; ++i)
            {
                cin >> pre[i];
            }
            bool pre_exist = true;
            for (int i = 0; i < no_of_pre; ++i)
            {
                if (mc.find(pre[i]) == mc.end())
                {
                    pre_exist = false;
                    break;
                }
            }
            if (pre_exist)
            {
                mc[id] = course(name, credits, cap, slot, no_of_pre, pre);
            }
        }
        else if (s == "enroll")
        {
            string st_id;
            string cour_id;
            cin >> st_id >> cour_id;
            if (ms.find(st_id) != ms.end() && mc.find(cour_id) != mc.end())
            {
                bool done = true;
                for (int i = 0; i < mc[cour_id].pre.size(); ++i)
                {
                    if (find(ms[st_id].comp_cour.begin(), ms[st_id].comp_cour.end(), mc[cour_id].pre[i]) == ms[st_id].comp_cour.end())
                    {
                        done = false;
                        break;
                    }
                }
                if (done)
                {
                    char req_slot = mc[cour_id].slot;
                    if (find(ms[st_id].slots.begin(), ms[st_id].slots.end(), req_slot) == ms[st_id].slots.end())
                    {
                        if (find(ms[st_id].enr_cour.begin(), ms[st_id].enr_cour.end(), cour_id) == ms[st_id].enr_cour.end())
                        {
                            if (mc[cour_id].isavaliable())
                            {
                                mc[cour_id].enr_students.push_back(st_id);
                                ms[st_id].enr_cour.push_back(cour_id);
                                ms[st_id].slots.push_back(req_slot);
                            }
                            else
                            {
                                mc[cour_id].waiting_list.push(st_id);
                            }
                        }
                    }
                }
            }
        }
        else if (s == "print")
        {
            string cour_id;
            cin >> cour_id;
            if (mc.find(cour_id) != mc.end())
            {
                cout << "Enrolled students in " << cour_id << ":\n";
                mc[cour_id].print();
            }
            else
            {
                cout << "Invalid Course " << cour_id << endl;
            }
        }
        else if (s == "drop")
        {
            string st_id;
            string cour_id;
            cin >> st_id >> cour_id;

            auto it = find(mc[cour_id].enr_students.begin(), mc[cour_id].enr_students.end(), st_id);
            mc[cour_id].enr_students.erase(it);
            auto it2 = find(ms[st_id].enr_cour.begin(), ms[st_id].enr_cour.end(), cour_id);
            ms[st_id].enr_cour.erase(it2);
            auto it3 = find(ms[st_id].slots.begin(), ms[st_id].slots.end(), mc[cour_id].slot);
    
            while (!mc[cour_id].waiting_list.empty())
            {
                string id = mc[cour_id].waiting_list.front();
                mc[cour_id].waiting_list.pop();
                bool done = true;
                for (int i = 0; i < mc[cour_id].pre.size(); ++i)
                {
                    if (find(ms[id].comp_cour.begin(), ms[id].comp_cour.end(), mc[cour_id].pre[i]) == ms[id].comp_cour.end())
                    {
                        done = false;
                        break;
                    }
                }
                if (done)
                {
                    char req_slot = mc[cour_id].slot;
                    if (find(ms[id].slots.begin(), ms[id].slots.end(), req_slot) == ms[id].slots.end())
                    {
                        if (find(ms[id].enr_cour.begin(), ms[id].enr_cour.end(), cour_id) == ms[id].enr_cour.end())
                        {
                            mc[cour_id].enr_students.push_back(id);
                            ms[id].enr_cour.push_back(cour_id);
                            ms[id].slots.push_back(req_slot);
                            break;
                        }
                    }
                }
            }
        }
    }
}
