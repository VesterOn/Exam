#include "functions.h"

string getline_()
{
	string temp;
	while (temp.empty())
		getline(cin, temp);
	return temp;
}

string ConvertToAscii(const string& str)
{
	string res;
	for (const int ascii : str)
		res.append(to_string(ascii) + " ");
	return res;
}

string ConvertFromAscii(const string& str)
{
	string res, tmp;
	for (const char& symbol : str)
	{
		tmp.push_back(symbol);

		if (symbol == ' ')
		{
			res.push_back(stoi(tmp));
			tmp.clear();
		}

	}
	return res;
}