#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <utility>
#include <cmath>
#include <iomanip>

#define all(x) x.begin(), x.end()

#define FILE_SIZE 4
#define DAYS_IN_MONTH 30
#define HOURS_IN_DAY 24
#define DIRECTORY_DELIM "/"
#define CSV_DELIM ','
#define MEMBER_ID_DELIM '$'
#define WORK_INTERVAL_DELIM '-'
#define EXPERTISE_SIZE 4
#define EXPERTISE_COLUMN_SIZE 6
#define EMPLOYEE_COLUMN_SIZE 4
#define TEAM_COLUMN_SIZE 5
#define WORKING_HOURS_COLUMN_SIZE 3
#define SKIPPED_LINE_CNT 1
#define CMD_SIZE 11
#define PERCENTAGE_AMOUNT 100.0
#define LINE_SEPERATOR "---"
#define NO_UPDATE_STR "-"
#define NA -1
#define INF 1e9
#define NA_STR "N/A"
#define OK "OK"
#define ERROR "ERROR"

enum FILE_NAME {SALARY_CONFIG_FILE, EMPLOYEE_FILE, TEAMS_FILE, WORKING_HOURS_FILE};
enum LEVEL {JUNIOR, EXPERT, SENIOR, TEAMLEAD};
enum EMPLOYEE_COLUMN {EMP_ID, EMP_NAME, EMP_AGE, EMP_LEVEL};
enum EXPERTISE_COLUMN {EXP_LEVEL, EXP_BASE_SALARY, EXP_SALARY_PER_HOUR, EXP_SALARY_PER_EXTRA_HOUR, EXP_OFFICIAL_WORKING_HOURS, EXP_TAX_PERCENTAGE};
enum TEAM_COLUMN {TEAM_ID, TEAM_HEAD_ID, TEAM_MEMBER_IDS, TEAM_BONUS_MIN_WORKING_HOURS, TEAM_BONUS_WORKING_HOURS_MAX_VARIANCE};
enum WORKIN_HOURS_COLUMN {WORK_H_EMPLOYEE_ID, WORK_H_DAY, WORK_H_WORKING_INTERVAL};
enum CMD_INSTANCE {REPORT_SALARIES, REPORT_EMPLOYEE_SALARY, REPORT_TEAM_SALARY, REPORT_TOTAL_HOURS_PER_DAY, REPORT_EMPLOYEE_PER_HOUR, SHOW_SALARY_CONFIG, UPDATE_SALARY_CONFIG, ADD_WORKING_HOURS, DELETE_WORKING_HOURS, UPDATE_TEAM_BONUS, FIND_TEAMS_FOR_BONUS};

using namespace std;
const string FILE_NAMES[] = {"salary_configs.csv" ,"employees.csv", "teams.csv", "working_hours.csv"};
const string LEVEL_NAMES[] = {"junior", "expert", "senior", "team_lead"};
const string CMD_NAMES[] = {"report_salaries", "report_employee_salary", "report_team_salary", "report_total_hours_per_day", "report_employee_per_hour", "show_salary_config", "update_salary_config", "add_working_hours", "delete_working_hours", "update_team_bonus", "find_teams_for_bonus"};

double roundOneDigit(double n){
    n = n*10;
    n = round(n);
    n /= 10;
    return n;
}

class Expertise{
    public:
        Expertise();
        Expertise(LEVEL _level, int _baseSalary, int _salaryPerHour, int _SalaryPerExtraHour, int _officialWorkingHours, int _taxPercentage);
        string getLevelName() { return LEVEL_NAMES[level]; };
        int calculateTax(int salaryBonus);
        int calculateEarning(int salaryBonus);
        int calculateSalary(int hours);
        string showConfig();
        void setBaseSalary(int _baseSalary) { baseSalary = _baseSalary; };
        void setSalaryPerHour(int _salaryPerHour) { salaryPerHour = _salaryPerHour; };
        void setSalaryPerExtraHour(int _salaryPerExtraHour) {salaryPerExtraHour = _salaryPerExtraHour; };
        void setOfficialWorkingHours(int _officialWorkingHours) { officialWorkingHours = _officialWorkingHours; };
        void setTaxPercentage(int _taxPercentage) { taxPercentage = _taxPercentage; };
    private:
        LEVEL level;
        int baseSalary;
        int salaryPerHour;
        int salaryPerExtraHour;
        int officialWorkingHours;
        int taxPercentage;
};

Expertise::Expertise(){}

Expertise::Expertise(LEVEL _level, int _baseSalary, int _salaryPerHour, int _salaryPerExtraHour, int _officialWorkingHours, int _taxPercentage){
    level = _level;
    baseSalary = _baseSalary;
    salaryPerHour = _salaryPerHour;
    salaryPerExtraHour = _salaryPerExtraHour;
    officialWorkingHours = _officialWorkingHours;
    taxPercentage = _taxPercentage;
}

int Expertise::calculateTax(int salaryBonus){
    double tax =  (taxPercentage * salaryBonus) / PERCENTAGE_AMOUNT;
    return round(tax);
}

int Expertise::calculateEarning(int salaryBonus){
    return salaryBonus - calculateTax(salaryBonus);
}

int Expertise::calculateSalary(int hours){
    int salary = baseSalary;
    salary += salaryPerHour * hours;
    salary += max(0, hours - officialWorkingHours) * (salaryPerExtraHour - salaryPerHour);
    return salary;
}

string Expertise::showConfig(){
    ostringstream output;
    output << "Base Salary: " << baseSalary << endl;
    output << "Salary Per Hour: " << salaryPerHour << endl;
    output << "Salary Per Extra Hour: " << salaryPerExtraHour << endl;
    output << "Official Working Hours: " << officialWorkingHours << endl;
    output << "Tax: " << taxPercentage << '%' << endl;
    return output.str();
}

class Employee{
    public:
        Employee(int _id, string _name, int _age, Expertise* _expertise);
        int getId() { return id; }
        void updateWorkingHours(int day, int intervalStart, int intervalFinish);
        void updateTeamBonusPercentage(int newTeamBonusPercentage);
        int getTotalEarning();
        string getBriefInfo();
        string getTeamIdStr();
        string getFullInfo();
        string outputAsTeamHead();
        string outputAsTeamMember();
        void setTeamId(int _id) { teamId = _id; };
        int getAbsentDays();
        int totalWorkingHours();
        int hoursInDay(int day);
        int getCountPresentInHour(int hour);
        string addWorkHours(int day, int stHour, int endHour);
        void deleteWorkDay(int day);
    private:
        int id;
        string name;
        int age;
        Expertise *expertise;
        bool isWorking[DAYS_IN_MONTH][HOURS_IN_DAY];
        int teamId;
        int teamBonusPercentage;
        int getRawSalary();
        int getBonusSalary();
        int getTax();
};

Employee::Employee(int _id, string _name, int _age, Expertise* _expertise){
    id = _id;
    name = _name;
    age = _age;
    expertise = _expertise;
    for(int i = 0; i < DAYS_IN_MONTH; i++){
        for(int j = 0; j < HOURS_IN_DAY; j++){
            isWorking[i][j] = false;
        }
    }
    teamBonusPercentage = 0;
    teamId = NA;
}

void Employee::updateTeamBonusPercentage(int newTeamBonusPercentage){
    teamBonusPercentage = newTeamBonusPercentage;
}

int Employee::hoursInDay(int day){
    int hours = 0;
    for(int i = 0; i < HOURS_IN_DAY; i++){
        hours += isWorking[day][i];
    }
    return hours;
}

int Employee::getCountPresentInHour(int hour){
    int count = 0;
    for (int i = 0; i < DAYS_IN_MONTH; ++i)
        count += isWorking[i][hour];
    return count;
}

int Employee::totalWorkingHours(){
    int total = 0;
    for(int i = 0; i < DAYS_IN_MONTH; i++){
        for(int j = 0; j < HOURS_IN_DAY; j++){
            total += isWorking[i][j];
        }
    }
    return total;
}

int Employee::getRawSalary(){
    return expertise -> calculateSalary(totalWorkingHours());
}

int Employee::getBonusSalary(){
    return (int)round((getRawSalary() * teamBonusPercentage) / PERCENTAGE_AMOUNT);
}

int Employee::getTax(){
    return expertise -> calculateTax(getRawSalary() + getBonusSalary());
}

int Employee::getTotalEarning(){
    int rawSalary = getRawSalary();
    int bonusSalary = getBonusSalary();
    int tax = getTax();
    int totalEarning = rawSalary + bonusSalary - tax;
    return totalEarning;
}

int Employee::getAbsentDays(){
    int presentDays = 0;
    for(int i = 0; i < DAYS_IN_MONTH; i++){
        for(int j = 0; j < HOURS_IN_DAY; j++){
            if(isWorking[i][j]){
                presentDays++;
                break;
            }
        }
    }
    return DAYS_IN_MONTH - presentDays;
}

void Employee::updateWorkingHours(int day, int intervalStart, int intervalFinish){
    for(int i = intervalStart; i < intervalFinish; i++){
        isWorking[day][i] = true;
    }
}

string Employee::getBriefInfo(){
    ostringstream output;
    output << "ID: " << id << endl;
    output << "Name: " << name << endl;
    output << "Total Working Hours: " << totalWorkingHours() << endl;
    output << "Total Earning: " << getTotalEarning() << endl;
    return output.str();
}

string Employee::getTeamIdStr(){
    if(teamId == NA){
        return NA_STR;
    }
    return to_string(teamId);
}

string Employee::getFullInfo(){
    ostringstream output;

    output << "ID: " << id << endl;
    output << "Name: " << name << endl;
    output << "Age: " << age << endl;
    output << "Level: " << expertise -> getLevelName() << endl;
    output << "Team ID: " << getTeamIdStr() << endl;
    output << "Total Working Hours: " << totalWorkingHours() << endl;
    output << "Absent Days: " << getAbsentDays() << endl;
    output << "Salary: " << getRawSalary() << endl;
    output << "Bonus: " << getBonusSalary() << endl;
    output << "Tax: " <<  getTax() << endl;
    output << "Total Earning: " << getTotalEarning() << endl;
    return output.str();
}

string Employee::outputAsTeamHead(){
    ostringstream output;
    output << "Head ID: " << id << endl;
    output << "Head Name: " << name << endl;
    return output.str();
}

string Employee::outputAsTeamMember(){
    ostringstream output;
    output << "Member ID: " << id << endl;
    output << "Total Earning: " << getTotalEarning() << endl;
    return output.str();
}

string Employee::addWorkHours(int day, int stHour, int endHour){
    for (int i = stHour; i <= endHour; ++i){
        if (isWorking[day][i])
            return ERROR;
    }
    for (int i = stHour; i <= endHour; ++i)
        isWorking[day][i] = true;
    return OK;
}

void Employee::deleteWorkDay(int day){
    for (int i = 0; i < HOURS_IN_DAY; ++i)
        isWorking[day][i] = false;
}

Employee* findEmployeeById(int id, const vector<Employee*> &employees, const set< pair<int, int> > &employeeInd){
    set< pair<int, int> >::iterator indPair = lower_bound(all(employeeInd), make_pair(id, 0));
    if(indPair == employeeInd.end()){
        return NULL;
    }
    if(indPair -> first == id){
        return employees[indPair -> second];
    }
    return NULL;
}

class Team{
    public:
        Team(int _id, Employee* _head, vector<Employee*> _members, int _bonusMinWorkingHours, double _bonusWorkingHoursMaxVariance);
        int getId();
        string getInfo();
        void updateBonusPercentage(int newBonusPercentage);
        bool isGoodForBonus();
        int workingHourSum();
    private:
        int id;
        Employee* head;
        vector<Employee*> members;
        set< pair<int, int> > memberInd;
        int bonusMinWorkingHours;
        double bonusWorkingHoursMaxVariance;
        int bonus;
        double workingHourAverage();
};

Team::Team(int _id, Employee* _head, vector<Employee*> _members, int _bonusMinWorkingHours, double _bonusWorkingHoursMaxVariance){
    id = _id;
    head = _head;
    members = _members;
    for(int i = 0; i < (int)members.size(); i++){
        memberInd.insert(make_pair(members[i] -> getId(), i));
        members[i] -> setTeamId(id);
    }
    bonusMinWorkingHours = _bonusMinWorkingHours;
    bonusWorkingHoursMaxVariance = _bonusWorkingHoursMaxVariance;
    bonus = 0;
}

int Team::getId(){
    return id;
}

int Team::workingHourSum(){
    int sum = 0;
    for(int i = 0; i < (int)members.size(); i++){
        sum += members[i] -> totalWorkingHours();
    }
    return sum;
}

double Team::workingHourAverage(){
    double avg = workingHourSum();
    avg /= (int)members.size();
    avg = roundOneDigit(avg);
    return avg;
}

string Team::getInfo(){
    ostringstream output;
    output << "ID: " << id << endl;
    output << head -> outputAsTeamHead();
    output << "Team Total Working Hours: " << workingHourSum() << endl;
    output << "Average Member Working Hours: " << fixed << setprecision(1) << workingHourAverage() << endl;
    output << "Bonus: " << bonus << endl;
    output << LINE_SEPERATOR << endl;
    for(set< pair<int, int> >::iterator i = memberInd.begin(); i != memberInd.end(); ++i){
        output << members[i -> second] -> outputAsTeamMember() << LINE_SEPERATOR << endl;
    }
    return output.str();
}

void Team::updateBonusPercentage(int newBonusPercentage){
    bonus = newBonusPercentage;
    for (Employee* emp : members){
        emp -> updateTeamBonusPercentage(newBonusPercentage);
    }
}

bool Team::isGoodForBonus(){
    vector <int> workHours;
    int memCount = (int)members.size();
    for (Employee* emp : members){
        workHours.push_back(emp -> totalWorkingHours());
    }
    int totalHours = 0;
    for (int i : workHours)
        totalHours += i;
    double avg = (double)totalHours/memCount;

    double variance = 0;
    for (int i : workHours)
        variance += (i-avg)*(i-avg);
    variance /= (int)workHours.size();
    if (totalHours > bonusMinWorkingHours && variance < bonusWorkingHoursMaxVariance)
        return true;
    return false;
}

int getExpertiseToken(string tokenStr){
    int token = NA;
    if(tokenStr == LEVEL_NAMES[JUNIOR]){
        token = JUNIOR;
    }
    if(tokenStr == LEVEL_NAMES[EXPERT]){
        token = EXPERT;
    }
    if(tokenStr == LEVEL_NAMES[SENIOR]){
        token = SENIOR;
    }
    if(tokenStr == LEVEL_NAMES[TEAMLEAD]){
        token = TEAMLEAD;
    }
    return token;
}

string trimBoth(string str){
    int stPos = 0, fnPos = (int)str.size();
    for(int i = 0; i < (int)str.size(); i++){
        if(!isspace(str[i])){
            stPos = i;
            break;
        }
    }
    for(int i = (int)str.size() - 1; ~i; i--){
        if(!isspace(str[i])){
            fnPos = i + 1;
            break;
        }
    }
    return str.substr(stPos, fnPos - stPos);
}

string wordFromPosition(string::iterator &it, string::iterator fn) {
    while (isspace(*(it++)))
        ;
    --it;
    string::iterator i = it;
    string res = "";
    for (; i != fn; ++i) {
        if (isspace(*i)) {
            break;
        }
        res.push_back(*i);
    }
    swap(i, it);
    return res;
}

vector<string> splitIntoWords(string str){
    vector<string> elements;
    string::iterator it = str.begin();
    while(it != str.end()){
        elements.push_back(wordFromPosition(it, str.end()));
    }
    return elements;
}

vector<string> splitString(string str, char delim){
     vector<string> elements;
     istringstream strStream(str);
     string element;

     while(getline(strStream, element, delim)){
         elements.push_back(trimBoth(element));
     }

     return elements;
}

class PedarSahab{
    public:
        PedarSahab();
        Expertise *getExpertiseByInd(int ind);
        void addEmployee(string id, string name, string age, string expertise);
        void updateExpertise(string level, string baseSalary, string salaryPerHour, string salaryPerExtraHour, string officialWorkingHours, string taxPercentage);
        void addTeam(string teamId, string teamHeadId, string memberIds, string bonusMinWorkingHours, string bonusWorkingHoursMaxVariance);
        void updateEmployeeWorkingDay(string id, string day, string workInterval);
        Team* findTeamById(int id);
        string reportSalaries();
        string reportEmployeeSalary(string idStr);
        string reportTeamSalary(string idStr);
        string reportTotalHoursPerDay(string stDaystr, string fnDaystr);
        string reportEmployeePerHour(string stHour, string endHour);
        string showSalaryConfig(string level);
        string updateSalaryConfig(string level, string newBaseSalaryStr, string newSalaryPerHourStr, string newSalaryPerExtraHourStr, string newOfficialWorkingHoursStr, string newTaxPercentageStr);
        string addWorkingHours(string idStr, string dayStr, string stHourStr, string endHourStr);
        string deleteWorkingHours(string idStr, string dayStr);
        string updateTeamBonus(string teamIdStr, string newBonusPercentageStr);
        string findTeamsForBonus();
        void freeMemory();
    private:
        vector<Employee*> employees;
        set< pair<int, int> > employeeInd;
        vector<Team*> teams;
        set< pair<int, int> > teamInd;
        Expertise expertise[EXPERTISE_SIZE];
        int workingHoursInDay(int day);
        vector<int> maxWorkingHours(int st, int fn);
        vector<int> minWorkingHours(int st, int fn);
        int getCountWorkingInHour(int hour);
        vector<int> getMaxWorkingHoursInMonth(int sthour, int endHour);
        vector<int> getMinWorkingHoursInMonth(int sthour, int endHour);
};

PedarSahab::PedarSahab(){}

Expertise *PedarSahab::getExpertiseByInd(int ind){
    return &(expertise[ind]);
}

int PedarSahab::workingHoursInDay(int day){
    int sum = 0;
    for(int i = 0; i < (int)employees.size(); i++){
        sum += employees[i] -> hoursInDay(day);
    }
    return sum;
}

vector<int> PedarSahab::maxWorkingHours(int st, int fn){
    int mx = 0;
    vector<int> res;
    for(int i = st; i <= fn; i++){
        int hours = workingHoursInDay(i);
        if(hours == mx){
            res.push_back(i);
        }
        if(hours > mx){
            res.clear();
            res.push_back(i);
            mx = hours;
        }
    }
    return res;
}

vector<int> PedarSahab::minWorkingHours(int st, int fn){
    int mn = INF;
    vector<int> res;
    for(int i = st; i <= fn; i++){
        int hours = workingHoursInDay(i);
        if(hours == mn){
            res.push_back(i);
        }
        if(hours < mn){
            res.clear();
            res.push_back(i);
            mn = hours;
        }
    }
    return res;
}

int PedarSahab::getCountWorkingInHour(int hour){
    int sum = 0;
    for (int i = 0; i < (int)employees.size(); ++i){
        sum += employees[i] -> getCountPresentInHour(hour);
    }
    return sum;
}

vector <int> PedarSahab::getMaxWorkingHoursInMonth(int stHour, int endHour){
    vector <int> res;
    double mx = 0;
    for (int i = stHour; i <= endHour; ++i){
        double count = roundOneDigit((double)getCountWorkingInHour(i) / DAYS_IN_MONTH);
        if (count == mx)
            res.push_back(i);
        if (count > mx){
            mx = count;
            res.clear();
            res.push_back(i);
        }
    }
    return res;
}

vector <int> PedarSahab::getMinWorkingHoursInMonth(int stHour, int endHour){
    vector <int> res;
    double mn = INF;
    for (int i = stHour; i <= endHour; ++i){
        double count = roundOneDigit((double)getCountWorkingInHour(i) / DAYS_IN_MONTH);
        if (count == mn)
            res.push_back(i);
        if (count < mn){
            mn = count;
            res.clear();
            res.push_back(i);
        }
    }
    return res;
}

void PedarSahab::addEmployee(string id, string name, string age, string expertise){
    employees.push_back(new Employee(stoi(id), name, stoi(age), getExpertiseByInd(getExpertiseToken(expertise))));
    employeeInd.insert(make_pair(stoi(id), (int)employees.size() - 1));
}

void PedarSahab::updateExpertise(string level, string baseSalary, string salaryPerHour, string salaryPerExtraHour, string officialWorkingHours, string taxPercentage){
    expertise[getExpertiseToken(level)] = Expertise((LEVEL)getExpertiseToken(level), stoi(baseSalary), stoi(salaryPerHour), stoi(salaryPerExtraHour), stoi(officialWorkingHours), stoi(taxPercentage));
}

void PedarSahab::addTeam(string teamId, string teamHeadId, string memberIds, string bonusMinWorkingHours, string bonusWorkingHoursMaxVariance){
    int id = stoi(teamId);
    Employee* head = findEmployeeById(stoi(teamHeadId), employees, employeeInd);
    vector<Employee*> members;
    vector<string> memberIdList = splitString(memberIds, MEMBER_ID_DELIM);
    for(int i = 0; i < (int)memberIdList.size(); i++){
        members.push_back(findEmployeeById(stoi(memberIdList[i]), employees, employeeInd));
    }
    teams.push_back(new Team(id, head, members, stoi(bonusMinWorkingHours), stof(bonusWorkingHoursMaxVariance)));
    teamInd.insert(make_pair(id, (int)teams.size() - 1));
}

void PedarSahab::updateEmployeeWorkingDay(string id, string day, string workInterval){
    Employee* employee = findEmployeeById(stoi(id), employees, employeeInd);
    vector<string> intervalStr = splitString(workInterval, WORK_INTERVAL_DELIM);
    employee -> updateWorkingHours(stoi(day) - 1, stoi(intervalStr[0]), stoi(intervalStr[1]));
}

Team* PedarSahab::findTeamById(int id){
    set< pair<int, int> >::iterator indPair = lower_bound(all(teamInd), make_pair(id, 0));
    if(indPair == teamInd.end()){
        return NULL;
    }
    if(indPair -> first == id){
        return teams[indPair -> second];
    }
    return NULL;
}

string PedarSahab::reportSalaries(){
    ostringstream output;
    for(set< pair<int, int> >::iterator i = employeeInd.begin(); i != employeeInd.end(); ++i){
        output << employees[i -> second] -> getBriefInfo() << LINE_SEPERATOR << endl;
    }
    return output.str();
}

string PedarSahab::reportEmployeeSalary(string idStr){
    ostringstream output;
    Employee* emp = findEmployeeById(stoi(idStr), employees, employeeInd);
    if(emp == NULL){
        output << "EMPLOYEE_NOT_FOUND" << endl;
        return output.str();
    }
    output << emp -> getFullInfo();
    return output.str();
}

string PedarSahab::reportTeamSalary(string idStr){
    ostringstream output;
    Team* teamPtr = findTeamById(stoi(idStr));
    if(teamPtr == NULL){
        output << "TEAM_NOT_FOUND" << endl;
        return output.str();
    }
    output << teamPtr -> getInfo();
    return output.str();
}

string PedarSahab::reportTotalHoursPerDay(string stDaystr, string fnDaystr){
    ostringstream output;
    int stDay = stoi(stDaystr) - 1;
    int fnDay = stoi(fnDaystr) - 1;
    if(stDay < 0 || stDay >= DAYS_IN_MONTH || fnDay < 0 || fnDay >= DAYS_IN_MONTH || fnDay < stDay){
        output << "INVALID_ARGUMENTS" << endl;
        return output.str();
    }
    for(int i = stDay; i <= fnDay; i++){
        output << "Day #" << (i + 1) << ": " << workingHoursInDay(i) << endl;
    }
    output << LINE_SEPERATOR << endl;
    output << "Day(s) with Max Working Hours: " ;
    vector<int> mx = maxWorkingHours(stDay, fnDay);
    for(int i = 0; i < (int)mx.size(); i++){
        output << mx[i] + 1 << " ";
    }
    output << endl;
    output << "Day(s) with Min Working Hours: " ;
    vector<int> mn = minWorkingHours(stDay, fnDay);
    for(int i = 0; i < (int)mn.size(); i++){
        output << mn[i] + 1 << " ";
    }
    output << endl;
    return output.str();
}

string PedarSahab::reportEmployeePerHour(string stHourStr, string endHourStr){
    ostringstream output;
    int stHour = stoi(stHourStr);
    int endHour = stoi(endHourStr) - 1;
    if (stHour < 0 || endHour >= HOURS_IN_DAY || stHour > endHour){
        output << "INVALID_ARGUMENTS" << endl;
        return output.str();
    }

    for (int hour = stHour; hour <= endHour; ++hour)
        output << hour << "-" << hour+1 << ": " << fixed << setprecision(1) << roundOneDigit((double)getCountWorkingInHour(hour)/DAYS_IN_MONTH) << endl;
    
    output << LINE_SEPERATOR << endl;

    vector<int> mx = getMaxWorkingHoursInMonth(stHour, endHour);
    output << "Period(s) with Max Working Employees: ";
    for (int i = 0; i < (int)mx.size(); ++i)
        output << mx[i] << '-' << mx[i]+1 << ' ';
    output << endl;
    vector<int> mn = getMinWorkingHoursInMonth(stHour, endHour);
    output << "Period(s) with Min Working Employees: ";
    for (int i = 0; i < (int)mn.size(); ++i)
        output << mn[i] << '-' << mn[i]+1 << ' ';
    output << endl;
    return output.str();
}

string PedarSahab::showSalaryConfig(string level){
    ostringstream output;
    if (getExpertiseToken(level) == NA){
        output << "INVALID_LEVEL" << endl;
        return output.str();
    }
    output << expertise[getExpertiseToken(level)].showConfig();
    return output.str();
}

string PedarSahab::updateSalaryConfig(string level, string newBaseSalaryStr, string newSalaryPerHourStr, string newSalaryPerExtraHourStr, string newOfficialWorkingHoursStr, string newTaxPercentageStr){
    ostringstream output;
    int ind = getExpertiseToken(level);

    if (ind == NA){
        output << "INVALID_LEVEL" << endl;
        return output.str();
    }

    if(newBaseSalaryStr != NO_UPDATE_STR){
        expertise[ind].setBaseSalary(stoi(newBaseSalaryStr));
    }
    if(newSalaryPerHourStr != NO_UPDATE_STR){
        expertise[ind].setSalaryPerHour(stoi(newSalaryPerHourStr));
    }
    if(newSalaryPerExtraHourStr != NO_UPDATE_STR){
        expertise[ind].setSalaryPerExtraHour(stoi(newSalaryPerExtraHourStr));
    }
    if(newOfficialWorkingHoursStr != NO_UPDATE_STR){
        expertise[ind].setOfficialWorkingHours(stoi(newOfficialWorkingHoursStr));
    }
    if(newTaxPercentageStr != NO_UPDATE_STR){
        expertise[ind].setTaxPercentage(stoi(newTaxPercentageStr));
    }
    output << "OK" << endl;
    return output.str();
}

string PedarSahab::addWorkingHours(string idStr, string dayStr, string stHourStr, string endHourStr){
    ostringstream output;
    int id = stoi(idStr);
    int day = stoi(dayStr) - 1;
    int stHour = stoi(stHourStr);
    int endHour = stoi(endHourStr)-1;
    Employee* emp = findEmployeeById(id, employees, employeeInd);
    if(emp == NULL){
        output << "EMPLOYEE_NOT_FOUND" << endl;
        return output.str();
    }
    if(stHour < 0 || endHour >= HOURS_IN_DAY || stHour > endHour || day < 0 || day >= DAYS_IN_MONTH){
        output << "INVALID_ARGUMENTS" << endl;
        return output.str();
    }
    if (emp->addWorkHours(day, stHour, endHour) == ERROR){
        output << "INVALID_INTERVAL" << endl;
        return output.str();
    }
    output << "OK" << endl;
    return output.str();
}

string PedarSahab::deleteWorkingHours(string idStr, string dayStr){
    ostringstream output;
    int id = stoi(idStr);
    int day = stoi(dayStr) - 1;
    Employee* emp = findEmployeeById(id, employees, employeeInd);
    if(emp == NULL){
        output << "EMPLOYEE_NOT_FOUND" << endl;
        return output.str();
    }
    if(day < 0 || day >= DAYS_IN_MONTH){
        output << "INVALID_ARGUMENTS" << endl;
        return output.str();
    }
    emp -> deleteWorkDay(day);
    output << "OK" << endl;
    return output.str();
}

string PedarSahab::updateTeamBonus(string teamIdStr, string newBonusPercentageStr){
    ostringstream output;
    int teamId = stoi(teamIdStr);
    int newBonusPercentage = stoi(newBonusPercentageStr);
    Team* teamPtr = findTeamById(teamId);
    if(teamPtr == NULL){
        output << "TEAM_NOT_FOUND" << endl;
        return output.str();
    }
    if(newBonusPercentage < 0 || newBonusPercentage > PERCENTAGE_AMOUNT){
        output << "INVALID_ARGUMENTS" << endl;
        return output.str();
    }
    teamPtr -> updateBonusPercentage(newBonusPercentage);
    output << "OK" << endl;
    return output.str();
}

string PedarSahab::findTeamsForBonus(){
    ostringstream output;
    int teamCount = 0;
    vector< pair < pair<int, int> , int > > vec;

    for(int i = 0; i < (int)teams.size(); i++){
        vec.push_back({make_pair(teams[i] -> workingHourSum(), teams[i]->getId()), i});
    }

    sort(all(vec));
    reverse(all(vec));

    for (int i = 0; i < (int)vec.size(); i++){
        if (teams[vec[i].second] -> isGoodForBonus()){
            output << "Team ID: " << teams[vec[i].second] -> getId() << endl;
            teamCount++;
        }
    }
    if(!teamCount){
        output << "NO_BONUS_TEAMS" << endl;
    }
    return output.str();
}

void PedarSahab::freeMemory(){
    for(int i = 0; i < (int)employees.size(); i++){
        delete employees[i];
    }
    for(int i = 0; i < (int)teams.size(); i++){
        delete teams[i];
    }
}

vector<vector<string>> readCSV(string fileName){
    ifstream inputFile(fileName);
    vector<vector<string>> readFile;
    string lineOfFile;

    while(getline(inputFile, lineOfFile)){
        readFile.push_back(splitString(lineOfFile, CSV_DELIM));
    }

    inputFile.close();
    return readFile;
}

void readEmployees(vector<vector<string>> employeeReadFile, PedarSahab &pedarSahab){
    for(int i = SKIPPED_LINE_CNT; i < (int)employeeReadFile.size(); i++){
        string id;
        string name;
        string age;
        string expertise;
        for (int j = 0; j < EMPLOYEE_COLUMN_SIZE; j++) {
            if(j == EMP_ID){
                id = employeeReadFile[i][j];
            }
            if(j == EMP_NAME){
                name = employeeReadFile[i][j];
            }
            if(j == EMP_AGE){
                age = employeeReadFile[i][j];
            }
            if(j == EMP_LEVEL){
                expertise = employeeReadFile[i][j];
            }
        }
        pedarSahab.addEmployee(id, name, age, expertise);
    }
}

void readExpertise(vector<vector<string>> salaryConfigsReadFile, PedarSahab &pedarSahab){
    for(int i = SKIPPED_LINE_CNT; i < (int)salaryConfigsReadFile.size(); i++){
        string level;
        string baseSalary;
        string salaryPerHour;
        string salaryPerExtraHour;
        string officialWorkingHours;
        string taxPercentage;
        for(int j = 0; j < EXPERTISE_COLUMN_SIZE; j++){
            if(j == EXP_LEVEL){
                level = salaryConfigsReadFile[i][j];
            }
            if(j == EXP_BASE_SALARY){
                baseSalary = salaryConfigsReadFile[i][j];
            }
            if(j == EXP_SALARY_PER_HOUR){
                salaryPerHour = salaryConfigsReadFile[i][j];
            }
            if(j == EXP_SALARY_PER_EXTRA_HOUR){
                salaryPerExtraHour = salaryConfigsReadFile[i][j];
            }
            if(j == EXP_OFFICIAL_WORKING_HOURS){
                officialWorkingHours = salaryConfigsReadFile[i][j];
            }
            if(j == EXP_TAX_PERCENTAGE){
                taxPercentage = salaryConfigsReadFile[i][j];
            }
        }
        pedarSahab.updateExpertise(level, baseSalary, salaryPerHour, salaryPerExtraHour, officialWorkingHours, taxPercentage);
    }
}

void readTeams(vector<vector<string>> teamsReadFile, PedarSahab &pedarSahab){
    for(int i = SKIPPED_LINE_CNT; i < (int)teamsReadFile.size(); i++){
        string id;
        string headId;
        string memberIds;
        string bonusMinWorkingHours;
        string BonusWorkingHoursMaxVariance;
        for(int j = 0; j < TEAM_COLUMN_SIZE; j++){
            if(j == TEAM_ID){
                id = teamsReadFile[i][j];
            }
            if(j == TEAM_HEAD_ID){
                headId = teamsReadFile[i][j];
            }
            if(j == TEAM_MEMBER_IDS){
                memberIds = teamsReadFile[i][j];
            }
            if(j == TEAM_BONUS_MIN_WORKING_HOURS){
                bonusMinWorkingHours = teamsReadFile[i][j];
            }
            if(j == TEAM_BONUS_WORKING_HOURS_MAX_VARIANCE){
                BonusWorkingHoursMaxVariance = teamsReadFile[i][j];
            }
        }
        pedarSahab.addTeam(id, headId, memberIds, bonusMinWorkingHours, BonusWorkingHoursMaxVariance);
    }
}

void readWorkingHours(vector<vector<string>> workingHoursReadFile, PedarSahab &pedarSahab){
    for(int i = SKIPPED_LINE_CNT; i < (int)workingHoursReadFile.size(); i++){
        string id;
        string day;
        string workingInterval;
        for(int j = 0; j < WORKING_HOURS_COLUMN_SIZE; j++){
            if(j == WORK_H_EMPLOYEE_ID){
                id = workingHoursReadFile[i][j];
            }
            if(j == WORK_H_DAY){
                day = workingHoursReadFile[i][j];
            }
            if(j == WORK_H_WORKING_INTERVAL){
                workingInterval = workingHoursReadFile[i][j];
            }
        }
        pedarSahab.updateEmployeeWorkingDay(id, day, workingInterval);
    }
}

void readInput(string assetsFolder, PedarSahab &pedarSahab){
    for(int i = 0; i < FILE_SIZE; i++){
        if(i == EMPLOYEE_FILE){
            readEmployees(readCSV(assetsFolder + FILE_NAMES[i]), pedarSahab);
        }
        if(i == SALARY_CONFIG_FILE){
            readExpertise(readCSV(assetsFolder + FILE_NAMES[i]), pedarSahab);
        }
        if (i == TEAMS_FILE){
            readTeams(readCSV(assetsFolder + FILE_NAMES[i]), pedarSahab);
        }
        if (i == WORKING_HOURS_FILE){
            readWorkingHours(readCSV(assetsFolder + FILE_NAMES[i]), pedarSahab);
        }
    }
}

void handleCommand(string cmdLine, PedarSahab &pedarSahab){
    vector<string> cmdWords = splitIntoWords(cmdLine);
    if(cmdWords.empty()){
        return;
    }
    int cmdInd = CMD_SIZE;
    for(int i = 0; i < CMD_SIZE; i++){
        if(cmdWords[0] == CMD_NAMES[i]){
            cmdInd = i;
            break;
        }
    }

    if(cmdInd == REPORT_SALARIES){
        cout << pedarSahab.reportSalaries();
    }
    if(cmdInd == REPORT_EMPLOYEE_SALARY){
        cout << pedarSahab.reportEmployeeSalary(cmdWords[1]);
    }
    if(cmdInd == REPORT_TEAM_SALARY){
        cout << pedarSahab.reportTeamSalary(cmdWords[1]);
    }
    if(cmdInd == REPORT_TOTAL_HOURS_PER_DAY){
        cout << pedarSahab.reportTotalHoursPerDay(cmdWords[1], cmdWords[2]);
    }
    if (cmdInd == REPORT_EMPLOYEE_PER_HOUR){
        cout << pedarSahab.reportEmployeePerHour(cmdWords[1], cmdWords[2]);
    }
    if (cmdInd == SHOW_SALARY_CONFIG){
        cout << pedarSahab.showSalaryConfig(cmdWords[1]);
    }
    if (cmdInd == UPDATE_SALARY_CONFIG){
        cout << pedarSahab.updateSalaryConfig(cmdWords[1], cmdWords[2], cmdWords[3], cmdWords[4], cmdWords[5], cmdWords[6]);
    }
    if (cmdInd == ADD_WORKING_HOURS){
        cout << pedarSahab.addWorkingHours(cmdWords[1], cmdWords[2], cmdWords[3], cmdWords[4]);
    }
    if (cmdInd == DELETE_WORKING_HOURS){
        cout << pedarSahab.deleteWorkingHours(cmdWords[1], cmdWords[2]);
    }
    if (cmdInd == UPDATE_TEAM_BONUS){
        cout << pedarSahab.updateTeamBonus(cmdWords[1], cmdWords[2]);
    }
    if (cmdInd == FIND_TEAMS_FOR_BONUS){
        cout << pedarSahab.findTeamsForBonus();
    }
}

int main(int argc, char *argv[]) {
    PedarSahab pedarSahab;

    if(argc == 2){
        readInput((string)argv[1] + DIRECTORY_DELIM, pedarSahab);
    }
    else{
        exit(0);
    }

    string cmdLine;

    while(getline(cin, cmdLine)){
        handleCommand(cmdLine, pedarSahab);
    }
    pedarSahab.freeMemory();
    return 0;
}