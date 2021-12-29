#include <iostream>
#include <string>
#include <ctime>
using namespace std;

class List;
time_t now = time(0);


struct expout
{
	string reason;
	friend ostream& operator<<(ostream &a, expout& ex) {
		a << ex.reason << endl;
		return a;
	}
};

class Account {
	string name;
	int amount = 0;
	tm time_draw{ 0 }; //the draw minute
	int spend = 0; //credit

public:
	int code;
	int number;
	//constractor
	Account(string n, int num, int c) :
		name(n), number(num), code(c) {}
	void cancelAccount(List L, Account& selected);
	Account startOperation(List L);
	void subCash();
	void showCash();
	void addCash();
	Account& operator=(const Account& A);
};

class List {
private:
	class Link {
	public:
		// data areas
		Account value;
		Link * next;
		// constructor
		Link(Account linkValue, Link * nextPtr);
		Link(const Link &);
	};
	// data field
	Link* last;

public:
	// constructors
	List();
	List(const List& l);
	~List();

	// operations
	void add(Account value);
	Account lastElement() const;
	Link* prevLink(int code) const;
	Link* includes(const int c) const;
	bool isEmpty() const;
	void removeLast();
	void clear();
	void removeSelected(Link* p);
};

//////////////////////// List Class //////////////////

List::Link* List::includes(int c) const
{
	// loop to test each element
	for (Link* p = last; p != NULL; p = p->next)
		if (c == p->value.number)
			return p;
	// not found
	return NULL;
}

List::Link* List::prevLink(int code) const // check if vaule is in the list
{
	if (code == lastElement().code)
		return last;
	// loop to test each element
	for (Link* p = last; p != NULL; p = p->next)
		if (code == p->next->value.code)
			return p;
	// not found
	return NULL;
}

List::Link::Link(Account val, Link* nxt) : //add link
	value(val), next(nxt) {}

List::Link::Link(const Link& source) : //copy link
	value(source.value), next(source.next) {}

List::List() : last(NULL) {} // creat list

List::List(const List &l) // copy list
{
	Link *src, *trg;
	if (l.last == NULL)
		last = NULL;
	else
	{
		last = new Link((l.last)->value, NULL);
		src = l.last;
		trg = last;
		while (src->next != NULL)
		{
			trg->next = new Link
			((src->next)->value, NULL);
			src = src->next;
			trg = trg->next;
		}
	}
}

List::~List() // list destractor
{
	clear();
}

void List::clear() // empty all elements from the List
{
	Link* next;
	for (Link * p = last; p != NULL; p = next)
	{
		// delete the element pointed to by p
		next = p->next;
		delete p;
	}
	// mark that the List contains no elements
	last = NULL;
}

bool List::isEmpty() const // ask if list is empty
{
	return last == NULL;
}

void List::add(Account val) // start the creation of the link
{
	last = new Link(val, last);
}

Account List::lastElement() const // return the last value that entered
{
	// return last value in List
	if (isEmpty()) {
		expout ex;
		ex.reason = "\tthe List is empty, no last Element";
		throw ex;
	}
	return last->value;
}

void List::removeLast() // remove the last value that entered
{
	// make sure there is a last element
	if (isEmpty()) {
		expout ex;
		ex.reason = "\tthe List is empty, no Elements to move";
		throw ex;
	}
	// save pointer to the removed node
	Link* p = last;
	// reassign the last node
	last = p->next;
	// recover memory used by the last element
	delete p;
	cout << "\tAccount has been deleted" << endl;
}

void List::removeSelected(Link* p) {
	if (p == last)
		removeLast();
	else {
		Link*tmp = p->next;
		p->next = p->next->next;
		delete tmp;
		cout << "\tAccount has been deleted" << endl;
	}
}

/////////////////Account Class//////////////////

void Account::cancelAccount(List L, Account& selected) {
	if (amount != 0) {
		expout ex;
		ex.reason = "\tThere is money in the account and can't be deleted";
		throw ex;
	}
	else {
		L.removeSelected(L.prevLink(code));
		selected = selected.startOperation(L);
	}
}

int len(int number) {
	int counter=0;
	while (number){
		number = number / 10;
		counter++;
	}
	return counter;
}

Account Account::startOperation(List L) {
	try {
		while (true)
		{
			int code, number;
			string answer;
			cout << "\n\tdo you want to access your account? (Y/N)\n"
				<< "\t[\"No\" will let you to create new account]: ";
			cin >> answer;
			while (answer != "y" && answer != "Y" && answer != "N" && answer != "n") {
				cout << "\ttry again: ";
				cin >> answer;
			}
			cout << "\tenter the account number: ";
			cin >> number;
			while (len(number) != 6) {
				cout << "\tnumber must be exact 6 digits, please try again: ";
				cin >> number;
			}
			cout << "\tenter the secret code: ";
			cin >> code;
			while (len(code) != 4) {
				cout << "\tnumber must be exact 4 digits, please try again: ";
				cin >> code;
			}
			if (answer == "Y" || answer == "y") {
				if (L.includes(number) && L.includes(number)->value.code == code) {
					return (L.includes(number)->value);
				}
				else
					cout << "\tAccount not found OR wrong code" << endl;
			}
			else {
				string name;
				while (L.includes(number) || len(number) != 6) {
					cout << "\tAccount number already in use, please try another number\n"
					<< "\tenter diffrent account number: ";
					cin >> number;
				}
				cout << "\tplease provide your name: ";
				cin.get();
				getline(cin, name);
				Account val(name, number, code);
				L.add(val);
				cout << "\tAccount created successfully\n\n";
				return val;
			}
		}
	}
	catch (...) {
		cout << "error";
	};
}

void Account::showCash() {
	cout << "\tyour amount is " << amount << " NIS" << endl;
}

void Account::subCash() {
	int money;
	cout << "\tHow much money do you like to draw? ";
	cin >> money;
	expout ex;
	if (amount < money) {
		ex.reason = "\tYou don't have enough money";
		throw ex;
	}
	if (money > 1000) {
		ex.reason = "\tYou can't draw more than 1000 NIS at once";
		throw ex;
	}
	int check = difftime((now / 60) % 60, time_draw.tm_min);
	if (check >= 1)
		spend = 0;
	if (spend + money > 1000) {
		now += 60;
		ex.reason = "\tYou try to spend more than 1000 NIS in one minute\n"
					"\tIn that case, you will able to spend until 1000 NIS at " + (string)(ctime(&now));
		throw ex;
	}
	time_draw.tm_min = (now / 60) % 60;
	spend += money;
	amount -= money;
	cout << "\tyou get " << money << " NIS successfully" << endl;
}

void Account::addCash() {
	int chak;
	cout << "\tenter the amount of the chak: ";
	cin >> chak;
	if (chak <= 10000) {
		amount += chak;
		cout << "\tyou add " << chak << " NIS to your account successfully" << endl;
	}
	else {
		expout ex;
		ex.reason = "\tyou can't add more than 10,000 NIS at once";
		throw ex;
	}
}

Account& Account::operator=(const Account& A) {
	name = A.name;
	number = A.number;
	code = A.code;
	amount = A.amount;
	time_draw = A.time_draw;
	spend = A.spend;
	return *this;
}

///////////////// Main Section//////////////////

int main() {
	//installize list
	List L;
	Account val1("ariel", 123456, 1234);
	L.add(val1);
	Account val2("avihu", 023456, 5678);
	L.add(val2);
	Account val3("daniel", 103456, 9000);
	L.add(val3);
	Account val4("efrat", 120456, 1011);
	L.add(val4);
	Account val5("eitan", 123056, 1213);
	L.add(val5);
	Account val6("hodaya", 123406, 1478);
	L.add(val6);
	
	cout << "\n\n\t\t\t\t\t\tWellcome to Digital Bank\n";
	Account selected("", 0, 0);
	selected = selected.startOperation(L);
	int choice = -1;
	while (choice != 0) {
		cout << "\n\tpick an action:\n";
		cout <<	"\t0 - exit program\n";
		cout <<	"\t1 - draw money\n";
		cout <<	"\t2 - show amount\n";
		cout <<	"\t3 - chak money\n";
		cout <<	"\t4 - delete account\n";
		cout <<	"\n\tyour choice: ";
		cin >> choice;
		try {
			switch (choice)
			{
			case 1:
				now = time(0);
				selected.subCash();
				break;
			case 2:
				selected.showCash(); //check how much is left
				break;
			case 3:
				now = time(0);
				selected.addCash();
				break;
			case 4:
				selected.cancelAccount(L, selected);
				break;
			}
		}
		catch (expout problem)
		{
			cout << problem;
		}
	}
	return 0;
}