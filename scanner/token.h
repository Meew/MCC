#pragma once 

#include <iostream>
#include <map>

using namespace std;

enum token_type {
	TOKEN_UNDEFINED = -1, TOKEN_KEYWORD, TOKEN_IDENTIFIER, TOKEN_PUNCTUATOR,
	TOKEN_INTEGER, TOKEN_DOUBLE, TOKEN_CHAR, TOKEN_STRING, TOKEN_COMMENT
};

enum token_punctuator_value {
	PUNCTUATOR_curlybraceL,
	PUNCTUATOR_curlybraceR,
	PUNCTUATOR_semicolon,
	PUNCTUATOR_comma,
	PUNCTUATOR_braceL,
	PUNCTUATOR_braceR,
	PUNCTUATOR_ternaryQ,
	PUNCTUATOR_ternaryC,
	PUNCTUATOR_squarebraceL,
	PUNCTUATOR_squarebraceR,
	PUNCTUATOR_xor,
	PUNCTUATOR_xorAssign,
	PUNCTUATOR_asterisk,
	PUNCTUATOR_asteriskAssign,
	PUNCTUATOR_notBin,
	PUNCTUATOR_not,
	PUNCTUATOR_notEqual,
	PUNCTUATOR_assign,
	PUNCTUATOR_equal,
	PUNCTUATOR_mod,
	PUNCTUATOR_modAssign,
	PUNCTUATOR_or,
	PUNCTUATOR_orBin,
	PUNCTUATOR_orBinAssign,
	PUNCTUATOR_andBin,
	PUNCTUATOR_and,
	PUNCTUATOR_andBinAssign,
	PUNCTUATOR_plus,
	PUNCTUATOR_increment,
	PUNCTUATOR_plusAssign,
	PUNCTUATOR_minus,
	PUNCTUATOR_fieldPtr,
	PUNCTUATOR_decrement,
	PUNCTUATOR_minusAssign,
	PUNCTUATOR_less,
	PUNCTUATOR_shiftL,
	PUNCTUATOR_lessEqual,
	PUNCTUATOR_shiftLAssign,
	PUNCTUATOR_greater,
	PUNCTUATOR_shiftR,
	PUNCTUATOR_greaterEqual,
	PUNCTUATOR_shiftRAssign,
	PUNCTUATOR_div,
	PUNCTUATOR_divAssign,
	PUNCTUATOR_field
};

class token {
public:
	token();

	string get_type_name();

	void reset();

	friend std::ostream& operator << (std::ostream&, token);

	class position {
	public:
		position();
		position(int p1, int p2);
		position(const position& position_);

		position& operator = (const position& position_);
		int operator == (const position& position_);
		friend std::ostream& operator << (std::ostream& out, const position& position_);

		int col;
		int row;
	};

	token_type type;
	union {
		int integer_value;
		double double_value;
		char* string_value;
		char char_value;
	} value;
	position pos;
	string text;

private:
	void feel_map();

	map<token_type, string> enum_to_name;
};