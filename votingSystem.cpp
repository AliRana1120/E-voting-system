#include<iostream>
#include<fstream>
#include<string>
using namespace std;
int main();
class user {
private: 
	string cnic;
	string password;
	static int statid;
	int id = statid++;
	string name;
	string city;
	string province;
public:
	user() {}; //default constructor
	string getname() { return name; }  //utility functions
	string getcity() { return city; }
	string getprovince() { return province; }
	string getcnic() { return cnic; }
	string getpassword() { return password; }
	void setname(string n) { name = n; }
	void setcity(string c) { city = c; }
	void setprovince(string p) { province = p; }
	bool login(string c, string p)
	{
		if (cnic == c && password == p){
			return 1;
		}
		else
			return 0;
	}
	void setcnic(string c) {
		cnic = c;
		//query of insert is called in voter table
	}
	void setpassword(string p) {
		password = p;
		//query of insert is called in voter table
	}
	void setcredentials(string cn,string pass,string n, string c, string p) 
	{
		//add insert query to insert it in voter table
		fstream voter("voter.txt",ios::app);
		if (!voter)
			cerr << "Error finding file .....\n";
		else {
			voter << cn << endl;
			voter << pass << endl;
			voter << n << endl;
			voter << c << endl;
			voter << p << endl;
		}
		voter.close();
		main();
	}
	~user(){} 
};
class voter :public user {
public:
    void createAccount()
    {
		int c;      //for choice
	cout << "Do you want to create your account \n  1. Yes \n  2. No  \n 3. press any key instead of 1 and 2 to exit " << endl;
	cin >> c;
	switch (c) {
	case 1: getInput();break;
	case 2: system("cls");main();break; // is sy dobara main me ja k re-enter kr skty hain
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
		string cn, p, n, c, pr;
		int a;
		cout << "enter your cnic " << endl;
		cin >> cn;
		cout << "enter password " << endl;
		cin >> p;
		cout << "enter your name : " << endl;
		cin >> n;
		cout << "Enter your city : " << endl;
		cin >> c;
		cout << "Enter your province : " << endl;
		cin >> pr;
		cout << "Enter your age " << endl;
		cin >> a;
		if (a >= 18) {
			cout << "Your account has been created successfully!! \n";
			setcredentials(cn,p,n,c,pr);
		}
		else {
			int c = 0;
			cout << "You are not able to cast vote " << endl;
			cout << "Do you want to exit or not 1. Enter 1 for yes \n 2. Enter 2 for no \n";
			if (c == 1)
				exit(0);
			else {
				system("cls");main();
			}
		}
	}
	voter(string c, string p)  {
		string cn,pass,na,ci,pro; // to get cnic and password from file
		//file handling is added
			ifstream voter("voter.txt");
		if (!voter)
			cerr << "error finding file \n";
		else {
			while (getline(voter,cn)) {
				getline(voter, pass);
				getline(voter, na);
				getline(voter, ci);
				getline(voter, pro);
				if (cn == c && pass == p) {
					setcnic(cn);
					setpassword(pass);
					setname(na);
					setcity(ci);
					setprovince(pro);
					break;
				}
			}
		voter.close();
		}

		if (login(c, p)) {
			cout << "You are logged in successfully !! \n";
			viewList();
		}
		else {
			cout << "invalid credentials \n";
			createAccount();
		}
	}
    void ViewStatus()
	{
		//select username and user id and candidate id , partyname , city , province of user and candidate
		cout << "Voter details \n"; // data to be displayed
		cout << "Voter name " << getname() << endl;
		cout << "Voter cnic " << getcnic() << endl;
		cout << "Voter city " << getcity() << endl;
		cout << "Voter province " << getprovince() << endl;

	}
	~voter(){}
};
int user::statid = 1;
int main()
{
	string c, p;
	cout << "Enter cnic and password \n";
	cin >> c >> p;
	voter v(c,p);
	return 0;
}