#pragma once 

#include "parser_node.h"
#include "..\scanner\mcc_scanner.h"

using namespace std;

class mcc_parser {
public:
	mcc_parser(mcc_scanner*);

	expr_node* parse();

	expr_node* parse_expr();
	expr_node* parse_term();
	expr_node* parse_factor();

private:
	mcc_scanner* scan_;
};
