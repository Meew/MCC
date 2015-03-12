#pragma once 

#include "scanner_node.h"

using namespace std;

class mcc_scanner {
public:
	mcc_scanner(const char*);
	void assign_file(const char*);

	int next();
	token get();

	class bad_token {};

	token::position get_position();

private:
	token::position curr_pos_;

	scanner_node* node_;
	
	token* token_;

	class bad_file {};
	ifstream input_;

	bool eof_found;
};
