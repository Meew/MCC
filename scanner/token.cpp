#include "token.h"

token::token() : type(TOKEN_UNDEFINED), pos(1, 0), text("") {
	feel_map();
}

string token::get_type_name() {
	int reserved_for_typename = 10;

	string type_name = enum_to_name[type];
	for (unsigned int i = 0; i < reserved_for_typename - type_name.size(); i++) {
		type_name += " ";
	}

	return enum_to_name[type];
}

void token::reset() {
	text.clear();
	value.char_value = '\0';
	pos = position();
	type = TOKEN_UNDEFINED;
}

token::position::position() : col(1), row(0) {}

token::position::position(int p1, int p2) : col(p1), row(p2) {}

token::position::position(const position& position_) : col(position_.col), row(position_.row) {}

token::position& token::position::operator = (const token::position& position_) {
	col = position_.col;
	row = position_.row;
	return *this;
}

int token::position::operator == (const token::position& position_) {
	return (col == position_.col && row == position_.row);
}

std::ostream& operator << (std::ostream& out, const token::position& position_) {
	return out << "[" << position_.col << ", " << position_.row << "]";
}

void token::feel_map() {
	enum_to_name[TOKEN_KEYWORD] = "keyword";
	enum_to_name[TOKEN_IDENTIFIER] = "identifier";
	enum_to_name[TOKEN_PUNCTUATOR] = "punctuator";
	enum_to_name[TOKEN_INTEGER] = "integer";
	enum_to_name[TOKEN_DOUBLE] = "double";
	enum_to_name[TOKEN_CHAR] = "char";
	enum_to_name[TOKEN_STRING] = "string";
	enum_to_name[TOKEN_COMMENT] = "comment";
}