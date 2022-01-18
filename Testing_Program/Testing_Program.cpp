#include "functions.h"
#include "quest.h"
using namespace std;

class Iuser
{
public:
	virtual bool regist(const string& login, const string& password, const string& fio, const string& number) = 0;

	virtual bool login(const string& login, const string& password) = 0;

	virtual void change_pass(const string login, string new_passowrd) = 0;

	virtual void parse() = 0;

	virtual void save() = 0;

	virtual void del_user(const string login) = 0;

	virtual void change_user(const string login) = 0;

	virtual bool get_logged() = 0;

	virtual string get_type() = 0;

	virtual void show_users() = 0;

};

class user :public Iuser
{
public:

	virtual void CreateUser(const string& login, const string& fio, const string& password, const string& number, const string& access)
	{
		user_[login] = {
			{"name", fio},
			{"password", password},
			{"number",number},
			{"access",access}
		};
	}

	void parse() override
	{
		_mkdir(R"(../Testing_Program/account/)");
		ifstream fin("../Testing_Program/account/users.json");
		
			if (!fin.is_open()) {
				ofstream fout("../Testing_Program/account/users.json");
				fout << "{}";
				fout.close();
			}

				string str,tmp;

				while (getline(fin, str))
					tmp += str;

				if (!tmp.empty())
					user_ = json::parse(tmp);

		fin.close();
	}

	void change_pass(const string login, string new_passowrd) override
	{
		user_[login]["password"] = ConvertToAscii(getline_());
	}

	string get_type() { return type_; }

	bool login(const string& login, const string& password) override
	{
		if (user_.contains(login))
			if (user_[login]["password"] == password)
			{
				login_ = login;
				fio_ = ConvertFromAscii(user_[login]["name"]);
				password_ = password;
				logged_ = true;
				type_ = user_[login]["access"];
			}
			else throw exception("Неверный пароль!");
		else throw exception("Пользователь не найден!");
		return true;
	}

	bool get_logged() override
	{
		return logged_;
	}

	void del_user(const string login) override
	{
		if (user_.contains(login))
			if (user_[login] == login)
				if (user_[login]["access"] != "Admin")
				{
					user_.erase(login);
				}
				else throw exception("Админа нельзя удалить!");
			else throw exception("Неверный логин!");
		else throw exception("Пользователь не найден!");

		user_.erase(login);
	}

	void change_user(const string login)
	{
		if (user_.contains(login)) {
			if (user_[login] == login) {
				if (user_[login]["access"] != "Admin")
				{
					cout << "Логин: ";
					string login = ConvertToAscii(getline_());
					cout << "\n\nПароль: ";
					string password = ConvertToAscii(getline_());
					cout << "\n\nФИО: ";
					string fio = ConvertToAscii(getline_());
					cout << "\n\nНомер телефона: ";
					string number = ConvertToAscii(getline_());
				}
				else throw exception("Админа нельзя изменять!");
			}
			else throw exception("Неверный логин!");
		}
		else throw exception("Пользователь не найден!");
	}

	void show_users()
	{
		cout << "--------------";
		for(auto &item:user_.items())
		{
			if (user_[item.key()]["access"] != "Admin") {
				cout << "Логин:" << ConvertFromAscii(item.key());
				cout << "Пароль:" << ConvertFromAscii(item.value()["password"]);
			}
			cout << "--------------";
		}
	}

	bool regist(const string& login, const string& password, const string& fio, const string& number) override
	{
		if (user_.contains(login)) throw exception("Пользователь уже существует");

		CreateUser(login, fio, password, number, "");

		return true;

	}

	void save() override
	{
		if (ofstream file("../Testing_Program/account/users.json"); file.is_open())
		{
			if(!user_.empty())
			file << user_.dump(2);
		}
	}

protected:
	bool logged_ = false;
	string login_, fio_, password_, number_,type_;
	static json user_;
};

class admin final:public user
{
public:
	bool regist(const string& login, const string& password, const string& fio, const string& number) override
	{
		if (user_.contains(login)) throw exception("Пользователь уже существует");

		CreateUser(login, fio, password, number, "Admin");

		return true;

	}
};

class person final :public user
{
public:
	bool regist(const string& login, const string& password, const string& fio, const string& number) override
	{
		if (user_.contains(login)) throw exception("Пользователь уже существует");

		CreateUser(login, fio, password, number, "Person");

		return true;

	}
};

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(nullptr));
	char choose = 0;
	Iuser* pers = new user();
	pers->parse();
	pers->save();
	do {
		if (!pers->get_logged()) {
			cout << "1.Вход\n2.Регистрация\n>> ";
			choose = static_cast<char>(_getch());
			cout << choose << "\n\n";
			switch (choose)
			{
			case '1':
			{
				cout << "Логин: ";
				string number = ConvertToAscii(getline_());
				cout << "\n\nПароль: ";
				string password = ConvertToAscii(getline_());
				if (pers->login(number, password)) cout << "Successfully!" << endl;
			}
			system("cls");
			break;
			case '2':
			{
				system("cls");
				cout << "1.Админ\n2.Гость\n>>";
				choose = static_cast<char>(_getch());
				cout << choose << "\n\n";
				switch (choose)
				{
				case '1':
					pers = new admin;
					break;
				case '2':
					pers = new person;
					break;
				}
				cout << "Логин: ";
				string login = ConvertToAscii(getline_());
				cout << "\n\nПароль: ";
				string password = ConvertToAscii(getline_());
				cout << "\n\nФИО: ";
				string fio = ConvertToAscii(getline_());
				cout << "\n\nНомер телефона: ";
				string number = ConvertToAscii(getline_());
				try
				{
					if (pers->regist(login, password, fio, number)) cout << "Succesfully!" << endl;
				}
				catch (const exception& err)
				{
					cerr << err.what() << endl;
				}
				system("pause");
			}
			break;
			default: {}
			}
			pers->save();
		}
		if (pers->get_logged())
		{
			if(pers->get_type()== "Admin")
			{
				quest qu;
				cout << "|Аккаунт Админа|\n";
				cout << "1.Управление пользователями\n2.Управление тестированием\n>>";
				choose = static_cast<char>(_getch());
				cout << choose << "\n\n";
				switch (choose)
				{
				case '1': {
					pers->show_users();
					cout << "Логин:";
					string login = getline_();
				}
					break;
				case '2': {
					cout << "1.Добавить категорию\n2.Выбрать категорию и создать тест\n>>";
					choose = static_cast<char>(_getch());
					cout << choose << "\n\n";
					switch (choose)
					{
					case '1': {
						qu.create_category();
					}
							break;
					case '2': {
						
						qu.show_cat();
						string name_cat=getline_();
						qu.create_test(name_cat);
					}

							break;
					}
				}
					break;
				
				}
			}
			if (pers->get_type() == "Person")
			{

			}
		}
	} while (choose != '0');
}

json user::user_ = {};