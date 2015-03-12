#pragma once

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <fstream>
#include <map>
#include <set>
#include <stdexcept>

#include "token.h"

using namespace std;

enum range_type { RANGE_SINGLE, RANGE_BETWEEN, RANGE_EXCEPTION, RANGE_ENUM };

class scanner_node {
public:
	class token_parts {
	public:
		token_parts();
		void clear();
		string s_integer;
		string s_frac;
		string s_exp;
	};

	token_parts parts_;

public:
	class char_range {
	public:
		char_range(char ch);
		char_range(char ch1, char ch2);
		char_range(vector<char> chs, bool);

		bool is_containing(char);

		vector<char> chars;
		range_type type;
	};

public:
	scanner_node();
	scanner_node(token*);
	scanner_node(bool, token*);

	void feel_the_node();

	virtual token* get_token();

	scanner_node* process_and_get_new_child(char);

	virtual void process_char(char);
	bool can_finish();
	virtual token* finish_processing();

	map<char, scanner_node*> char_to_node;

protected:
	token* token_;
	bool can_be_stripped_;

	virtual char cast_escape(char);

private:
	void add_node_to_children(scanner_node*, scanner_node*); // add second to first node's children

	void add_to_map(char_range, scanner_node*);
	void add_to_map(scanner_node*, char_range, scanner_node*);

	void get_identifiers();
	void get_numbers();

	void get_1st_zero();
	void get_1st_not_zero();
	void get_dot(scanner_node*);
	void get_numbers_after_dot(scanner_node*);
	void get_e(scanner_node*);

	void get_char();
	void get_string();

	scanner_node* add_pctr(char, token_punctuator_value); //pctr - punctuator
	scanner_node* add_to_pctr(scanner_node*, char, token_punctuator_value);

	void get_punctuators();
	void get_single_comments(scanner_node*);
	void get_multiple_comments(scanner_node*);
};

class identifiers_node : public scanner_node {
public:
	identifiers_node(bool, token*);
	void initialize_keywords();

	virtual void process_char(char);
	token* finish_processing();

private:
	set<string> keywords;
};

class int_node : public scanner_node {
public:
	int_node();
	int_node(bool, token*);

	virtual void process_char(char);
	token* finish_processing();
};

class dot_double_node : public int_node {
public:
	dot_double_node();
	dot_double_node(bool, token*);

	virtual void process_char(char);
	token* finish_processing();
};

class exp_double_node : public dot_double_node {
public:
	exp_double_node(bool, token*);

	virtual void process_char(char);
	token* finish_processing();
};

class string_node: public scanner_node {
public:
	string_node(bool, token*);

	virtual void process_char(char);
	token* finish_processing();
};

class char_node : public scanner_node {
public:
	char_node(bool, token*);

	virtual void process_char(char);
	token* finish_processing();
};

class punctuator_node: public scanner_node {
public:
	punctuator_node(bool, token*, token_punctuator_value);

	virtual void process_char(char);
	token* finish_processing();
private:
	token_punctuator_value value_;
};

class comments_node: public scanner_node {
public:
	comments_node(bool, token*);

	virtual void process_char(char);
	token* finish_processing();
};