#include "token.h"

token_maps t_maps;

token_maps::token_maps() {
	//fill token_types
	token_type_to_str[TOKEN_KEYWORD] = "keyword";
	token_type_to_str[TOKEN_IDENTIFIER] = "identifier";
	token_type_to_str[TOKEN_PUNCTUATOR] = "punctuator";
	token_type_to_str[TOKEN_INTEGER] = "integer";
	token_type_to_str[TOKEN_DOUBLE] = "double";
	token_type_to_str[TOKEN_CHAR] = "char";
	token_type_to_str[TOKEN_STRING] = "string";
	token_type_to_str[TOKEN_COMMENT] = "comment";

	//fill pctr_types
	pctr_type_to_str[PUNCTUATOR_curlybraceL] = "curlybraceL";
	pctr_type_to_str[PUNCTUATOR_curlybraceR] = "curlybraceR";
	pctr_type_to_str[PUNCTUATOR_semicolon] = "semicolon";
	pctr_type_to_str[PUNCTUATOR_comma] = "comma";
	pctr_type_to_str[PUNCTUATOR_braceL] = "braceL";
	pctr_type_to_str[PUNCTUATOR_braceR] = "braceR";
	pctr_type_to_str[PUNCTUATOR_ternaryQ] = "ternaryQ";
	pctr_type_to_str[PUNCTUATOR_ternaryC] = "ternaryC";
	pctr_type_to_str[PUNCTUATOR_squarebraceL] = "squarebraceL";
	pctr_type_to_str[PUNCTUATOR_squarebraceR] = "squarebraceR";
	pctr_type_to_str[PUNCTUATOR_xor] = "xor";
	pctr_type_to_str[PUNCTUATOR_xorAssign] = "xorAssign";
	pctr_type_to_str[PUNCTUATOR_asterisk] = "asterisk";
	pctr_type_to_str[PUNCTUATOR_asteriskAssign] = "asteriskAssign";
	pctr_type_to_str[PUNCTUATOR_notBin] = "notBin";
	pctr_type_to_str[PUNCTUATOR_not] = "not";
	pctr_type_to_str[PUNCTUATOR_notEqual] = "notEqual";
	pctr_type_to_str[PUNCTUATOR_assign] = "assign";
	pctr_type_to_str[PUNCTUATOR_equal] = "equal";
	pctr_type_to_str[PUNCTUATOR_mod] = "mod";
	pctr_type_to_str[PUNCTUATOR_modAssign] = "modAssign";
	pctr_type_to_str[PUNCTUATOR_or] = "or";
	pctr_type_to_str[PUNCTUATOR_orBin] = "orBin";
	pctr_type_to_str[PUNCTUATOR_orBinAssign] = "orBinAssign";
	pctr_type_to_str[PUNCTUATOR_andBin] = "andBin";
	pctr_type_to_str[PUNCTUATOR_and] = "and";
	pctr_type_to_str[PUNCTUATOR_andBinAssign] = "andBinAssign";
	pctr_type_to_str[PUNCTUATOR_plus] = "plus";
	pctr_type_to_str[PUNCTUATOR_increment] = "increment";
	pctr_type_to_str[PUNCTUATOR_plusAssign] = "plusAssign";
	pctr_type_to_str[PUNCTUATOR_minus] = "minus";
	pctr_type_to_str[PUNCTUATOR_fieldPtr] = "fieldPtr";
	pctr_type_to_str[PUNCTUATOR_decrement] = "decrement";
	pctr_type_to_str[PUNCTUATOR_minusAssign] = "minusAssign";
	pctr_type_to_str[PUNCTUATOR_less] = "less";
	pctr_type_to_str[PUNCTUATOR_shiftL] = "shiftL";
	pctr_type_to_str[PUNCTUATOR_lessEqual] = "lessEqual";
	pctr_type_to_str[PUNCTUATOR_shiftLAssign] = "shiftLAssign";
	pctr_type_to_str[PUNCTUATOR_greater] = "greater";
	pctr_type_to_str[PUNCTUATOR_shiftR] = "shiftR";
	pctr_type_to_str[PUNCTUATOR_greaterEqual] = "greaterEqual";
	pctr_type_to_str[PUNCTUATOR_shiftRAssign] = "shiftRAssign";
	pctr_type_to_str[PUNCTUATOR_div] = "div";
	pctr_type_to_str[PUNCTUATOR_divAssign] = "divAssign";
	pctr_type_to_str[PUNCTUATOR_field] = "field";
}

token::token() : type(TOKEN_UNDEFINED), pos(1, 0), text("") {}

bool token::is_constant() {
	return (type == TOKEN_CHAR || type == TOKEN_INTEGER || type == TOKEN_DOUBLE);
}

string token::get_type_name() {
#if 0
	int reserved_for_typename = 10;

	string type_name = t_maps.token_type_to_str[type];
	for (unsigned int i = 0; i < reserved_for_typename - type_name.size(); i++) {
		type_name += " ";
	}
#endif
	return t_maps.token_type_to_str[type];
}

void token::reset() {
	text.clear();
	value.char_value = '\0';
	pos = position();
	type = TOKEN_UNDEFINED;
}

std::ostream& operator << (std::ostream& out, token token_) {
	std::ostream& temp = out << token_.get_type_name() << " " << token_.pos << "\t" << token_.text << "\t";
	switch (token_.type) {
	case     TOKEN_INTEGER:    return temp << token_.value.integer_value;
	case     TOKEN_DOUBLE:     return temp << token_.value.double_value;
	case     TOKEN_CHAR:	   return temp << token_.value.char_value;
	case     TOKEN_PUNCTUATOR: return temp << t_maps.pctr_type_to_str[token_.value.pctr_value];
	default:                   return temp << token_.value.string_value;
	}
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
