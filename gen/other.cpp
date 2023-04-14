#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <bits/stdc++.h>
#include <regex>
using namespace std;

string const WORKING_HOUR = "assets/working_hours.csv";
string const EMPLOYEES = "assets/employees.csv";
string const TEAMS = "assets/teams.csv";
string const SALARY_CONFIGS = "assets/salary_configs.csv";
string const PROCESS_DONE = "OK";
string const DOLLAR_SIGN = "$";
string const COMMA = ",";
string const SPACE = " ";

const string LEVEL1 = "junior";
const string LEVEL2 = "senior";
const string LEVEL3 = "expert";
const string LEVEL4 = "team_lead";

const int MONTH_SIZE = 30;

const string ADD_WORKING_HOURS = "add_working_hours";
const string DELETE_WORKING_HOURS = "delete_working_hours";
const string UPDATE_TEAM_BONUS = "update_team_bonus";
const string REPORT_TOTAL_HOURS_PER_DAY = "report_total_hours_per_day";
const string REPORT_EMLOYEE_PER_HOUR = "report_employee_per_hour";
const string REPORT_SALARIES = "report_salaries";
const string REPORT_EMPLOYEE_SALARY = "report_employee_salary";
const string INVALID_ARGUMENTS_ERROR = "INVALID_ARGUMENTS";
const string INVALID_TIME_INTERVAL_ERROR = "INVALID_INTERVAL";
const string INVALID_EMPLOYEE_ID_ERROR = "EMPLOYEE_NOT_FOUND";
const string INVALID_LEVEL_ERROR = "INVALID_LEVEL";
const string INVALID_TEAM_ID_ERROR = "TEAM_NOT_FOUND";
const string REPORT_TEAM_SALARY = "report_team_salary";
const string SHOW_SALARY_CONFIG = "show_salary_config";
const string UPDATE_SALARY_PARAMETERS = "update_salary_parameters";
const string FIND_TEAMS_FOR_BONUS = "find_teams_for_bonus";
const string NO_BONUS_ERROR = "NO-BONUS";

const std::string WHITESPACE = " \n\r\t\f\v";
std::string trim(const std::string &s);
std::string rtrim(const std::string &s);
std::string ltrim(const std::string &s);

typedef struct time_interval_info
{
    int start_time_interval;
    int end_time_interval;
} time_interval_info;

typedef struct working_time_info
{
    int day;
    vector<string> time_interval;
    vector<time_interval_info> se_info;
} working_time_info;

class teams;
class salary_configs;
class employees;
class workingHour;
class command;
class workingHour
{
public:
    int employee_ID;
    void set_date(string e, string t, string n);
    void check_data(working_time_info w, time_interval_info t);
    int caculate_total_hours_for_each_day(int day);
    int each_employee_total_working_hour();
    void add_working_time(int day_no, int start_time, int end_time);
    void delete_working_time(int d);
    void check_time_interval_overlap(int day, int start_time, int end_time);
    void check_day_exists(int day);
    vector<working_time_info> each_employee;

private:
    string start_time_interval_s, end_time_interval_s;
    int start_time_interval, end_time_interval;
    void check_time_interval(time_interval_info t);
    void check_number_of_day(working_time_info w);
};

class employees
{
public:
    int employee_ID;
    string name;
    int age;
    string level;
    void set_employee_data(string i, string n, string a, string l);
    int get_employee_iD() { return employee_ID; }

private:
};
class salary_configs
{
public:
    string level;
    int base_salary;
    int salary_per_hour;
    int salary_per_extra_hour;
    int official_working_hours;
    int tax_percentage;
    void set_salary_config(string l, string b, string s_p_h, string s_p_e_h, string o, string t);
    int calculate_tax_percentage(int salary);
    int calculate_extra_hour(int total_working_hour);
    int calculate_total_salary(int id, workingHour working_hour_data, vector<teams> team_data);
    int calculate_earning(int id, workingHour working_hour_data, vector<teams> team_data);

private:
    vector<teams> team_data;
    vector<employees> employee_data;
};

class teams
{
public:
    int team_id;
    int team_head_id;
    vector<int> member_ids;
    int bonus_min_working_hour;
    double bonus_working_hour_max_variance;
    int bonus_percentage;
    int all_team_members_total_working_hour(vector<workingHour> working_hour_data);
    void set_teams_info(string t, string t_h_i, string m, string b_m_w_h, string b_w_h_v);
    double get_bonus_working_hour_max_variance() { return bonus_working_hour_max_variance; }
    double all_team_members_total_working_hour_variance(vector<workingHour> working_hour_data, int all_team_members_working_hours, int num_team_members);

private:
};

class command
{

public:
    vector<workingHour> working_hour_data;
    vector<salary_configs> salary_configs_data;
    vector<teams> teams_data;
    vector<employees> employee_data;
    void set_database(vector<employees> e, vector<workingHour> w, vector<teams> t, vector<salary_configs> s);
    vector<employees> sort_employee_data_by_ids(vector<employees>);
    vector<workingHour> sort_working_hour_data_by_ids(vector<workingHour>);
    string text;
    void report_salary();
    void show_salary_config(string level);
    void report_employee_salary(int employee_id);
    salary_configs update_salary_parameters(string level, string b, string s_p_h, string s_p_e_h, string o, string t);
    void report_team_salary(int team_id);
    void update_team_bonus(int team_id, int bonus_percentage);
    void add_working_time_for_specific_employee(int id, int day_number, int start_time, int end_time);
    void delete_working_time_for_specific_employee(int id, int day_number);
    void report_total_hours_per_day(int start_day, int end_day);
    void report_employee_per_hour(int start_hour, int end_hour);
    void print_report_employee_per_hour(vector<vector<double>> report_vec);
    void print_report_total_hours(vector<vector<double>> report_vec);
    bool validate_level(int level_id);
    vector<int> find_busiest(vector<vector<double>> report_vec);
    vector<int> find_emptiest(vector<vector<double>> report_vec);
    int find_the_level_configs(string level);
    void find_teams_for_bonus();
    void print_employee_workingHour_info();
    void print_teams_data();
    void print_salary_configs_data();
    void print_employee_personal_info();

private:
    int calculate_total_team_hours(int team_id);
    void print_members_data(teams team_target);
    vector<teams> sort_teams_by_ids(vector<teams> t);
    bool check_day_validation(int day_num);
    bool check_time_validation(int start_time, int end_time);
    bool check_id_validation(int id);
    bool check_bonus_percentage_validation(int bonus_percentage);
    bool check_st_ed_day_validation(int start_day, int end_day);
    bool check_team_id_validation(int team_id);
    void check_all_working_hour_info();
    teams find_team_by_data(int data_id);
    workingHour find_working_hour(int employee_id);
    employees find_employee_by_id(int employee_id);
};

int find_persons_id(vector<workingHour> w, int persons_id);
void error(string message);
bool compare_by_id_working_hour_version(const workingHour w1, const workingHour w2);
bool compare_by_id_employee_ver(const employees e1, const employees e2);
salary_configs read_salary_configs(string str);
vector<workingHour> delete_merge_repeated_employeeID(vector<workingHour> works);
employees read_employees(string str);
vector<salary_configs> read_salary_configs_info(string filename);
workingHour read_working_hour(string str);
teams read_teams(string str);
vector<teams> read_teams_info(string filename);
vector<workingHour> delete_merge_repeated_employeeID(vector<workingHour> works);
employees read_employees(string str);
vector<workingHour> read_working_hour_info(string filename);
vector<employees> read_employees_info(string filename);
void check_all_working_hour_info(vector<workingHour> w);
vector<string> split_by_delimiter(string str, string delimiter);
string erase_carriege_control(string str);
int find_team_id_employee(int id, vector<teams> team_data);
int find_bonus(int id, vector<teams> team_data);
int main()
{

    string line;
    command new_command;
    vector<workingHour> working_hour_data;
    vector<employees> employees_data;
    vector<salary_configs> salary_configs_data;
    vector<teams> teams_data;
    working_hour_data = read_working_hour_info(WORKING_HOUR);
    salary_configs_data = read_salary_configs_info(SALARY_CONFIGS);
    employees_data = read_employees_info(EMPLOYEES);
    teams_data = read_teams_info(TEAMS);
    new_command.set_database(employees_data, working_hour_data, teams_data, salary_configs_data);

    while (getline(cin, new_command.text))
    {
        vector<string> fields = split_by_delimiter(new_command.text, SPACE);
        string command = fields[0];
        if (command == ADD_WORKING_HOURS)
        {
            new_command.add_working_time_for_specific_employee(stoi(fields[1]), stoi(fields[2]), stoi(fields[3]), stoi(fields[4]));
        }
        if (command == DELETE_WORKING_HOURS)
        {
            new_command.delete_working_time_for_specific_employee(stoi(fields[1]), stoi(fields[2]));
        }
        if (command == UPDATE_TEAM_BONUS)
        {
            new_command.update_team_bonus(stoi(fields[1]), stoi(fields[2]));
        }
        if (command == REPORT_TOTAL_HOURS_PER_DAY)
        {
            new_command.report_total_hours_per_day(stoi(fields[1]), stoi(fields[2]));
        }
        if (command == REPORT_EMLOYEE_PER_HOUR)
        {
            new_command.report_employee_per_hour(stoi(fields[1]), stoi(fields[2]));
        }
        if (command == REPORT_SALARIES)
        {
            new_command.report_salary();
        }
        if (command == REPORT_EMPLOYEE_SALARY)
        {
            new_command.report_employee_salary(stoi(fields[1]));
        }
        if (command == REPORT_TEAM_SALARY)
        {
            new_command.report_team_salary(stoi(fields[1]));
        }
        if (command == SHOW_SALARY_CONFIG)
        {
            new_command.show_salary_config(fields[1]);
        }
        if (command == UPDATE_SALARY_PARAMETERS)
        {
            new_command.update_salary_parameters(fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]);
        }
        if (command == FIND_TEAMS_FOR_BONUS)
        {
            new_command.find_teams_for_bonus();
        }
        if (command == "work")
        {
            new_command.print_employee_workingHour_info();
        }
        if (command == "employee")
        {
            new_command.print_employee_personal_info();
        }
        if (command == "team")
        {
            new_command.print_teams_data();
        }
        if (command == "salary")
        {
            new_command.print_salary_configs_data();
        }
    }
    return 0;
}

string erase_carriege_control(string str)
{
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] == '\r')
        {
            str.erase(str.begin() + i);
        }
    }
    return str;
}

std::string trim(const std::string &s)
{
    return rtrim(ltrim(s));
}

int command ::find_the_level_configs(string level)
{
    trim(level);

    level = erase_carriege_control(level);

    if (level == salary_configs_data[0].level)
    {
        return 0;
    }
    if (level == salary_configs_data[1].level)
    {
        return 1;
    }
    if (level == salary_configs_data[2].level)
    {

        return 2;
    }
    if (level == salary_configs_data[3].level)
    {
        return 3;
    }
    else
    {
        cout << INVALID_LEVEL_ERROR << '\n';
        return MAX_INPUT;
    }
}

void command::report_employee_salary(int employee_id)
{
    employee_data = sort_employee_data_by_ids(employee_data);
    working_hour_data = sort_working_hour_data_by_ids(working_hour_data);
    if (check_id_validation(employee_id) == false)
        return;
    employees target_employee = find_employee_by_id(employee_id);
    int id_level = find_the_level_configs(target_employee.level);
    cout << "ID:" << target_employee.employee_ID << '\n';
    cout << "Name:" << trim(target_employee.name) << '\n';
    cout << "Age:" << target_employee.age << '\n';
    cout << "Level:" << trim(target_employee.level) << '\n';

    if (find_team_id_employee(target_employee.employee_ID, teams_data) < MAX_INPUT)
        cout << "Team:" << find_team_id_employee(target_employee.employee_ID, teams_data) << '\n';
    else
        cout << "Team:N/A\n";

    workingHour target_working_hour = find_working_hour(employee_id);
    cout << "Total Working Hours:" << target_working_hour.each_employee_total_working_hour() << '\n';
    cout << "Absent Days:" << MONTH_SIZE - target_working_hour.each_employee.size() << '\n';
    cout << "Salary: " << salary_configs_data[id_level].calculate_total_salary(employee_id, target_working_hour, teams_data) << '\n';
    cout << "Bonus:" << find_bonus(employee_id, teams_data) << '\n';
    cout << "Tax:" << salary_configs_data[id_level].calculate_tax_percentage(salary_configs_data[id_level].calculate_total_salary(employee_id, target_working_hour, teams_data)) << '\n';
    cout << "Total Earning:" << salary_configs_data[id_level].calculate_earning(employee_id, target_working_hour, teams_data) << '\n';
}

void command::set_database(vector<employees> e, vector<workingHour> w, vector<teams> t, vector<salary_configs> s)
{
    working_hour_data = w;
    salary_configs_data = s;
    teams_data = t;
    employee_data = e;
}

void command ::report_salary()
{
    employee_data = sort_employee_data_by_ids(employee_data);
    working_hour_data = sort_working_hour_data_by_ids(working_hour_data);
    for (int i = 0; i < employee_data.size(); i++)
    {
        int id_level = find_the_level_configs(employee_data[i].level);
        cout << "ID:" << employee_data[i].employee_ID << '\n';
        cout << "Name:" << employee_data[i].name << '\n';
        cout << "Total Working Hours:" << working_hour_data[i].each_employee_total_working_hour() << '\n';
        cout << "Total earning:" << salary_configs_data[id_level].calculate_earning(employee_data[i].employee_ID, working_hour_data[i], teams_data) << '\n';
        cout << "---" << '\n';
    }
}

bool compare_by_id_employee_ver(const employees e1, const employees e2)
{
    return e1.employee_ID < e2.employee_ID;
}

vector<employees> command ::sort_employee_data_by_ids(vector<employees> e)
{
    sort(e.begin(), e.end(), compare_by_id_employee_ver);
    return e;
}

bool compare_by_id_working_hour_version(const workingHour w1, const workingHour w2)
{
    return w1.employee_ID < w2.employee_ID;
}

bool compare_by_id_team_version(const teams t1, const teams t2)
{
    return t1.team_id < t2.team_id;
}

vector<teams> command ::sort_teams_by_ids(vector<teams> t)
{
    sort(t.begin(), t.end(), compare_by_id_team_version);
    return t;
}

vector<workingHour> command ::sort_working_hour_data_by_ids(vector<workingHour> w)
{
    sort(w.begin(), w.end(), compare_by_id_working_hour_version);
    return w;
}

void workingHour::add_working_time(int day_no, int start_time, int end_time)
{
    for (int i = 0; i < each_employee.size(); i++)
    {
        if (each_employee[i].day == day_no)
        {
            each_employee[i].se_info.push_back({start_time, end_time});

            return;
        }
    }
    each_employee.push_back({day_no});
    int last_index = each_employee.size() - 1;
    each_employee[last_index].se_info.push_back({start_time, end_time});
}

void command::add_working_time_for_specific_employee(int id, int day_number, int start_time, int end_time)
{
    if (check_day_validation(day_number) == false)
        return;
    if (check_time_validation(start_time, end_time) == false)
        return;
    if (check_id_validation(id) == false)
        return;
    int index_employee = find_persons_id(working_hour_data, id);
    working_hour_data[index_employee].check_time_interval_overlap(day_number, start_time, end_time);
    working_hour_data[index_employee].add_working_time(day_number, start_time, end_time);
    cout << PROCESS_DONE << '\n';
}

string find_level(vector<employees> employee_data, int id)
{
    for (int i = 0; i < employee_data.size(); i++)
    {
        if (id == employee_data[i].employee_ID)
            return employee_data[i].level;
    }
    return "";
}

int salary_configs::calculate_total_salary(int id, workingHour working_hour_data, vector<teams> team_data)
{

    int total_hour = working_hour_data.each_employee_total_working_hour();

    int total_salary_s = base_salary + salary_per_hour * total_hour;

    int extra_hours = salary_configs::calculate_extra_hour(total_hour);

    total_salary_s += extra_hours * salary_per_extra_hour;

    if (find_bonus(id, team_data))
        total_salary_s += ceil(total_salary_s * find_bonus(id, team_data)) / 100;

    return total_salary_s;
}

std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

int salary_configs::calculate_earning(int id, workingHour working_hour_data, vector<teams> team_data)
{

    int salary = salary_configs::calculate_total_salary(id, working_hour_data, team_data);
    int earning = salary - calculate_tax_percentage(salary);
    return earning;
}

void error(string message)
{
    cerr << message << endl;
}

vector<string> split_by_delimiter(string str, string delimiter)
{
    vector<string> result;
    int start_pos = 0;
    while (start_pos < str.length())
    {
        int comma_pos = str.find(delimiter, start_pos);
        if (comma_pos == string::npos)
        {
            result.push_back(str.substr(start_pos));
            break;
        }
        result.push_back(str.substr(start_pos, comma_pos - start_pos));
        start_pos = comma_pos + 1;
    }
    return result;
}

void salary_configs ::set_salary_config(string l, string b, string s_p_h, string s_p_e_h, string o, string t)
{
    level = l;
    base_salary = stoi(b);
    salary_per_hour = stoi(s_p_h);
    salary_per_extra_hour = stoi(s_p_e_h);
    official_working_hours = stoi(o);
    tax_percentage = stoi(t);
}

void teams ::set_teams_info(string t, string t_h_i, string m, string b_m_w_h, string b_w_h_v)
{
    team_id = stoi(t);
    team_head_id = stoi(t_h_i);
    bonus_min_working_hour = stoi(b_m_w_h);
    bonus_working_hour_max_variance = stod(b_w_h_v);
    bonus_percentage = 0;
    vector<string> temp_m = split_by_delimiter(m, DOLLAR_SIGN);
    for (int i = 0; i < temp_m.size(); i++)
    {
        member_ids.push_back(stoi(temp_m[i]));
    }
}

void employees ::set_employee_data(string i, string n, string a, string l)
{
    employee_ID = stoi(i);
    name = n;
    age = stoi(a);
    level = l;
}

void workingHour::set_date(string e, string n, string t)
{
    employee_ID = stoi(e);
    each_employee.resize(1);
    each_employee[0].time_interval.push_back(t);
    each_employee[0].day = stoi(n);
    int pos = each_employee[0].time_interval[0].find("-");
    start_time_interval_s = each_employee[0].time_interval[0].substr(0, pos);
    end_time_interval_s = each_employee[0].time_interval[0].substr(pos + 1);
    start_time_interval = stoi(start_time_interval_s);
    end_time_interval = stoi(end_time_interval_s);
    each_employee[0].se_info.push_back({start_time_interval, end_time_interval});
}

void workingHour::check_data(working_time_info w, time_interval_info t)
{
    check_time_interval(t);
    check_number_of_day(w);
}

void workingHour ::check_number_of_day(working_time_info w)
{
    if (w.day > 30 || w.day < 1)
    {
        error("INVALID_ARGUMENTS");
    }
}

void workingHour::check_time_interval(time_interval_info t)
{
    if (t.start_time_interval < 0 || t.start_time_interval > 24 || t.end_time_interval < 0 || t.end_time_interval > 24 || t.start_time_interval > t.end_time_interval)
    {
        error("INVALID_ARGUMENTS");
    }
}

bool check_day_validation(int day_num)
{
    if (day_num > 30 || day_num < 1)
    {
        error("INVALID_ARGUMENTS");
        return false;
    }
    return true;
}

bool check_time_validation(int start_time, int end_time)
{
    if (start_time < 0 || start_time > 24 || end_time < 0 || end_time > 24 || start_time >= end_time)
    {
        error("INVALID_ARGUMENTS");
        return false;
    }
    return true;
}

void workingHour::delete_working_time(int day_num)
{
    if (check_day_validation(day_num) == false)
        return;
    for (int i = 0; i < each_employee.size(); i++)
    {
        if (day_num == each_employee[i].day)
        {
            each_employee.erase(each_employee.begin() + i);
        }
    }
}

int workingHour::each_employee_total_working_hour()
{
    int sum_duration = 0;
    for (int i = 0; i < each_employee.size(); i++)
    {
        for (int j = 0; j < each_employee.at(i).se_info.size(); j++)
        {
            sum_duration += each_employee.at(i).se_info.at(j).end_time_interval - each_employee.at(i).se_info.at(j).start_time_interval;
        }
    }
    return sum_duration;
}

vector<workingHour> delete_merge_repeated_employeeID(vector<workingHour> works)
{
    for (int i = works.size() - 1; i >= 0; i--)
    {
        for (int j = 0; j < i; j++)
        {
            if (works.at(i).employee_ID == works.at(j).employee_ID && i != j)
            {
                works.at(j).each_employee.push_back(works.at(i).each_employee[0]);
                works.erase(works.begin() + i);
                break;
            }
        }
    }
    return works;
}

employees read_employees(string str)
{
    vector<string> fields = split_by_delimiter(str, COMMA);
    employees new_data;
    new_data.set_employee_data(fields[0], fields[1], fields[2], fields[3]);
    return new_data;
}

vector<employees> read_employees_info(string filename)
{
    vector<employees> employees_member;
    ifstream input_csv(filename);
    string line;
    getline(input_csv, line);
    while (getline(input_csv, line))
    {
        employees_member.push_back(read_employees(line));
    }
    input_csv.close();
    return employees_member;
}

salary_configs read_salary_configs(string str)
{
    vector<string> fields = split_by_delimiter(str, COMMA);
    salary_configs new_data;
    new_data.set_salary_config(fields[0], fields[1], fields[2], fields[3], fields[4], fields[5]);
    return new_data;
}

vector<salary_configs> read_salary_configs_info(string filename)
{
    vector<salary_configs> salary_configs_member;
    ifstream input_csv(filename);
    string line;
    getline(input_csv, line);
    while (getline(input_csv, line))
    {
        salary_configs_member.push_back(read_salary_configs(line));
    }
    input_csv.close();
    return salary_configs_member;
}

workingHour read_working_hour(string str)
{
    vector<string> fields = split_by_delimiter(str, COMMA);
    workingHour new_data;
    new_data.set_date(fields[0], fields[1], fields[2]);
    return new_data;
}

vector<workingHour> read_working_hour_info(string filename)
{
    vector<workingHour> works;
    ifstream input_csv(filename);
    string line;
    getline(input_csv, line);
    while (getline(input_csv, line))
    {
        works.push_back(read_working_hour(line));
    }
    input_csv.close();
    works = delete_merge_repeated_employeeID(works);
    return works;
}
teams read_teams(string str)
{
    vector<string> fields = split_by_delimiter(str, COMMA);
    teams new_data;
    new_data.set_teams_info(fields[0], fields[1], fields[2], fields[3], fields[4]);
    return new_data;
}
vector<teams> read_teams_info(string filename)
{
    vector<teams> teams_member;
    ifstream input_csv(filename);
    string line;
    getline(input_csv, line);
    while (getline(input_csv, line))
    {
        teams_member.push_back(read_teams(line));
    }

    input_csv.close();
    return teams_member;
}

void command::print_employee_workingHour_info()
{
    for (int i = 0; i < working_hour_data.size(); i++)
    {
        cout << working_hour_data.at(i).employee_ID << ' ';
        for (int j = 0; j < working_hour_data.at(i).each_employee.size(); j++)
        {
            cout << working_hour_data.at(i).each_employee.at(j).day << ' ';
            for (int t = 0; t < working_hour_data.at(i).each_employee[j].se_info.size(); t++)
            {
                cout << working_hour_data.at(i).each_employee[j].se_info[t].start_time_interval << "-" << working_hour_data.at(i).each_employee[j].se_info[t].end_time_interval << ' ';
            }
        }
        cout << '\n';
    }
}

void command::print_salary_configs_data()
{
    for (int i = 0; i < salary_configs_data.size(); i++)
    {
        cout << salary_configs_data.at(i).level << ' ';
        cout << salary_configs_data.at(i).base_salary << ' ';
        cout << salary_configs_data.at(i).salary_per_hour << ' ';
        cout << salary_configs_data.at(i).salary_per_extra_hour << ' ';
        cout << salary_configs_data.at(i).official_working_hours << ' ';
        cout << salary_configs_data.at(i).tax_percentage;

        cout << '\n';
    }
}

void command::print_employee_personal_info()
{
    for (int i = 0; i < employee_data.size(); i++)
    {
        cout << employee_data.at(i).employee_ID << ' ';
        cout << employee_data.at(i).name << ' ';
        cout << employee_data.at(i).age << ' ';
        cout << employee_data.at(i).level;
        cout << '\n';
    }
}

void command::print_teams_data()
{
    for (int i = 0; i < teams_data.size(); i++)
    {
        cout << teams_data.at(i).team_id << ' ';
        cout << teams_data.at(i).team_head_id << ' ';
        for (int j = 0; j < teams_data.at(i).member_ids.size(); j++)
        {
            cout << teams_data.at(i).member_ids[j] << ' ';
        }
        cout << teams_data.at(i).bonus_min_working_hour << ' ';
        cout << teams_data.at(i).bonus_percentage << ' ';
        cout << teams_data.at(i).bonus_working_hour_max_variance;
        cout << '\n';
    }
}

int salary_configs::calculate_tax_percentage(int salary)
{

    return ceilf32((salary * tax_percentage) / 100);
}
int salary_configs::calculate_extra_hour(int total_working_hour)
{
    int extra_hour = 0;
    extra_hour = (total_working_hour > official_working_hours) ? (total_working_hour - official_working_hours) : 0;
    return extra_hour;
}

void command ::show_salary_config(string level)
{
    int level_id = find_the_level_configs(level);
    if (validate_level(level_id) == false)
        return;
    cout << "Base Salary:" << salary_configs_data[level_id].base_salary << '\n';
    cout << "Salary Per Hour:" << salary_configs_data[level_id].salary_per_hour << '\n';
    cout << "Salary Per Extra Hour:" << salary_configs_data[level_id].salary_per_extra_hour << '\n';
    cout << "Official Working Hour:" << salary_configs_data[level_id].official_working_hours << '\n';
    cout << "Tax:" << salary_configs_data[level_id].tax_percentage << "%" << '\n';
}

bool command::validate_level(int level_id)
{
    if (level_id == MAX_INPUT)
    {
        return false;
    }
    return true;
}

salary_configs command::update_salary_parameters(string level, string b, string s_p_h, string s_p_e_h, string o, string t)
{
    int level_id = find_the_level_configs(level);
    if (validate_level(level_id) == false)
        return salary_configs_data[0];
    if (o == "-")
        o = to_string(salary_configs_data[level_id].official_working_hours);
    if (b == "-")
        b = to_string(salary_configs_data[level_id].base_salary);
    if (s_p_h == "-")
        s_p_h = to_string(salary_configs_data[level_id].salary_per_hour);
    if (s_p_e_h == "-")
        s_p_e_h = to_string(salary_configs_data[level_id].salary_per_extra_hour);
    if (t == "-")
        t = to_string(salary_configs_data[level_id].tax_percentage);

    salary_configs_data[level_id].set_salary_config(level, b, s_p_h, s_p_e_h, o, t);
    cout << PROCESS_DONE << '\n';
    return salary_configs_data[level_id];
}

void command ::report_team_salary(int team_id)
{
    employee_data = sort_employee_data_by_ids(employee_data);
    working_hour_data = sort_working_hour_data_by_ids(working_hour_data);
    if (check_team_id_validation(team_id) == false)
        return;

    teams team_target = find_team_by_data(team_id);
    cout << "ID:" << team_target.team_id << '\n';
    cout << "Head ID:" << team_target.team_head_id << '\n';
    employees head = find_employee_by_id(team_target.team_head_id);
    cout << "Head Name:" << head.name << '\n';
    cout << "Team Total Working Hours:" << calculate_total_team_hours(team_id) << '\n';
    cout << "Average Member Working Hour:" << fixed << setprecision(1) << calculate_total_team_hours(team_id) / teams_data[team_id - 1].member_ids.size() << '\n';
    cout << "Bonus:" << team_target.bonus_percentage << '\n';
    cout << "---" << '\n';
    print_members_data(team_target);
}

teams command::find_team_by_data(int data_id)
{
    for (int i = 0; i < teams_data.size(); i++)
    {
        if (teams_data[i].team_head_id == data_id)
        {
            return teams_data[i];
        }
    }
    return teams_data[0];
}

int command::calculate_total_team_hours(int team_id)
{
    int total_hours_value = 0;
    for (int i = 0; i < teams_data[team_id - 1].member_ids.size(); i++)
    {
        for (int j = 0; j < teams_data[team_id - 1].member_ids.size(); j++)
        {
            total_hours_value += working_hour_data[teams_data[team_id - 1].member_ids[j] - 1].each_employee_total_working_hour();
        }
    }
    return total_hours_value;
}

void command::print_members_data(teams team_target)
{

    sort(team_target.member_ids.begin(), team_target.member_ids.end());
    for (int i = 0; i < team_target.member_ids.size(); i++)
    {
        employees member = find_employee_by_id(team_target.member_ids[i]);
        int level_id = find_the_level_configs(member.level);
        cout << "Member ID:" << member.employee_ID << '\n';
        workingHour working_hour_target = find_working_hour(member.employee_ID);
        cout << "Total Earning:" << salary_configs_data[level_id].calculate_earning(member.employee_ID, working_hour_target, teams_data) << '\n';
        cout << "---" << '\n';
    }
}

void command::update_team_bonus(int team_id, int bonus_percentage)
{
    if (check_team_id_validation(team_id) == false)
        return;
    if (check_bonus_percentage_validation(bonus_percentage) == false)
        return;
    teams team_target = find_team_by_data(team_id);
    team_target.bonus_percentage = bonus_percentage;
    cout << PROCESS_DONE << '\n';
}

void command::delete_working_time_for_specific_employee(int id, int day_number)
{
    if (check_id_validation(id) == false)
        return;
    if (check_day_validation(day_number) == false)
        return;
    int index_employee = find_persons_id(working_hour_data, id);
    working_hour_data[index_employee].check_day_exists(day_number);
    working_hour_data[index_employee].delete_working_time(day_number);
    cout << PROCESS_DONE << '\n';
}

void workingHour::check_day_exists(int day)
{
    for (int i = 0; i < each_employee.size(); i++)
    {
        if (each_employee[i].day == day)
        {
            return;
        }
    }
    error(INVALID_ARGUMENTS_ERROR);
}

bool command::check_day_validation(int day_num)
{
    if (day_num > 30 || day_num < 1)
    {
        error(INVALID_ARGUMENTS_ERROR);
        return false;
    }
    return true;
}

bool command::check_time_validation(int start_time, int end_time)
{
    if (start_time < 0 || start_time > 24 || end_time < 0 || end_time > 24 || start_time >= end_time)
    {
        error(INVALID_ARGUMENTS_ERROR);
        return false;
    }
    return true;
}

bool command::check_id_validation(int id)
{
    for (int i = 0; i < employee_data.size(); i++)
    {
        if (employee_data[i].employee_ID == id)
        {
            return true;
        }
    }
    error(INVALID_EMPLOYEE_ID_ERROR);
    return false;
}

void workingHour::check_time_interval_overlap(int day, int start_time, int end_time)
{
    for (int i = 0; i < each_employee.size(); i++)
    {
        if (each_employee[i].day == day)
        {
            for (int j = 0; j < each_employee[i].se_info.size(); j++)
            {
                if ((start_time > each_employee[i].se_info[j].start_time_interval && start_time < each_employee[i].se_info[j].end_time_interval) || (end_time > each_employee[i].se_info[j].start_time_interval && end_time < each_employee[i].se_info[j].end_time_interval))
                {
                    error(INVALID_TIME_INTERVAL_ERROR);
                }
            }
        }
    }
}

bool command::check_team_id_validation(int team_id)
{
    for (int i = 0; i < teams_data.size(); i++)
    {
        if (teams_data[i].team_id == team_id)
        {
            return true;
        }
    }
    error(INVALID_TEAM_ID_ERROR);
    return false;
}

bool command::check_st_ed_day_validation(int start_day, int end_day)
{
    if (check_day_validation(start_day) == false || check_day_validation(end_day) == false)
        return false;
    if (start_day > end_day)
    {
        error(INVALID_ARGUMENTS_ERROR);
        return false;
    }
    return true;
}

bool command::check_bonus_percentage_validation(int bonus_percentage)
{
    if (bonus_percentage < 0 || bonus_percentage > 100)
    {
        error(INVALID_ARGUMENTS_ERROR);
        return false;
    }
    return true;
}
void command::check_all_working_hour_info()
{
    for (int i = 0; i < working_hour_data.size(); i++)
    {
        for (int j = 0; j < working_hour_data.at(i).each_employee.size(); j++)
        {
            for (int k = 0; k < working_hour_data.at(i).each_employee.at(j).se_info.size(); k++)
            {
                working_hour_data[i].check_data(working_hour_data[i].each_employee[j], working_hour_data[i].each_employee[j].se_info[k]);
            }
        }
    }
}

int find_persons_id(vector<workingHour> w, int persons_id)
{
    for (int i = 0; i < w.size(); i++)
    {
        if (w[i].employee_ID == persons_id)
        {
            return i;
        }
    }
    return MAX_INPUT;
}

void command::print_report_employee_per_hour(vector<vector<double>> report_vec)
{
    for (int i = 0; i < report_vec.size(); i++)
    {
        cout << int(report_vec[i][0]) << "-" << int(report_vec[i][0]) + 1 << ": "
             << fixed << showpoint << setprecision(1) << report_vec[i][1] << '\n';
    }
    cout << "---" << '\n';
    vector<int> busiest_hour = find_busiest(report_vec);
    cout << "Period(s) with Max Working Employees: ";
    for (auto x : busiest_hour)
    {
        cout << x << "-" << x + 1 << " ";
    }
    cout << '\n';
    vector<int> emptiest_hour = find_emptiest(report_vec);
    cout << "Period(s) with Min Working Employees: ";
    for (auto x : emptiest_hour)
    {
        cout << x << "-" << x + 1 << " ";
    }
    cout << '\n';
}

void command::report_employee_per_hour(int start_hour, int end_hour)
{
    if (check_time_validation(start_hour, end_hour) == false)
        return;
    int num_person = 0;
    vector<vector<double>> report_vec;
    for (int hour = start_hour; hour <= end_hour; hour++)
    {
        for (int i = 0; i < working_hour_data.size(); i++)
        {
            for (int j = 0; j < working_hour_data[i].each_employee.size(); j++)
            {
                for (int k = 0; k < working_hour_data[i].each_employee[j].se_info.size(); k++)
                {
                    if (hour >= working_hour_data[i].each_employee[j].se_info[k].start_time_interval &&
                        (hour + 1) <= working_hour_data[i].each_employee[j].se_info[k].end_time_interval)
                    {
                        num_person++;
                    }
                }
            }
        }
        report_vec.push_back({double(hour), double(num_person) / MONTH_SIZE});
        num_person = 0;
    }
    print_report_employee_per_hour(report_vec);
}

void command::print_report_total_hours(vector<vector<double>> report_vec)
{
    for (int i = 0; i < report_vec.size(); i++)
    {
        cout << "Day #" << int(report_vec[i][0]) << ": " << int(report_vec[i][1]) << '\n';
    }
    cout << "---" << '\n';
    vector<int> busiest_days = find_busiest(report_vec);
    cout << "Day(s) with Max Working Hours: ";
    for (auto x : busiest_days)
    {
        cout << x << " ";
    }
    cout << '\n';
    vector<int> emptiest_days = find_emptiest(report_vec);
    cout << "Day(s) with Min Working Hours: ";
    for (auto x : emptiest_days)
    {
        cout << x << " ";
    }
    cout << '\n';
}
vector<int> command::find_busiest(vector<vector<double>> report_vec)
{
    int largest = 0;
    vector<int> busiest_day, vec_person;
    for (int i = 0; i < report_vec.size(); i++)
    {
        vec_person.push_back(report_vec[i][1]);
    }
    int max = *max_element(vec_person.begin(), vec_person.end());
    for (int i = 0; i < report_vec.size(); i++)
    {
        if (report_vec[i][1] == max)
            busiest_day.push_back(report_vec[i][0]);
    }
    return busiest_day;
}

vector<int> command::find_emptiest(vector<vector<double>> report_vec)
{
    int smallest = 0;
    vector<int> emptiest_day, vec_person;
    for (int i = 0; i < report_vec.size(); i++)
    {
        vec_person.push_back(report_vec[i][1]);
    }
    int min = *min_element(vec_person.begin(), vec_person.end());
    for (int i = 0; i < report_vec.size(); i++)
    {
        if (report_vec[i][1] == min)
            emptiest_day.push_back(report_vec[i][0]);
    }
    return emptiest_day;
}
employees command ::find_employee_by_id(int employee_id)
{
    for (int i = 0; i < employee_data.size(); i++)
    {
        if (employee_data[i].get_employee_iD() == employee_id)
        {
            return employee_data[i];
        }
    }
    return employee_data[0];
}
workingHour command::find_working_hour(int employee_id)
{
    for (int i = 0; i < working_hour_data.size(); i++)
    {
        if (working_hour_data[i].employee_ID == employee_id)
        {
            return working_hour_data[i];
        }
    }
    return working_hour_data[0];
}

int find_team_id_employee(int id, vector<teams> team_data)
{
    for (int i = 0; i < team_data.size(); i++)
    {
        for (int j = 0; j < team_data[i].member_ids.size(); j++)
        {
            if (team_data[i].member_ids[j] == id)
            {
                return team_data[i].team_id;
            }
        }
    }
    return MAX_INPUT;
}
int teams::all_team_members_total_working_hour(vector<workingHour> working_hour_data)
{
    int sum_total_hours = 0;
    for (int i = 0; i < member_ids.size(); i++)
    {
        int emp_index = find_persons_id(working_hour_data, member_ids[i]);
        sum_total_hours = sum_total_hours + working_hour_data[emp_index].each_employee_total_working_hour();
    }
    return sum_total_hours;
}

double teams::all_team_members_total_working_hour_variance(vector<workingHour> working_hour_data, int all_team_members_working_hours, int num_team_members)
{
    double sum_total_squares;
    double mean_working_hour = double(all_team_members_working_hours) / double(num_team_members);
    for (int i = 0; i < member_ids.size(); i++)
    {
        int emp_index = find_persons_id(working_hour_data, member_ids[i]);
        sum_total_squares = sum_total_squares + pow(mean_working_hour - working_hour_data[emp_index].each_employee_total_working_hour(), 2);
    }
    double variance = sum_total_squares / double(num_team_members);
    return variance;
}

void command::find_teams_for_bonus()
{
    int flag = 0;
    for (int i = 0; i < teams_data.size(); i++)
    {
        int all_team_members_working_hours = teams_data[i].all_team_members_total_working_hour(working_hour_data);
        int num_team_members = teams_data[i].member_ids.size();
        double all_team_members_variance = teams_data[i].all_team_members_total_working_hour_variance(working_hour_data, all_team_members_working_hours, num_team_members);
        if (all_team_members_working_hours > teams_data[i].bonus_min_working_hour && all_team_members_variance < teams_data[i].bonus_working_hour_max_variance)
        {
            cout << "Team ID: " << teams_data[i].team_id << '\n';
            flag = 1;
        }
    }
    if (flag == 0)
    {
        cout << NO_BONUS_ERROR << '\n';
    }
}

int workingHour ::caculate_total_hours_for_each_day(int day)
{
    int sum_hour = 0;
    for (int i = 0; i < each_employee.size(); i++)
    {
        if (each_employee[i].day == day)
        {
            for (int j = 0; j < each_employee[i].se_info.size(); j++)
            {
                sum_hour = sum_hour + each_employee[i].se_info[j].end_time_interval - each_employee[i].se_info[j].start_time_interval;
            }
        }
    }
    return sum_hour;
}

void command::report_total_hours_per_day(int start_day, int end_day)
{
    if (check_st_ed_day_validation(start_day, end_day) == false)
        return;
    int sum_hour = 0;
    vector<vector<double>> report_vec;
    for (int day = start_day; day <= end_day; day++)
    {
        for (int i = 0; i < working_hour_data.size(); i++)
        {
            sum_hour = sum_hour + working_hour_data[i].caculate_total_hours_for_each_day(day);
        }
        report_vec.push_back({double(day), double(sum_hour)});
        sum_hour = 0;
    }
    print_report_total_hours(report_vec);
}

int find_bonus(int id, vector<teams> team_data)
{
    for (int i = 0; i < team_data.size(); i++)
    {
        for (int j = 0; j < team_data[i].member_ids.size(); j++)
        {
            if (team_data[i].member_ids[j] == id)
            {
                return team_data[i].bonus_percentage;
            }
        }
    }
    return 0;
}