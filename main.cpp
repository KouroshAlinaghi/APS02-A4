#include <iomanip>
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <utility>
#include<cassert>
#include<fstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

using namespace std;

//remove csv/

const string EMPLOYEES_FILE_NAME = "/employees.csv";
const string WORKING_HOURS_FILE_NAME = "/working_hours.csv";
const string TEAMS_FILE_NAME = "/teams.csv";
const string SALARY_CONFIGS_FILE_NAME = "/salary_configs.csv";
const int8_t NO_TEAM = -1;
const int MONTH_DAY_COUNT = 30;

class WorkingDateTime;
class Database;
class Employee;
class SalaryConfig;
class Team;

typedef map<string,string> Dictionary;
typedef vector <vector <string>> StringTable;

enum ProficiencyLevel {
    JUNIOR,
    EXPERT,
    SENIOR,
    TEAM_LEAD
};

#define debug(x) cout << x, exit(0)

vector <string> split(string str, char delimeter){
    vector <string> res;
    string cur = "";
    for(char c : str)
        if(c == delimeter or c == '\r')
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
    WorkingDateTime(int day_num, TimeRange range){
        day = day_num;
        time_range = range;
    }
    int get_day() { return day; }
    int get_length() { return time_range.second - time_range.first; }
    int get_start(){return time_range.first;}
    int get_end(){return time_range.second;}
    bool overlaps(WorkingDateTime time){
        if(time.get_day() != day)
            return false;
        if(time_range.first >= time.get_end()) 
            return false;
        if(time_range.second <= time.get_start())
            return false;
        return true;
    }
};

class Database {
private:
    vector <Employee> employees;
    vector <SalaryConfig> salary_configs;
    vector <Team> teams;
public:
    SalaryConfig get_salary_config(ProficiencyLevel level);
    SalaryConfig* get_pointer_to_salary_config(ProficiencyLevel level);
    void add_employee(Employee employee);
    void add_config(SalaryConfig conf);
    void add_team(Team team);
    void handle_hour_data(Dictionary data);
    void report_salaries();
    void report_salary(int id);
    void report_team_salary(int id);
    Employee get_employee(int id);
    Employee* get_pointer_to_employee(int id);
    Team get_team(int id);
    Team* get_pointer_to_team(int id);
    vector <Employee> get_employees();
    int get_total_working_hours_of_day(int day);
    void report_total_hours_in_range(int l, int r);
    void print_salary_config(string level_name);
    void update_salary_config(vector <string> input);
    void add_working_hours(int id, int day, int l, int r);
    void delete_working_hours(int id, int day);
    void update_team_bonus(int id, int bonus);
    void recalculate_salaries();
    void report_employee_per_hour(int l, int r);
    int count_busy_employees(TimeRange time);
    int min_value_in_map(map<TimeRange,int>);
    int max_value_in_map(map<TimeRange,int>);
    void find_teams_for_bonus(Database &db);
    int get_total_working_hours_of_employee(int id);
    double calculate_avg(vector < int > vals);
    double calculate_variance(vector < int > vals);
};

ProficiencyLevel get_level(string level){   
    if(level == "team_lead"){
        return TEAM_LEAD;
    }
    if(level == "senior"){
        return SENIOR;
    }
    if(level == "expert"){
        return EXPERT;
    }
    if(level == "junior"){
        return JUNIOR;
    }
    throw runtime_error("level not found");
}  
bool is_invalid_day(int day){
    return day < 1 or day > MONTH_DAY_COUNT;
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
    double calculate_raw_salary(vector <WorkingDateTime> working_date_times) {
        double salary = base_salary;
        int total_working_hours = 0;
        for (WorkingDateTime working_date_time : working_date_times) {
            total_working_hours += working_date_time.get_length();
            salary += (double)working_date_time.get_length() * (double)salary_per_hour;
        }
        if (total_working_hours > official_working_hours)
            salary += (double)((double)total_working_hours - (double)official_working_hours) * (double)salary_per_extra_hour;
        
        return salary;
    }
    SalaryConfig(Dictionary salary_config){
        base_salary = stoi(salary_config["base_salary"]);
        salary_per_hour = stoi(salary_config["salary_per_hour"]);
        salary_per_extra_hour = stoi(salary_config["salary_per_extra_hour"]);
        official_working_hours = stoi(salary_config["official_working_hours"]);
        tax_percentage = stoi(salary_config["tax_percentage"]);
        level = ::get_level(salary_config["level"]);
    }
    double get_tax_amount(double salary) {
        return salary * (double)tax_percentage / 100.0;
    }
    ProficiencyLevel get_level() { return level; }
    int get_base_salary() { return base_salary; }
    int get_salary_per_hour() { return salary_per_hour; }
    int get_salary_per_extra_hour() { return salary_per_extra_hour; }
    int get_official_working_hours() { return official_working_hours; }
    int get_tax_percentage() { return tax_percentage; }
    void set_base_salary(int x) { base_salary = x; }
    void set_salary_per_hour(int x) { salary_per_hour = x; }
    void set_salary_per_extra_hour(int x) { salary_per_extra_hour = x; }
    void set_official_working_hours(int x) { official_working_hours = x; }
    void set_tax_percentage(int x) { tax_percentage = x; }
    void print_config(){
        cout << "Base Salary: " << base_salary << endl;
        cout << "Salary Per Hour: " << salary_per_hour << endl;
        cout << "Salary Per Extra Hour: " << salary_per_extra_hour << endl;
        cout << "Official Working Hours: " << official_working_hours << endl;
        cout << "Tax: " << tax_percentage << '%' << endl;
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

    double raw_salary;
    double total_earning;
    
    double calculate_raw_salary(Database db) {
        return db.get_salary_config(level).calculate_raw_salary(working_date_times);
    }
public:
    Employee (Dictionary data, Database db){
        id = stoi(data["id"]);
        name = data["name"];
        age = stoi(data["age"]);
        level = ::get_level(data["level"]);
        team_id = NO_TEAM;
        raw_salary = calculate_raw_salary(db);
        total_earning = raw_salary - get_tax_amount(db) + get_bonus_amount(db);
    }
    bool has_team() { return team_id != NO_TEAM; }
    void delete_working_hours(int day) {
        vector <WorkingDateTime> result;
        for(auto time : working_date_times){
            if(time.get_day() != day)
                result.push_back(time);
        }
        working_date_times = result;
    }
    void add_working_date_time(WorkingDateTime working_date_time) {
        working_date_times.push_back(working_date_time);
    }
    void join_team(Team team);
    void recalculate_salary_and_earning(Database db) {
        total_earning = raw_salary - get_tax_amount(db) + get_bonus_amount(db);
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
    int calculate_total_working_hours(){
        int total = 0;
        for(auto time : working_date_times)
            total += time.get_length();
        return total;
    }   
    void print_salary_report(){
        cout << "ID: " << id << endl;
        cout << "Name: " << name << endl; 
        cout << "Total Working Hours: " << calculate_total_working_hours() << endl;   
        cout << "Total Earning: " << total_earning << endl; 
        cout << "---" << endl;
    }
    string get_level_humanized() {
        switch (level) {
            case JUNIOR:
                return "junior";
            case EXPERT:
                return "expert";
            case SENIOR:
                return "senior";
            default:
                return "team lead";
        }
    }
    double get_raw_salary() { return raw_salary; }
    double get_total_earning() { return total_earning; }
    int get_id() { return id; }
    int get_age() { return age; }
    int get_team_id() { return team_id; }
    double get_tax_amount(Database db) {
        return db.get_salary_config(level).get_tax_amount(raw_salary+get_bonus_amount(db));
    }
    double get_bonus_amount(Database db);
    int count_absent_days(){
        bool is_present[MONTH_DAY_COUNT+5];
        memset(is_present, 0, sizeof is_present);
        for(WorkingDateTime working_date_time : working_date_times){
            if(working_date_time.get_length())
                is_present[working_date_time.get_day()] = 1;
        }
        int absent_days_count = 0;
        for(int i = 1 ; i <= MONTH_DAY_COUNT ; i ++)
            absent_days_count += !is_present[i];
        return absent_days_count;
    }
    void print_team_id(){
        if(team_id == NO_TEAM)
            cout << "N/A";
        else
            cout << team_id;
        cout << endl;
    }
    void print_detailed_salary_report(Database &db){
        cout << "ID: " << get_id() << endl;
        cout << "Name: " << get_name() << endl;
        cout << "Age: " << get_age() << endl;
        cout << "Level: " <<  get_level_humanized() << endl;
        cout << "Team ID: ", print_team_id();
        cout << "Total Working Hours: " << calculate_total_working_hours() << endl; 
        cout << "Absent Days: " << count_absent_days() << endl;
        cout << "Salary: " << fixed << setprecision(0) << get_raw_salary() << endl; //gerd kon agha
        cout << "Bonus: " << fixed << setprecision(0) << get_bonus_amount(db) << endl; 
        cout << "Tax: " << fixed << setprecision(0) << get_tax_amount(db) << endl;
        cout << "Total Earning: " << fixed << setprecision(0) << total_earning << endl; 
    }
    string get_name() { return name; }
    ProficiencyLevel get_level() { return level; }
    bool is_busy(WorkingDateTime cur_time){
        for(auto time : working_date_times)
            if(time.overlaps(cur_time))
                return 1;
        return 0;
    }
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
    Team (Dictionary data){
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
    int get_total_working_hours(Database db);
    double get_average_member_working_hours(Database db) { return (double)get_total_working_hours(db) / (double)get_number_of_members(db); }
    void update_bonus_percentage(int new_bonus_percentage) { bonus_percentage = new_bonus_percentage; }
    void report_salary(Database db);
    int get_bonus_percentage() { return bonus_percentage; }
    bool is_eligible_for_bonus(Database &db);
    double calculate_variance(Database &db);
};


TimeRange vector_to_pair(const vector<string>& int_vector) {
    return make_pair(stoi(int_vector[0]), stoi(int_vector[1]));
}

bool is_invalid_date_range(int l, int r){
    if(r < l)
        return 1;
    if(l < 1)
        return 1;
    if(r > MONTH_DAY_COUNT)
        return 1;
    return 0;
}

bool is_invalid_time_range(int l, int r){
    if(r <= l)
        return 1;
    if(l < 0)
        return 1;
    if(r > 24)
        return 1;
    return 0;
}

bool is_valid_percentage(int x){
    return x>=0 and x <= 100;
}

string read_next_line(ifstream& file){
    string res;
    getline(file, res);
    return res;
}

StringTable read_csv(string file_name){
    ifstream file(file_name);
    vector <string> new_line;
    StringTable lines;
    while(file.peek() != EOF){
        new_line = split(read_next_line(file), ',');
        lines.push_back(new_line);
    }
    return lines;
}

Dictionary make_map(vector < string > keys, vector < string > values){
    Dictionary res;
    assert(keys.size() == values.size());
    for(int i = 0 ; i < (int)keys.size() ; i ++)
        res[keys[i]] = values[i];
    return res;
}

void get_employees_input(string file_prefix, Database& db){
    string file_name = file_prefix + EMPLOYEES_FILE_NAME;
    StringTable employees_raw_info = read_csv(file_name.c_str());
    for(int i = 1 ; i < (int)employees_raw_info.size() ; i ++)
        db.add_employee(Employee(make_map(employees_raw_info[0], employees_raw_info[i]), db));
}

void get_salary_configs(string file_prefix, Database& db){
    string file_name = file_prefix + SALARY_CONFIGS_FILE_NAME;
    StringTable configs_raw_info = read_csv(file_name.c_str());
    for(int i = 1 ; i < (int)configs_raw_info.size() ; i ++)
        db.add_config(SalaryConfig(make_map(configs_raw_info[0], configs_raw_info[i])));
}

void get_teams_input(string file_prefix, Database& db){
    string file_name = file_prefix + TEAMS_FILE_NAME;
    StringTable teams_raw_info = read_csv(file_name.c_str());
    for(int i = 1 ; i < (int)teams_raw_info.size() ; i ++)
        db.add_team(Team(make_map(teams_raw_info[0], teams_raw_info[i])));
}

void get_working_hours_input(string file_prefix, Database& db){
    string file_name = file_prefix + WORKING_HOURS_FILE_NAME;
    StringTable hours_raw_info = read_csv(file_name.c_str());
    for(int i = 1 ; i < (int)hours_raw_info.size() ; i ++)
        db.handle_hour_data(make_map(hours_raw_info[0], hours_raw_info[i]));
}

void get_file_inputs(string file_prefix, Database &db){
    get_salary_configs(file_prefix, db);
    get_employees_input(file_prefix, db);
    get_teams_input(file_prefix, db);
    get_working_hours_input(file_prefix, db);
}

void process_stdin_input(Database &db){
    string new_line;
    while(getline(cin, new_line)){
        vector < string > words = split(new_line, ' ');
        if(words.empty())
            continue;
        if(words.front() == "report_salaries")
            db.report_salaries();
        if(words.front() == "report_employee_salary")
            db.report_salary(stoi(words[1]));
        if(words.front() == "report_team_salary")
            db.report_team_salary(stoi(words[1]));
        if(words.front() == "report_total_hours_per_day")
            db.report_total_hours_in_range(stoi(words[1]), stoi(words[2]));
        if(words.front() == "report_employee_per_hour")
            db.report_employee_per_hour(stoi(words[1]), stoi(words[2]));
        if(words.front() == "show_salary_config")
            db.print_salary_config(words[1]);
        if(words.front() == "update_salary_config")
            db.update_salary_config(vector<string>(words.begin()+1, words.end()));
        if(words.front() == "add_working_hours")
            db.add_working_hours(stoi(words[1]), stoi(words[2]), stoi(words[3]), stoi(words[4]));
        if(words.front() == "delete_working_hours")
            db.delete_working_hours(stoi(words[1]), stoi(words[2]));
        if(words.front() == "update_team_bonus")
            db.update_team_bonus(stoi(words[1]), stoi(words[2]));
        if(words.front() == "find_teams_for_bonus")
            db.find_teams_for_bonus(db);
        db.recalculate_salaries();
    }
}

int main(int argc, char* argv[]){
    assert(argc > 1);
    Database database;
    get_file_inputs(argv[1], database);
    process_stdin_input(database);
    return 0;
}

void Database::report_total_hours_in_range(int l, int r){
    if(is_invalid_date_range(l, r)){
        cout << "INVALID_ARGUMENTS" << endl;
        return;
    }
    int total_working_hours[MONTH_DAY_COUNT + 5];
    memset(total_working_hours, 0, sizeof total_working_hours);
    for(int day = l, indx = 1 ; day <= r ; day ++, indx++)
        total_working_hours[day] = get_total_working_hours_of_day(day),
        cout << "DAY #" << indx << ": " << total_working_hours[day] << endl;
    cout << "---" << endl;
    int max_working_hours = *max_element(total_working_hours + l, total_working_hours + r + 1);
    int min_working_hours = *min_element(total_working_hours + l, total_working_hours + r + 1);
    cout << "Day(s) with Max Working Hours: ";
    for(int day = l ; day <= r ; day ++)
        if(total_working_hours[day] == max_working_hours)
            cout << day << ' ';
    cout << endl;
    cout << "Day(s) with Min Working Hours: ";
    for(int day = l ; day <= r ; day ++)
        if(total_working_hours[day] == min_working_hours)
            cout << day << ' ';
    cout << endl;
}

void Database::print_salary_config(string level_name){
    try{
        get_salary_config(::get_level(level_name)).print_config();
    }
    catch(exception &e){
        cout << "INVALID_LEVEL" << endl;
    }
}

vector <Employee> Database::get_employees() { return employees; }

void Employee::join_team(Team team) { team_id = team.get_id(); }

SalaryConfig Database::get_salary_config(ProficiencyLevel level){
    for (SalaryConfig config : salary_configs)
        if (config.get_level() == level)
            return config;
    throw runtime_error("salary config not found");
}

SalaryConfig* Database::get_pointer_to_salary_config(ProficiencyLevel level){
    for (SalaryConfig& config : salary_configs)
        if (config.get_level() == level)
            return &config;
    throw runtime_error("salary config not found");
}

void Team::report_salary(Database db){
    cout << "ID: " << id << endl;
    cout << "Head ID: " << team_head_id << endl;
    cout << "Head Name: " << db.get_employee(team_head_id).get_name() << endl;
    cout << "Team Total Working Hours: " << get_total_working_hours(db) << endl;
    cout << "Average Member Working Hour: " << fixed << setprecision(1) << get_average_member_working_hours(db) << endl;
    cout << "Bonus: " << get_bonus_percentage() << endl;
    cout << "---" << endl;
    for(int id : member_ids){
        cout << "Member ID: " << id << endl;
        cout << "Total Earning: " << fixed << setprecision(0) << db.get_employee(id).get_total_earning() << endl;
        cout << "---" << endl;  
    }
}

double Employee::get_bonus_amount(Database db) {
    if (!has_team()) return 0;
    return raw_salary * db.get_team(team_id).get_bonus_percentage() / 100;
}

void Database::recalculate_salaries() {
    for (auto& emp : employees)
        emp.recalculate_salary_and_earning(*this);
}

void Database::update_salary_config(vector<string> input){  
    SalaryConfig* conf;  
    try{
        conf = get_pointer_to_salary_config(::get_level(input[0]));
    } 
    catch(exception &e){
        cout << "INVALID_LEVEL" << endl;
        return;
    }   
    if(input[1] != "-")conf->set_base_salary(stoi(input[1]));
    if(input[2] != "-")conf->set_salary_per_hour(stoi(input[2]));
    if(input[3] != "-")conf->set_salary_per_extra_hour(stoi(input[3]));
    if(input[4] != "-")conf->set_official_working_hours(stoi(input[4]));
    if(input[5] != "-")conf->set_tax_percentage(stoi(input[5]));
    cout << "OK" << endl;
}

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

Team* Database::get_pointer_to_team(int id) {
    for (Team &team : teams)
        if (team.get_id() == id)
            return &team;
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

void Database::report_salaries(){
    for(auto employee: employees)
        employee.print_salary_report();
}

void Database::delete_working_hours(int id, int day){
    Employee* emp;
    try{
        emp = get_pointer_to_employee(id);
    }
    catch(exception &e){
        cout << "EMPLOYEE_NOT_FOUND" << endl;
        return;
    }
    if(is_invalid_day(day)){
        cout << "INVALID_ARGUMENTS" << endl;
        return;
    }
    emp->delete_working_hours(day);
    cout << "OK" << endl;
}

int Database::count_busy_employees(TimeRange time){
    int cnt = 0;
    for(int day = 1 ; day <= MONTH_DAY_COUNT ; day ++)
        for(auto emp : employees)
            cnt += emp.is_busy({day, time});
    return cnt;
}
//typedef?
int Database::max_value_in_map(map<TimeRange, int> mp){
    return max_element(mp.begin(), mp.end(), [](pair <TimeRange, int> x, pair <TimeRange, int> y){return x.second < y.second;})->second;
}

int Database::min_value_in_map(map<TimeRange, int> mp){
    return min_element(mp.begin(), mp.end(), [](pair <TimeRange, int> x, pair <TimeRange, int> y){return x.second < y.second;})->second;
}

void Database::report_employee_per_hour(int l, int r){ 
    //probably better if this was shorter/decomposed into smaller functions
    if(::is_invalid_time_range(l, r)){
        cout << "INVALID_ARGUMENTS" << endl;
        return;
    }
    map<TimeRange,int> working_cnt;
    for(int start = l ; start < r ; start ++){
        TimeRange cur_time = {start, start+1};
        working_cnt[cur_time] = count_busy_employees(cur_time);
        cout << start << '-' << start+1 << ": ";
        cout << fixed << setprecision(1) << double(working_cnt[cur_time])/MONTH_DAY_COUNT;
        cout << endl;
    }
    cout << "---" << endl;
    int max_cnt = max_value_in_map(working_cnt);
    int min_cnt = min_value_in_map(working_cnt);
    cout << "Period(s) with Max Working Employees: ";
    for(int start = l ; start < r ; start ++){
        TimeRange cur_time = {start, start+1};
        if(working_cnt[cur_time] == max_cnt)
            cout << start << '-' << start + 1 << ' ';
    }
    cout << endl << "Period(s) with Min Working Employees: ";
    for(int start = l ; start < r ; start ++){
        TimeRange cur_time = {start, start+1};
        if(working_cnt[cur_time] == min_cnt)
            cout << start << '-' << start + 1 << ' ';
    }
    cout << endl;
}

void Database::update_team_bonus(int team_id, int bonus) {
    if(!is_valid_percentage(bonus)){
        cout << "INVALID_ARGUMENTS" << endl;
    }   
    try{
        get_pointer_to_team(team_id)->update_bonus_percentage(bonus);
    }
    catch(exception& e){
        cout << "TEAM_NOT_FOUND" << endl;
        return;
    }
    cout << "OK" << endl;
}

void Database::add_working_hours(int id, int day, int l, int r){
    Employee* emp;
    try{
        emp = get_pointer_to_employee(id);
    }
    catch(exception &e){
        cout << "EMPLOYEE_NOT_FOUND" << endl;
        return;
    }
    if(is_invalid_time_range(l, r) or is_invalid_day(day)){
        cout << "INVALID_ARGUMENTS" << endl;
        return;
    }
    if(emp->is_busy(WorkingDateTime(day, {l, r}))) {
        cout << "INVALID_INTERVAL" << endl;
        return;
    }
    emp->add_working_date_time(WorkingDateTime(day, {l, r}));
    cout << "OK" << endl;
}

void Database::handle_hour_data(Dictionary data){
    int id = stoi(data["employee_id"]);
    int day = stoi(data["day"]);
    auto times = split(data["working_interval"], '-');
    auto new_day = WorkingDateTime(day, vector_to_pair(times));
    get_pointer_to_employee(id)->add_working_date_time(new_day);
}

void Database::report_salary(int id){
    try{
        get_pointer_to_employee(id)->print_detailed_salary_report(*this);
    }
    catch(exception& e){
        cout << "EMPLOYEE_NOT_FOUND" << endl;
    }
}

int Team::get_total_working_hours(Database db) {
    int total_working_hours = 0;
    vector <Employee> employees = db.get_employees();
    for (Employee employee : employees)
        total_working_hours += employee.get_total_working_hours();
    return total_working_hours;
}

void Database::report_team_salary(int team_id){
    try{
        get_pointer_to_team(team_id)->report_salary(*this);
    }
    catch(exception& e){
        cout << "TEAM_NOT_FOUND" << endl;
    }
}

void Database::find_teams_for_bonus(Database &db){
    int bonus_team_count = 0;
    for(auto team : teams)
        if(team.is_eligible_for_bonus(db))
            cout << "Team ID: " << team.get_id() << endl, bonus_team_count++;
    if(bonus_team_count == 0)
        cout << "NO_BONUS_TEAMS" << endl;
}

int Database::get_total_working_hours_of_employee(int id){
    return get_employee(id).calculate_total_working_hours();
}

double Database::calculate_avg(vector < int > vals){
    double sum = 0;
    int n = vals.size();
    for(int x : vals)
        sum += x;
    return sum/n;
}

double Database::calculate_variance(vector < int > vals){
    double avg = calculate_avg(vals);
    int n = vals.size();
    double ans = 0;
    for(int x : vals)
        ans += (x - avg) * (x - avg);
    return ans / n;
}   

double Team::calculate_variance(Database &db){
    vector < int > total_working_hours;
    for(int id : member_ids)
        total_working_hours.push_back(db.get_total_working_hours_of_employee(id));
    return db.calculate_variance(total_working_hours);
}

bool Team::is_eligible_for_bonus(Database &db){//TODO:test this because testdata is dogshit
    if(get_total_working_hours(db) <= bonus_min_working_hours)
        return 0;
    if(calculate_variance(db) >= bonus_working_hours_max_variance)
        return 0;
    return 1;
}