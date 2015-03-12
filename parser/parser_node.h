#pragma once 

#include "../scanner/token.h"

using namespace std;

class parser_node {
public:
	virtual void print() = 0;

protected:
	token* token_;
};

class expr_node : public parser_node {
	virtual void print() = 0;
};

class const_node : public expr_node {
public:
	const_node(token*);

	virtual void print();
};
