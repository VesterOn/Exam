#pragma once
#include "include.h"

class iquest
{
public:
	virtual ~iquest() = default;
	virtual void read_test() = 0;
	virtual void create_category() = 0;
	virtual void create_question(string name_test, string name_cat) = 0;
	virtual void create_test(string name_cat) = 0;
	virtual void save_test() const = 0;
	virtual void show_test() = 0;
	virtual void show_cat() = 0;
};

class quest final : public iquest
{
	static json tests_;
public:
	void read_test() override;
	void create_category() override;
	void create_question(string name_test, string name_cat) override;
	void create_test(string name_cat) override;
	void save_test() const override;
	void show_test() override;
	void show_cat() override;
};