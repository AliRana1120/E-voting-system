#include<iostream>
using namespace std;
class user {
protected: 
	string username;
	string password;
private:
	static int statid;
	int id = statid++;
	string name;
	string city;
	string province;
public:
	user() {};
	user(string u,string p): username(u),password(p) {
	
	}
	bool login(string u, string p)
	{
		//select username and password from voter table
		if (username == u && password == p){
			return 1;
		}
		else
			return 0;
	}
	void setusername(string u) {
		username = u;
		id++;
		//query of insert is called in voter table
	}
	void setpassword(string p) {
		password = p;
		//query of insert is called in voter table
	}
	void setcredentials(string n, string c, string p) {
		name = n;
		city = c;
		province = p;
		//add insert query to insert it in voter table
	}
	~user(){} 
};
class voter :public user {
public:
    void createAccount()
    {
		int c;      //for choice
	cout << "Do you want to create your account \n  1. Yes \n  2. No" << endl;
	cin >> c;
	switch (c) {
	case 1: getInput();break;
	default: break;
	           }
    }
    void viewList() {
		int c;
		do {
			cout << "What do you want to do ? \n  1. view list of elections \n  2. vote for the candidate  \n  3. view status\n   4. exit\n";
			cin >> c;
			switch (c) {
			//case 1: election e; // class election composition
			//	e.ElectionList();break;
			//case 2: candidate c; // class candidate composition
			//	c.VoteCandidate();break;
			case 3: ViewStatus();break;
			default: break;
			}
		} while (c != 4);
	} 
	void getInput() {
		string u, p, n, c, pr;
		int a;
		cout << "enter your username " << endl;
		cin >> u;
		cout << "enter password " << endl;
		cin >> p;
		cout << "enter your name : " << endl;
		cin >> n;
		cout << "Enter your city : " << endl;
		cin >> c;
		cout << "Enter your prvince : " << endl;
		cin >> pr;
		cout << "Enter your age " << endl;
		cin >> a;
		if (a >= 18) {
			setusername(u);
			setpassword(p);
			setcredentials(n, c, pr);
		}
		else
			cout << "You are not able to cast vote " << endl;
	}
	void checkcredentials(string u,string p) {

		if (login(u, p)) {
			cout << "You are logged in successfully !! \n";
			viewList();
		}
		else {
			cout << "invalid credentials \n";
			createAccount();
		}
	}
	voter(string u, string p) : user(u, p) { }
    void ViewStatus()
	{
		//select username and user id and candidate id , partyname , city , province of user and candidate
		cout << "-------------"; // data to be displayed
	}
	~voter(){}
};
int user::statid = 1;
int main()
{
	string u, p;
	cout << "Enter username and password \n";
	cin >> u >> p;
	voter v(u,p);
}