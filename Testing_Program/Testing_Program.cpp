#include "functions.h"
using namespace std;

class iuser
{
public:
	virtual bool regist(const string& login, const string& password, const string& fio, const string& number) = 0;

	virtual bool login(const string& login, const string& password) = 0;

	virtual void change_pass() = 0;

	virtual void parse() = 0;

	virtual void save() = 0;

	virtual void reset() = 0;

	virtual bool check_for_admin() = 0;

	virtual void del_user() = 0;

	virtual void change_user() = 0;

	virtual bool get_logged() = 0;

	virtual string get_type() = 0;

	virtual string get_login() = 0;

	virtual void show_users() = 0;

	virtual void statistics_test() = 0;

};

class user :public iuser
{
public:
	void reset() override
	{
		login_ = "";
		fio_ = "";
		password_ = "";
		logged_ = false;
		type_ = "";
	}//выход из акаунта

	virtual void create_user(const string& login, const string& fio, const string& password, const string& number, const string& access)
	{
		user_[login] = {
			{"name", fio},
			{"password", password},
			{"number",number},
			{"access",access}
		};
	} //Записть пользователя в json переменую пользователей

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
	}//считование json файла

	bool check_for_admin() override
	{
		for (const auto& item : user_.items()) {
			if (item.value()["access"] == "Admin")
				return true;
		}
		return false;
	}//проверка ести ли админ в json переменой пользователей

	void change_pass() override
	{
		cout << "\nНовый пароль: ";
		user_[login_]["password"] = convert_to_ascii(getline());
	}//изменение пароля

	string get_type() override { return type_; }//Вожрашает тип пользвателя

	bool login(const string& login, const string& password) override
	{
		try
		{
			if (user_.contains(login)) {
				try {
					if (user_[login]["password"] == password)
					{
						login_ = login;
						fio_ = convert_from_ascii(user_[login]["name"]);
						password_ = password;
						logged_ = true;
						type_ = user_[login]["access"];
						return true;
					}
				}
				catch (exception& e)
				{
					return e.what();
				}
			}
		}
		catch (exception& e)
		{
			return e.what();
		}
		return false;
	}//Авторизация пользователя или админа

	bool get_logged() override
	{
		return logged_;
	}//Возрашает true -если пользователь или админ авторизовался

	void del_user() override
	{
		char choose;
		vector<string> login;
		for (auto &item : user_.items())
		{
			login.push_back(static_cast<string>(item.key()));
		}
		cout << ">>";
		choose = static_cast<char>(_getch());
		cout << choose;
		system("cls");
		const string login_ = login[atoi(&choose) - 1];
		if (user_.contains(login_))
			if (user_[login_]["access"] != "Admin")
				user_.erase(login_);
			else throw exception("Админа нельзя удалить!");
		else throw exception("Пользователь не найден!");
	}//Удаление пользователя

	void change_user() override
	{

		char choose;
		vector<string> login;
		string login2_;
		for (auto &item : user_.items())
		{
			if (user_[item.key()]["access"] != "Admin") {
				login.push_back(static_cast<string>(item.key()));
			}
		}
		cout << ">>";
		choose = static_cast<char>(_getch());
		cout << choose;
		int b = (atoi(&choose) - 1);
		system("cls");
			login2_ = login[b];
			if (user_.contains(login2_)) {
				if (user_[login2_]["access"] != "Admin")
				{
					string access = user_[login2_]["access"];
					cout << "Пароль:";
					string password = convert_to_ascii(getline());
					cout << "\nФИО: ";
					string fio = convert_to_ascii(getline());
					cout << "\nНомер телефона: ";
					string number = convert_to_ascii(getline());
					user_[login2_]["access"] = access;
					user_[login2_]["name"] = fio;
					user_[login2_]["number"] = number;
					user_[login2_]["password"] = password;
				}
				else cout << "Админа нельзя изменять!\n";
			}
			else cout << "Пользователь не найден!\n";
		system("pause");
	}//Модификация пользователя

	void show_users() override
	{
		int a = 1;
		for(auto &item:user_.items())
		{
			if (user_[item.key()]["access"] != "Admin") {
				cout << a++ << ")";
				cout << "Логин:" << convert_from_ascii(item.key())<<endl;
				cout << "Пароль:" << convert_from_ascii(item.value()["password"]) << endl;
				cout << "Имя:" << convert_from_ascii(item.value()["name"]) << endl;
				cout << "Номер телефона:" << convert_from_ascii(item.value()["number"]) << endl;
			}
		}
	}//показывет пользователей

	string get_login() override { return login_; };//Возрашает логин

	bool regist(const string& login, const string& password, const string& fio, const string& number) override
	{
		if (user_.contains(login)) throw exception("Пользователь уже существует");

		create_user(login, fio, password, number, "");

		return true;

	}

	void save() override
	{
		if (ofstream file("../Testing_Program/account/users.json"); file.is_open())
		{
			if(!user_.empty())
			file << user_.dump(2);
		}
	}//сохроняет json переменую пользователей

	void statistics_test() override
	{
		int a=1;
		char choose;
		vector<string> cat, podcat;
		cout << "Категории:\n";
		for(auto item :user_[login_].items() )
		{
			if(item.key()!="access"&& item.key() != "name"&& item.key() != "number"&& item.key() != "password")
			{
				cout << a++ << ". " << item.key() << endl;
				cat.push_back(static_cast<string>(item.key()));
			}
		}
		cout << ">>";
		choose = static_cast<char>(_getch());
		cout << choose;
		const string category = cat[atoi(&choose)-1];
		system("cls");
		a = 1;
		cout << "Тесты:\n";
		for (auto& item : user_[login_][category].items())
		{
			cout << a++ << ". " << item.key() << endl;
			podcat.push_back(static_cast<string>(item.key()));
		}
		cout << ">>";
		choose = static_cast<char>(_getch());
		cout << choose;
		const string podcategory = podcat[atoi(&choose) - 1];
		system("cls");

		const int count = user_[login_][category][podcategory][0]["count_right_answer"];
		int colq = user_[login_][category][podcategory][0]["last_quest"];
		colq += 1;
		const int percent = (100 * 1 / colq) * count;
		cout << "Категория:" << category << endl;
		cout << "Тест:" << podcategory << endl;
		cout << "Количество правильных ответов:" << count << endl;
		cout << "Процент правильных ответов:" << percent << "%" << endl;
		cout << "Оценка:" << user_[login_][category][podcategory][0]["score"] << endl;
		system("pause");
		system("cls");
	}//Показует пользователу статистику

	static json user_;
protected:
	bool logged_ = false;
	string login_, fio_, password_, number_, type_;
};

class admin final:public user
{
public:
	bool regist(const string& login, const string& password, const string& fio, const string& number) override
	{
		if (user_.contains(login)) throw exception("Пользователь уже существует");

		create_user(login, fio, password, number, "Admin");
		return true;
	}
};

class person final :public user
{
public:
	bool regist(const string& login, const string& password, const string& fio, const string& number) override
	{
		if (user_.contains(login)) throw exception("Пользователь уже существует");

		create_user(login, fio, password, number, "Person");

		return true;

	}
};

class iquest
{
public:
	virtual void read_test() = 0;
	virtual void create_category() = 0;
	virtual void create_question(string name_test, string name_cat) = 0;
	virtual void create_test(int cat_) = 0;
	virtual void save_test() const = 0;
	virtual void show_cat() = 0;
	virtual void testing(string login) = 0;
};

class quest final : public iquest
{
public:
	static json tests_;

	void read_test() override
	{
		const string dir = { "../Testing_Program/test/" };
		_mkdir(dir.c_str());
		ifstream fin(dir + "test.json");
		if (!fin.is_open()) {
			ofstream fout(dir + "test.json");
			fout << "{}";
			fout.close();
		}

		string str, tmp;

		while (getline(fin, str))
			tmp += str;

		if (!tmp.empty())
			tests_ = json::parse(tmp);

		fin.close();
	}//считывет json файл

	void create_category() override
	{
		string name_cat;
		cout << "Название категории:";
		cin >> name_cat;
		if (!tests_.contains(name_cat)) {
			tests_[name_cat];
		}
		else throw exception("Категория уже есть!");
	}//Создает категорию для тестов

	void create_question(string name_test, string name_cat) override
	{
		int col_q = 0;
		string question, answer;
		int right_answer = 0;
		vector<string> var;
		cin.ignore();
		cout << "Введите вопрос:";
		getline(cin, question);
		do
		{
			cout << "Cколько ответов:";
			cin >> col_q;

		} while (col_q < 2 || col_q>4);
		cin.ignore();
		for (int i = 0; i < col_q; i++)
		{
			cout << i + 1 << ". ";
			getline(cin, answer);
			var.push_back(answer);
		}
		do
		{
			cout << "Укажите номер правильного ответа(от 1 до " << col_q << "): ";
			cin >> right_answer;

		} while (right_answer < 1 || right_answer>col_q);
		tests_[name_cat][name_test].push_back({
			{ "question", question },
			{"answer",var},
			{"right_answer",--right_answer}
			});
	}//Создает вопрос для теста и записует в json переменую

	void create_test(int cat_) override
	{
		vector<string>cat;
		string category;
		for (auto& item : tests_.items())
			cat.push_back(static_cast<string>(item.key()));
		category = cat[cat_ - 1];

		if (tests_.contains(category)) {
			int wl = 0;
			string name_test;
			cout << "Название теста:";
			cin >> name_test;
			tests_[category][name_test] = json::array();
			do
			{
				cout << "сколько вы хотите создать вопросов(До 12)?";
				cout << "0-Выход\n>";
				cin >> wl;
			} while (wl < 0 || wl>20);
			if (wl > 0)
				for (int i = 0; i < wl; i++) {
					create_question(name_test, category);
				}
			else
				return;
		}
		else throw exception("Категория такой нету!");
	}//Создает тест

	void save_test() const override
	{
		if (ofstream file("../Testing_Program/test/test.json"); file.is_open())
		{
			if (!tests_.empty())
				file << tests_.dump(2);
		}
	}//Сохроняет json переменую с тестами

	void show_cat() override
	{
		int a = 1;
		cout << "Категории:\n";
		for (auto& item : tests_.items())
			cout << a++ << ". " << item.key() << endl;
	}//показует категорию

	void testing(const string login) override {
		int count = 0, a = 1, index_q;
		char choose;
		vector<string> cat, podcat;

		cout << "Категории:\n";
		for (auto& item : tests_.items())
		{
			cout << a++ << ". " << item.key() << endl;
			cat.push_back(static_cast<string>(item.key()));
		}
		cout << ">>";
		choose = static_cast<char>(_getch());
		cout << choose;
		const string category = cat[atoi(&choose)-1];
		system("cls");
		a = 1;
		cout << "Тест:\n";
		for (auto& item : tests_[category].items())
		{
			cout << a++ << ". " << item.key() << endl;
			podcat.push_back(static_cast<string>(item.key()));
		}
		cout << ">>";
		choose = static_cast<char>(_getch());
		cout << choose;
		const string podcategory = podcat[atoi(&choose)-1];
		system("cls");
		for (int i = 0; i < tests_[category][podcategory].size(); i++) {

			cout << i + 1 << ". " << static_cast<string>(tests_[category][podcategory][i]["question"]) << endl;
			for (int j = 0; j < tests_[category][podcategory][i]["answer"].size(); j++)
			{
				cout << j + 1 << ". " << static_cast<string>(tests_[category][podcategory][i]["answer"][j]) << endl;
			}
			cout << ">>";
			choose = static_cast<char>(_getch());
			cout << choose;
			system("cls");
			if (tests_[category][podcategory][i]["right_answer"] == atoi(&choose) - 1)
			{
				count++;
			}
			
			if(i < tests_[category][podcategory].size())
			{
				index_q = i;
			}

		}

		const int colq = tests_[category][podcategory].size();
		const int percent = (100 * 1 / colq) * count;
		int score = percent/8;
		cout << "Количество правильных ответов:" << count << endl;
		cout << "Процент правильных ответов:" << percent << "%" << endl;
		cout << "Оценка:" << score << endl;
		system("pause");
		system("cls");
		user::user_[login][category][podcategory] = {
			 {
				{"score",score},
				{"count_right_answer",count},
				{"last_quest",index_q}
			 }
		};
	}//Прохождение теста и записует результаты

};



int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(reinterpret_cast<time_t>(nullptr));

	char choose = 0;
	iuser* pers = new user();
	pers->parse();
	do {
		if (!pers->get_logged()) {
			if(!pers->check_for_admin())
			{
				pers = new admin;
				cout << "    |Регистрация Админа|\n";
				cout << "Логин: ";
				string login = convert_to_ascii(getline());
				cout << "\n\nПароль: ";
				string password = convert_to_ascii(getline());
				cout << "\n\nФИО: ";
				string fio = convert_to_ascii(getline());
				cout << "\n\nНомер телефона: ";
				string number = convert_to_ascii(getline());
				try
				{
					if (pers->regist(login, password, fio, number)) cout << "Succesfully!" << endl;
				}
				catch (const exception& err)
				{
					cerr << err.what() << endl;
				}
				system("cls");
			}
			cout << "1.Вход\n2.Регистрация\n>> ";
			choose = static_cast<char>(_getch());
			cout << choose << "\n\n";
			system("cls");
			switch (choose)
			{
			case '1':
			{
				cout << "Логин: ";
				string number = convert_to_ascii(getline());
				cout << "\nПароль: ";
				string password = convert_to_ascii(getline());
				if (pers->login(number, password)) cout << "Успешно!" << endl;
				else cout << "Неверный логин или пароль!" << endl;
				system("pause");
				system("cls");
			}
			break;
			case '2':
			{
					pers = new person;
				cout << "    |Регистрация Пользователя|\n";
				cout << "Логин: ";
				string login = convert_to_ascii(getline());
				cout << "\nПароль: ";
				string password = convert_to_ascii(getline());
				cout << "\nФИО: ";
				string fio = convert_to_ascii(getline());
				cout << "\nНомер телефона: ";
				string number = convert_to_ascii(getline());
				try
				{
					if (pers->regist(login, password, fio, number)) cout << "Succesfully!" << endl;
				}
				catch (const exception& err)
				{
					cerr << err.what() << endl;
				}
				system("pause");
				system("cls");
			}
			break;
		default: ;
			}
			pers->save();
		}
		if (pers->get_logged())
		{
			if(pers->get_type()== "Admin")
			{
				cout << "    |Аккаунт Админа|\n";
				cout << "1.Управление пользователями\n2.Управление тестированием\n3.Изменить пароль\n4.Выход\n>>";
				choose = static_cast<char>(_getch());
				cout << choose << "\n\n";
				system("cls");
				switch (choose)
				{
				case '1': {//Управление пользователями
					cout << "1.Регистрация\n2.Редактирование\n3.Удаление\n4.Назад\n>>";
					choose = static_cast<char>(_getch());
					cout << choose << "\n\n";
					system("cls");
					switch (choose)
					{
					case '1': {
						pers = new person;
						cout << "    |Регистрация Пользователя|\n";
						cout << "Логин: ";
						string login = convert_to_ascii(getline());
						cout << "\nПароль: ";
						string password = convert_to_ascii(getline());
						cout << "\nФИО: ";
						string fio = convert_to_ascii(getline());
						cout << "\nНомер телефона: ";
						string number = convert_to_ascii(getline());
						try
						{
							if (pers->regist(login, password, fio, number)) cout << "Успешно!" << endl;
						}
						catch (const exception& err)
						{
							cerr << err.what() << endl;
						}
						
						system("pause");
						system("cls");
					}
						break;
					case '2':
						{
						pers->show_users();
						pers->change_user();
						system("cls");
						}
						break;
					case '3': {
						pers->show_users();
						pers->del_user();
						system("cls");
					}
						break;
					default: ;
					}
					pers->save();
				}
					break;
				case '2': { //Управление тестированием
					iquest* qu = new quest();
					qu->read_test();
					cout << "1.Добавить категорию\n2.Выбрать категорию и создать тест\n3.Назад\n>>";
					choose = static_cast<char>(_getch());
					cout << choose << "\n\n";
					system("cls");
					switch (choose)
					{
						
					case '1': {
						qu->create_category();
					}
							break;
					case '2': {
						qu->show_cat();
						choose = static_cast<char>(_getch());
						qu->create_test(atoi(&choose));
					}
							break;
						default: ;
					}
					qu->save_test();
				}
					break;
				case '3': { //Изменение пароля
					pers->change_pass();
				}
					break;
				case '4'://Выход из аккаунта
					pers->reset();
					break;
				default: ;
				}

			}
			if (pers->get_type() == "Person")
			{
				iquest* qu = new quest();
				qu->read_test();
				cout << "    |Аккаунт Пользователя|\n";
				cout << "1.Пройти тест\n2.Посмотреть пройденые тесты\n3.Выход\n>>";
				choose = static_cast<char>(_getch());
				cout << choose << "\n\n";
				switch (choose)
				{
				case '1':{//тестирование
					pers->parse();
						qu->testing(pers->get_login());
						pers->save();
					}
					break;
				case '2'://статистика привидущих тестов
					{
					pers->statistics_test();
					}
					break;
				case '3'://выход
				{
					pers->reset();
				}
				break;
				}
			}
		}
	} while (choose != '0');
}

json user::user_ = {};
json quest::tests_ = {};