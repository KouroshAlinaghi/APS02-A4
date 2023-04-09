#include <string>
#include <vector>

using namespace std;

class WorkingDateTime;
class Database;
class Employee;
class SalaryConfig;
class Team;

enum ProficiencyLevel {
    JUNIOR,
    EXPERT,
    SENIOR,
    TEAM_LEAD
};

typedef pair<int, int> TimeRange;

class WorkingDateTime {
private:
    int day;
    TimeRange time_range;
public:
    int get_day() { return day; }
    int get_length() { return time_range.second - time_range.first; }
};

class SalaryConfig {
private:
    ProficiencyLevel level;
    int base_salary;
    int salary_per_hour;
    int salary_per_extra_hour;
    int official_working_hours;
    int tax_percentage;
public:
    int calculate(vector<WorkingDateTime> working_date_times) {
        int salary = base_salary, total_working_hours = 0;
        for (WorkingDateTime working_date_time : working_date_times) {
            total_working_hours += working_date_time.get_length();
            salary += working_date_time.get_length() * salary_per_hour;
        }
        if (total_working_hours > official_working_hours)
            salary += (total_working_hours - official_working_hours) * salary_per_extra_hour;
        
        return salary;
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

class Database {
private:
    vector<Employee> employees;
    vector<SalaryConfig> salary_configs;
    vector<Team> teams;
public:
    SalaryConfig get_salary_config(ProficiencyLevel level) {
        for (SalaryConfig config : salary_configs)
            if (config.get_level() == level)
                return config;
    }

    Employee get_employee(int id);
    Team get_team(int id);
    vector<Employee> get_employees();
    int get_total_working_hours_of_day(int day);
    
};

class Employee {
private:
    int id;
    string name;
    int age;
    ProficiencyLevel level;

    vector<WorkingDateTime> working_date_times;

    int team_id;

    int salary;
    int total_earning;
    
    int calculate_salary(Database db) {
        return db.get_salary_config(level).calculate(working_date_times);
    }
public:
    Employee( int id, string name,int age, ProficiencyLevel level, Database db) {
        id = id;
        name = name;
        age = age;
        level = level;
        team_id = -1;
        salary = calculate_salary(db);
        total_earning = db.get_salary_config(level).apply_tax(salary);
    }
    bool has_team() { return team_id != -1; }
    void delete_working_hours(int day) {
        for (int i = 0; i < working_date_times.size(); i++)
            if (working_date_times[i].get_day() == day)
                working_date_times.erase(working_date_times.begin() + i);
    }
    void add_working_date_time(WorkingDateTime working_date_time) {
        working_date_times.push_back(working_date_time);
    }
    void join_to_team(Team team);
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
    int get_number_of_absent_days() { return 30 - working_date_times.size(); }
};

class Team {
private:
    int id;
    int team_head_id;

    int bonus_percentage;
    int bonus_min_working_hours;
    double bonus_working_hours_max_variance;
public:
    Team(int id, int team_head_id) {
        id = id;
        team_head_id = team_head_id;
        bonus_percentage = 0;
    }
    int get_id() { return id; }
    int get_team_head_id() { return team_head_id; }
    string get_head_member_name(Database db) { return db.get_employee(team_head_id).get_name(); }
    vector<Employee> get_employees(Database db) {
        vector<Employee> members, employees = db.get_employees();
        for (Employee employee : employees)
            if (employee.get_team_id() == id)
                members.push_back(employee);
        return members;
    }
    int get_number_of_members(Database db) { return get_employees(db).size(); }
    int get_total_working_hours(Database db) {
        int total_working_hours = 0;
        vector<Employee> employees = db.get_employees();
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
}
Team Database::get_team(int id) {
    for (Team team : teams)
        if (team.get_id() == id)
            return team;
}
int Database::get_total_working_hours_of_day(int day) {
    int total_hours = 0;
    for (Employee employee : employees)
        if (employee.does_work_on_day(day))
            total_hours += employee.get_total_working_hours();
    return total_hours;
}
vector<Employee> Database::get_employees() { return employees; }
void Employee::join_to_team(Team team) { team_id = team.get_id(); }

int main() {
    return 0;
}
