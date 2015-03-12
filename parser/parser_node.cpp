#include "parser_node.h"

bin_op_node::bin_op_node(token token, expr_node* left, expr_node* right) : left_(left), right_(right) {
	token_ = token;
}

void bin_op_node::print() {
	left_->print();

	cout << token_.text << endl;

	right_->print();
}

const_node::const_node(token token) {
	token_ = token;
}

void const_node::print() {
	cout << token_.text << endl;
}