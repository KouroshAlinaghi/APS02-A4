#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef long double ld;
typedef pair<int , int> pii;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

const int maxn = 3e6;
const ll mod = 1e9+7;

#define pb push_back
#define endl '\n'
#define dokme(x) cout << x , exit(0)
#define ms(x , y) memset(x , y , sizeof x)
ll pw(ll a, ll b, ll md = mod){ll res = 1;while(b){if(b&1){res=(a*res)%md;}a=(a*a)%md;b>>=1;}return(res);}

int emp_cnt = rng()%10 + 4;
int team_cnt = rng()%emp_cnt;
vector < int > emp_id;
string file_addr = "assets";
vector < string > names = {"soroush", "koroush", "saman", "shadi", "marzie", "Aliakbardehkhoda", "AlbertGholami", "SeyedAliKhameneyi",
    "soroush", "soroush", "erfan", "garshasb", "nigger", "AbbasSalimiAngil", "mozi"};

string random_name(){
    return names[rng()%names.size()];
}

string random_level(){
    switch(rng()%4){
        case 0:
            return "junior";
        case 1:
            return "senior";
        case 2:
            return "expert";
        case 3:
            return "team_lead";
    }
}

void make_emp(){
    int id = 0;
    for(int i = 0 ; i < emp_cnt ; i ++){
        id += rng()%3 + 1;
        emp_id.pb(id);
    }
    shuffle(emp_id.begin(), emp_id.end(), rng);
    string file_name = file_addr + "/employees.csv";
    freopen(file_name.c_str(), "w", stdout);
    cout << "id,name,age,level" << endl;
    for(int id : emp_id){
        cout << id << ',' << random_name() << ',' << rng()%100+20 << ',' << random_level() << endl;
    }
    cout << flush;
}

void make_conf(string s){
    cout << s << ',';
    //why small? to catch division by zero
    cout << rng()%10 << ',' << rng()%10 << ',' << rng()%10 << ',' << rng()%10 << ',' << rng()%20 << endl;
}

void make_salary(){
    string file_name = file_addr + "/salary_configs.csv";
    freopen(file_name.c_str(), "w", stdout);
    cout << "level,base_salary,salary_per_hour,salary_per_extra_hour,official_working_hours,tax_percentage" << endl;
    make_conf("junior");
    make_conf("team_lead");
    make_conf("expert");
    make_conf("senior");    
    cout << flush;
}

vector < int > team_ids;
vector < int > members[100];

void make_teams(){
    string file_name = file_addr + "/teams.csv";
    freopen(file_name.c_str(), "w", stdout);
    int id = 0;
    for(int i = 0 ; i < team_cnt ; i ++){
        id += rng()%4 + 1;
        team_ids.pb(id);
    }
    shuffle(team_ids.begin(), team_ids.end(), rng);
    for(int i = 0 ; i < team_cnt ; i ++){
        members[i].pb(emp_id[i]);
    }
    for(int i = team_cnt ; i < emp_cnt ; i ++){
        int team_id = rng()%(team_cnt + 1);
        if(team_id == 0)continue;
        team_id --;
        members[team_id].pb(emp_id[i]);
    }
    cout << "team_id,team_head_id,member_ids,bonus_min_working_hours,bonus_working_hours_max_variance" << endl;
    int i = 0;
    for(int t : team_ids){
        shuffle(members[i].begin(), members[i].end(), rng);
        cout << t << ',' << members[i].front() << ',';
        for(int j = 0 ; j < members[i].size() ; j ++){
            if(j)cout << '$';
            cout << members[i][j];
        }
        //variance khatari kardesh
        cout << ',' << rng()%20 << ',' << fixed << setprecision(3) << (rng()%100000)/1000.0 << endl;
        i++;
    }   
    cout << flush;
}

vector < pii > intervals[100][35];

#define L first
#define R second

bool good(int l, int r, int day, int ind){
    for(pii x : intervals[ind][day]){
        if(max(l, x.L) < min(r, x.R))
            return 0;
    }
    return 1;
}

void make_hours(){
    string file_name = file_addr + "/working_hours.csv";
    freopen(file_name.c_str(), "w", stdout);
    cout << "employee_id,day,working_interval" << endl;
    int ranges = (emp_cnt*3) + rng()%emp_cnt;
    for(int id = 0 ; id < emp_cnt ; id ++){
        bool ok = 0;
        while(!ok){
            int emp_ind = id;
            int l = rng()%25, r = rng()%25;
            if(l == 24)continue;
            int day = rng()%30 + 1;
            while(r <= l)
                r = rng()%25;
            if(good(l, r, day, emp_ind)){
                ok = 1;
                intervals[emp_ind][day].pb({l, r});
                cout << emp_id[emp_ind] << ',' << day << ',' << l << '-' << r << endl;
            }
        }
        ranges --;
    }
    while(ranges--){
        bool ok = 0;
        while(!ok){
            int emp_ind = rng()%emp_cnt;
            int l = rng()%25, r = rng()%25;
            if(l == 24)continue;
            int day = rng()%30 + 1;
            while(r <= l)
                r = rng()%25;
            if(good(l, r, day, emp_ind)){
                ok = 1;
                intervals[emp_ind][day].pb({l, r});
                cout << emp_id[emp_ind] << ',' << day << ',' << l << '-' << r << endl;
            }
        }
    }
    cout << flush;
}

/*
if(words.front() == "find_teams_for_bonus")
    db.find_teams_for_bonus(db);
*/

void make_input(){
    int commands_cnt = rng()%20+1;
    freopen("input.txt", "w", stdout);
    while(commands_cnt--){
        int cmd = rng()%12;
        if(cmd == 0)
            continue;//cout << random_name() << endl;
        if(cmd == 1)
            cout << "report_salaries" << endl;
        if(cmd == 2)
            cout << "report_employee_salary " << rng()%emp_id.back() + 1 << endl;
        if(cmd == 3)
            cout << "report_team_salary " << rng()%team_ids.back() + 1 << endl;
        if(cmd == 4)
            cout << "report_total_hours_per_day " << rng()%40 << ' ' << rng()%40 << endl;
        if(cmd == 5)
            cout << "report_employee_per_hour " << rng()%emp_id.back() + 1 <<  ' ' << rng()%30 << endl;
        if(cmd == 6)
            cout << "show_salary_config " << ((rng()%2) ? random_level() : random_name()) << endl;
        if(cmd == 7){
            //cout << "update_salary_config" << 
            //goshadam bara in felan
            continue;
        }
        if(cmd == 8)
            cout << "add_working_hours " << rng()%emp_id.back() + 1 << ' ' << rng()%40 << ' ' << rng()%30 << ' ' << rng()%30 << endl;
        if(cmd == 9)
            cout << "delete_working_hours " << rng()%emp_id.back() + 1 << ' ' << rng()%40 << endl;
        if(cmd == 10)
            cout << "update_team_bonus " << rng()%team_ids.back() + 1 << ' ' << rng()%150 << endl;
        if(cmd == 11)  
            cout << "find_teams_for_bonus" << endl;

    }
}

int32_t main(int argc, char *argv[]){
	cin.tie(0)->sync_with_stdio(0);
    if(argc > 1){
        file_addr = argv[1];
    }
	make_emp();
    make_salary();
    make_teams();
    make_hours();
    make_input();
	return(0);
}
