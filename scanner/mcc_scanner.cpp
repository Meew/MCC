#include "mcc_scanner.h"

mcc_scanner::mcc_scanner(const char* filename): eof_found(false) {
	token_ = new scanner_node::token();
	node_ = new scanner_node(token_);

	try {
		assign_file(filename);
	}
	catch (mcc_scanner::bad_file) {
		cerr << "Failed to open file." << endl;
	}

	node_->feel_the_node();
}

void mcc_scanner::assign_file(const char* input) {
	input_.open(input);
	if (input_.fail())
		throw bad_file();
}

int mcc_scanner::next() {
	int ch;
	bool found = 0;
	token_->reset();

	scanner_node* node = node_;

	while (true) {
		ch = input_.get();

		curr_pos_.row++;
		if (ch == '\n') {
			curr_pos_.col++;
			curr_pos_.row = 0;
		}

		bool in_root = node == node_;
		if (in_root) {
			if (isspace(ch)) continue;
			if (ch == EOF) return 0;
			token_->pos = curr_pos_;
		}

		scanner_node* temp;
		if ((temp = node->process_and_get_new_child(ch)) != 0) {
			node = temp;
			node->process_char(ch);
		}
		else if (node->can_finish()) {
			token_ = node->finish_processing();

			input_.unget();
			if (ch == '\n') curr_pos_.col--;
			else curr_pos_.row--;

			if (token_->type == TOKEN_COMMENT) {
				if (next() == 0) return 0;
			}
			return 1;
		}
		else {
			throw(bad_token());
		}
	}
}

scanner_node::token mcc_scanner::get() {
	return *token_;
}

scanner_node::token::position mcc_scanner::get_position() {
	return curr_pos_;
}