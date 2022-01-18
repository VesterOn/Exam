#pragma once
#include "include.h"
class quest
{
	json tests_;
public:
	void read_test();
	void create_category();
	void create_question(string name_test,string name_cat);
	void create_test(string name_cat);
	void save_test() const;
	void show_test();
	void show_cat();
};
