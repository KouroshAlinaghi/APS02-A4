#include <stdexcept>
#include <utility>
#include<cassert>
#include<fstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

const string EMPLOYEES_FILE_NAME = "employees.csv";
const string WORKING_HOURS_FILE_NAME = "working_hours.csv";
const string TEAMS_FILE_NAME = "teams.csv";
const string SALARY_CONFIGS_FILE_NAME = "salary_configs.csv";
const int8_t NO_TEAM = -1;
const int MONTH_DAY_COUNT = 30;

class WorkingDateTime;
class Database;
class Employee;
class SalaryConfig;
class Team;

typedef map<string,string> Nigger;

enum ProficiencyLevel {
    JUNIOR,
    EXPERT,
    SENIOR,
    TEAM_LEAD
};

vector <string> split(string str, char delimeter){
    vector <string> res;
    string cur = "";
    for(char c : str)
        if(c == delimeter)
            res.push_back(cur), cur = "";
        else
            cur += c;
    res.push_back(cur);
    return res;
}

typedef pair<int, int> TimeRange;

class WorkingDateTime {
private:
    int day;
    TimeRange time_range;
public:
    WorkingDateTime(int day, TimeRange range){
        day = day;
        time_range = range;
    }
    int get_day() { return day; }
    int get_length() { return time_range.second - time_range.first; }
};

class Database {
private:
    vector <Employee> employees;
    vector <SalaryConfig> salary_configs;
    vector <Team> teams;
public:
    SalaryConfig get_salary_config(ProficiencyLevel level);
    void add_employee(Employee employee);
    void add_config(SalaryConfig conf);
    void add_team(Team team);
    void handle_hour_data(Nigger data);
    Employee get_employee(int id);
    Employee* get_pointer_to_employee(int id);
    Team get_team(int id);
    vector <Employee> get_employees();
    int get_total_working_hours_of_day(int day);
};

ProficiencyLevel get_level(string level){   
    if(level == "team_lead"){
        return TEAM_LEAD;
    }
    else if(level == "senior"){
        return SENIOR;
    }
    else if(level == "expert"){
        return EXPERT;
    }
    else{
        return JUNIOR;
    }
}  
class SalaryConfig {
private:
    ProficiencyLevel level;
    int base_salary;
    int salary_per_hour;
    int salary_per_extra_hour;
    int official_working_hours;
    int tax_percentage;
public:
    int calculate_raw_salary(vector <WorkingDateTime> working_date_times) {
        int salary = base_salary, total_working_hours = 0;
        for (WorkingDateTime working_date_time : working_date_times) {
            total_working_hours += working_date_time.get_length();
            salary += working_date_time.get_length() * salary_per_hour;
        }
        if (total_working_hours > official_working_hours)
            salary += (total_working_hours - official_working_hours) * salary_per_extra_hour;
        
        return salary;
    }
    SalaryConfig(Nigger salary_config){
        base_salary = stoi(salary_config["base_salary"]);
        salary_per_hour = stoi(salary_config["salary_per_hour"]);
        salary_per_extra_hour = stoi(salary_config["salary_per_extra_hour"]);
        official_working_hours = stoi(salary_config["official_working_hours"]);
        tax_percentage = stoi(salary_config["tax_percentage"]);
        level = ::get_level(salary_config["level"]);
    }
    int apply_tax(int salary) {
        return salary - get_tax_amount(salary);
    }
    int get_tax_amount(int salary) {
        return salary * tax_percentage / 100;
    }
    ProficiencyLevel get_level() { return level; }
    int get_base_salary() { return base_salary; }
    int get_salary_per_hour() { return salary_per_hour; }
    int get_salary_per_extra_hour() { return salary_per_extra_hour; }
    int get_official_working_hours() { return official_working_hours; }
    int get_tax_percentage() { return tax_percentage; }
    void update_configs(ProficiencyLevel level, int base_salary, int salary_per_hour, int salary_per_extra_hour, int official_working_hours, int tax_percentage) {
        level = level;
        base_salary = base_salary;
        salary_per_hour = salary_per_hour;
        salary_per_extra_hour = salary_per_extra_hour;
        official_working_hours = official_working_hours;
        tax_percentage = tax_percentage;
    }
};

class Employee {
private:
    int id;
    string name;
    int age;
    ProficiencyLevel level;

    vector <WorkingDateTime> working_date_times;

    int team_id;

    int salary;
    int total_earning;
    
    int calculate_salary(Database db) {
        return db.get_salary_config(level).calculate_raw_salary(working_date_times);
    }
public:
    Employee (Nigger data, Database db){
        id = stoi(data["id"]);
        name = data["name"];
        age = stoi(data["age"]);
        level = ::get_level(data["level"]);
        team_id = NO_TEAM;
        salary = calculate_salary(db);
        total_earning = db.get_salary_config(level).apply_tax(salary);
    }
    bool has_team() { return team_id != NO_TEAM; }
    void delete_working_hours(int day) {
        for (int i = 0; i < (int)working_date_times.size(); i++)
            if (working_date_times[i].get_day() == day)
                working_date_times.erase(working_date_times.begin() + i);
    }
    void add_working_date_time(WorkingDateTime working_date_time) {
        working_date_times.push_back(working_date_time);
    }
    void join_team(Team team);
    void recalculate_salary_and_earning(Database db) {
        salary = calculate_salary(db);
        total_earning = db.get_salary_config(level).apply_tax(salary);
    }
    int get_total_working_hours() {
        int total_working_hours = 0;
        for (WorkingDateTime working_date_time : working_date_times)
            total_working_hours += working_date_time.get_length();
        return total_working_hours;
    }
    bool does_work_on_day(int day) {
        for (WorkingDateTime working_date_time : working_date_times)
            if (working_date_time.get_day() == day)
                return true;
        
        return false;
    }
    int get_salary() { return salary; }
    int get_total_earning() { return total_earning; }
    int get_id() { return id; }
    int get_age() { return age; }
    int get_team_id() { return team_id; }
    int get_tax_amount(Database db) { return db.get_salary_config(level).get_tax_amount(salary); }
    string get_name() { return name; }
    ProficiencyLevel get_level() { return level; }
    string get_level_humanized() {
        switch (level) {
            case JUNIOR:
                return "junior";
            case EXPERT:
                return "expert";
            case SENIOR:
                return "senior";
            case TEAM_LEAD:
                return "team lead";
        }
    }
    int get_number_of_absent_days() { return MONTH_DAY_COUNT - working_date_times.size(); }
};

vector<int> string_to_int_vector(const vector<string>& str_vector) {
    vector<int> int_vector;
    for (const auto& str : str_vector) {
        int_vector.push_back(stoi(str));
    }
    return int_vector;
}

class Team {
private:
    int id;
    int team_head_id;
    int bonus_percentage;
    int bonus_min_working_hours;
    vector<int> member_ids;
    double bonus_working_hours_max_variance;
public:
    Team (Nigger data){
        id = stoi(data["team_id"]);
        team_head_id = stoi(data["team_head_id"]);
        bonus_min_working_hours = stoi(data["bonus_min_working_hours"]);
        bonus_working_hours_max_variance = stod(data["bonus_working_hours_max_variance"]);
        member_ids = string_to_int_vector(split(data["member_ids"], '$'));
        bonus_percentage = 0;
    }
    int get_id() { return id; }
    int get_team_head_id() { return team_head_id; }
    string get_head_member_name(Database db) { return db.get_employee(team_head_id).get_name(); }
    vector <Employee> get_employees(Database db) {
        vector <Employee> members, employees = db.get_employees();
        for (Employee employee : employees)
            if (employee.get_team_id() == id)
                members.push_back(employee);
        return members;
    }
    vector<int> get_ids(){return member_ids;}
    int get_number_of_members(Database db) { return get_employees(db).size(); }
    int get_total_working_hours(Database db) {
        int total_working_hours = 0;
        vector <Employee> employees = db.get_employees();
        for (Employee employee : employees)
            total_working_hours += employee.get_total_working_hours();
        return total_working_hours;
    }
    int get_average_member_working_hours(Database db) { return get_total_working_hours(db) / get_number_of_members(db); }
    void update_bonus_percentage(int new_bonus_percentage) { bonus_percentage = new_bonus_percentage; }
};

// Defining bodies of methods that caused dependency loops
Employee Database::get_employee(int id) {
    for (Employee employee : employees)
        if (employee.get_id() == id)
            return employee;
    throw runtime_error("employee not found");
}
Employee* Database::get_pointer_to_employee(int id) {
    for (Employee &employee : employees)
        if (employee.get_id() == id)
            return &employee;
    throw runtime_error("employee (pointer) not found");
}
Team Database::get_team(int id) {
    for (Team team : teams)
        if (team.get_id() == id)
            return team;
    throw runtime_error("team not found");
}
int Database::get_total_working_hours_of_day(int day) {
    int total_hours = 0;
    for (Employee employee : employees)
        if (employee.does_work_on_day(day))
            total_hours += employee.get_total_working_hours();
    return total_hours;
}
void Database::add_employee(Employee employee){
    employees.push_back(employee);
}
void Database::add_config(SalaryConfig conf){
    salary_configs.push_back(conf);
}
void Database::add_team(Team team){
    teams.push_back(team);
    auto ids = teams.back().get_ids();
    for(int id : ids)
        get_pointer_to_employee(id)->join_team(teams.back());
}

TimeRange vector_to_pair(const vector<string>& int_vector) {
    return make_pair(stoi(int_vector[0]), stoi(int_vector[1]));
}

void Database::handle_hour_data(Nigger data){
    int id = stoi(data["employee_id"]);
    int day = stoi(data["day"]);
    auto times = split(data["working_interval"], '-');
    get_pointer_to_employee(id)->add_working_date_time(WorkingDateTime(day, vector_to_pair(times)));
}
vector <Employee> Database::get_employees() { return employees; }
void Employee::join_team(Team team) { team_id = team.get_id(); }
SalaryConfig Database::get_salary_config(ProficiencyLevel level){
    for (SalaryConfig config : salary_configs)
        if (config.get_level() == level)
            return config;
    throw runtime_error("salary config not found");
}

string read_next_line(ifstream& file){
    string res;
    getline(file, res);
    return res;
}

typedef vector <vector <string>> StringTable;

StringTable read_csv(string file_name){
    ifstream file(file_name);
    vector <string> new_line;
    StringTable lines;
    while(file.peek()){
        new_line = split(read_next_line(file), ',');
        lines.push_back(new_line);
    }
    return lines;
}

Nigger make_map(vector < string > keys, vector < string > values){
    Nigger res;
    assert(keys.size() == values.size());
    for(int i = 0 ; i < (int)keys.size() ; i ++)
        res[keys[i]] = values[i];
    return res;
}

void get_employees_input(Database& db){
    StringTable employees_raw_info = read_csv(EMPLOYEES_FILE_NAME);
    for(int i = 1 ; i < (int)employees_raw_info.size() ; i ++)
        db.add_employee(Employee(make_map(employees_raw_info[0], employees_raw_info[i]), db));
}

void get_salary_configs(Database& db){
    StringTable configs_raw_info = read_csv(SALARY_CONFIGS_FILE_NAME);
    for(int i = 1 ; i < (int)configs_raw_info.size() ; i ++)
        db.add_config(SalaryConfig(make_map(configs_raw_info[0], configs_raw_info[i])));
}

void get_teams_input(Database& db){
    StringTable teams_raw_info = read_csv(TEAMS_FILE_NAME);
    for(int i = 1 ; i < (int)teams_raw_info.size() ; i ++)
        db.add_team(Team(make_map(teams_raw_info[0], teams_raw_info[i])));
}

void get_working_hours_input(Database& db){
    StringTable hours_raw_info = read_csv(WORKING_HOURS_FILE_NAME);
    for(int i = 1 ; i < (int)hours_raw_info.size() ; i ++)
        db.handle_hour_data(make_map(hours_raw_info[0], hours_raw_info[i]));
}

int main(){
    Database database;
    get_salary_configs(database);
    get_employees_input(database);
    get_teams_input(database);
    get_working_hours_input(database);
    return 0;
}
