#pragma once 

#include <iostream>
#include <string>
#include "..\scanner\token.h"

using namespace std;

class parser_node {
public:
	virtual void print() = 0;

protected:
	token token_;
};

class expr_node : public parser_node {
public:
	virtual void print() = 0;
};

class bin_op_node : public expr_node {
public:
	bin_op_node(token, expr_node* left, expr_node* right);
	virtual void print();

private:
	expr_node* left_;
	expr_node* right_;
};

class const_node : public expr_node {
public:
	const_node(token);

	virtual void print();
};
