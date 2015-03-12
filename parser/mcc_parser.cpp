#include "mcc_parser.h"

mcc_parser::mcc_parser(mcc_scanner* scan): scan_(scan) {}

expr_node* mcc_parser::parse() {
	scan_->next();

	return parse_expr();
}

expr_node* mcc_parser::parse_expr() {
	auto res = parse_term();
	auto op = scan_->get();

	if (op.type == TOKEN_PUNCTUATOR) {
		if (op.value.pctr_value == PUNCTUATOR_plus || op.value.pctr_value == PUNCTUATOR_minus) {
			scan_->next();
			res = new bin_op_node(op, res, parse_term());
		}
	}

	return res;
}

expr_node* mcc_parser::parse_term() {
	auto res = parse_factor();
	auto op = scan_->get();

	if (op.type == TOKEN_PUNCTUATOR) {
		if (op.value.pctr_value == PUNCTUATOR_asterisk || op.value.pctr_value == PUNCTUATOR_div) {
			scan_->next();
			res = new bin_op_node(op, res, parse_term());
		}
	}
	
	return res;
}

expr_node* mcc_parser::parse_factor() {
	auto token = scan_->get();
	if (token.is_constant()) {
		scan_->next();
		return new const_node(token);
	}

	return NULL;
}