#include "quest.h"
void quest::create_question(const string name_test, const string name_cat)
{
	int col_q = 0;
	string question, answer;
	int right_answer=0;
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
	save_test();
}

void quest::create_test(const string name_cat)
{
	read_test();
	if (!tests_.contains(name_cat)) {
		int wl = 0;
		string name_test;
		cout << "Название теста:";
		cin >> name_test;
		tests_[name_cat][name_test] = json::array();
		do
		{
			cout << "сколько вы хотите создать вопросов(До 12)?";
			cout << "0-Выход\n>";
			cin >> wl;
		} while (wl < 0 || wl>12);
		if (wl > 0)
			for (int i = 0; i < wl; i++) {
				create_question(name_test, name_cat);
			}
		else
			return;
	}
	else throw exception("Категория такой нету!");
}
void quest::create_category()
{
	read_test();
	string name_cat;
	cout << "Название категории:";
	cin >> name_cat;
	if (!tests_.contains(name_cat)) {
		tests_[name_cat] = json::array();
		tests_[name_cat].push_back({});
	}
	else throw exception("Категория уже есть!");
	save_test();
}
void quest::read_test()
{
	const string dir = { "../Testing_Program/test/" };
	_mkdir(dir.c_str());
	ifstream fin(dir+"tests.json");
	if (!fin.is_open()) {
		ofstream fout(dir+"test.json");
		fout << "{}";
		fout.close();
	}

	string str, tmp;

	while (getline(fin, str))
		tmp += str;

	if (!tmp.empty())
		tests_ = json::parse(tmp);

	fin.close();
}

void quest::save_test() const
{
	if (ofstream file("../Testing_Program/test/test.json"); file.is_open())
	{
		if (!tests_.empty())
			file << tests_.dump(2);
	}
}

void quest::show_test()
{
}

void quest::show_cat()
{
	cout << "Категория:\n\n";
	cout << "------------\n";
	for (auto& item : tests_.items())
	{
		string cat = item.key();
		cout << cat;
		cout << "\n------------\n";
	}
}

