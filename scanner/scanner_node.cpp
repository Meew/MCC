#include "scanner_node.h"

scanner_node::token::token() : type(TOKEN_UNDEFINED), pos(1, 0), text("") {
	feel_map();
}

string scanner_node::token::get_type_name() {
	int reserved_for_typename = 10;

	string type_name = enum_to_name[type];
	for (unsigned int i = 0; i < reserved_for_typename - type_name.size(); i++) {
		type_name += " ";
	}

	return enum_to_name[type];
}

void scanner_node::token::reset() {
	text.clear();
	value.char_value = '\0';
	pos = position();
	type = TOKEN_UNDEFINED;
}

std::ostream& operator << (std::ostream& out, scanner_node::token token_) {
	switch (token_.type) {
		case     TOKEN_INTEGER:    return out << token_.get_type_name() << " " << token_.pos << "\t" << token_.text << "\t" << token_.value.integer_value;
		case     TOKEN_DOUBLE:     return out << token_.get_type_name() << " " << token_.pos << "\t" << token_.text << "\t" << token_.value.double_value;
		default:                   return out << token_.get_type_name() << " " << token_.pos << "\t" << token_.text << "\t" << token_.value.string_value;
	}
}

scanner_node::token_parts::token_parts() {}

void scanner_node::token_parts::clear() {
	s_integer.clear();
	s_frac.clear();
	s_exp.clear();
}

scanner_node::token::position::position() : col(1), row(0) {}

scanner_node::token::position::position(int p1, int p2) : col(p1), row(p2) {}

scanner_node::token::position::position(const position& position_) : col(position_.col), row(position_.row) {}

scanner_node::token::position& scanner_node::token::position::operator = (const token::position& position_) {
	col = position_.col;
	row = position_.row;
	return *this;
}

int scanner_node::token::position::operator == (const scanner_node::token::position& position_) {
	return (col == position_.col && row == position_.row);
}

std::ostream& operator << (std::ostream& out, const scanner_node::token::position& position_) {
	return out << position_.row << "\t" << position_.col;
}

void scanner_node::token::feel_map() {
	enum_to_name[TOKEN_KEYWORD] = "keyword";
	enum_to_name[TOKEN_IDENTIFIER] = "identifier";
	enum_to_name[TOKEN_PUNCTUATOR] = "punctuator";
	enum_to_name[TOKEN_INTEGER] = "integer";
	enum_to_name[TOKEN_DOUBLE] = "double";
	enum_to_name[TOKEN_CHAR] = "char";
	enum_to_name[TOKEN_STRING] = "string";
	enum_to_name[TOKEN_COMMENT] = "comment";
}

void scanner_node::add_to_map(scanner_node* node, char_range key, scanner_node* value) {
	switch (key.type) {
	case RANGE_SINGLE: {
		node->char_to_node[key.chars[0]] = value;
		break;
	}

	case RANGE_BETWEEN: {
		for (char c = key.chars[0]; c <= key.chars[1]; c++) {
			node->char_to_node[c] = value;
		}
		break;
	}

	case RANGE_EXCEPTION: {
		for (int i = 0; i <= 255; i++) {
			bool found = 0;
			for (unsigned int j = 0; j < key.chars.size(); j++) { // rewrite array chars to set
				if ((char)i == key.chars[j])
					found = 1;
			}
			if (!found) 
				node->char_to_node[(char)i] = value;
		}
		break;
	}

	default: break;
	}
}

void scanner_node::add_to_map(char_range key, scanner_node* value) {
	add_to_map(this, key, value);
}

scanner_node::char_range::char_range(char ch) : type(RANGE_SINGLE) {
	chars.push_back(ch);
}
scanner_node::char_range::char_range(char ch1, char ch2) : type(RANGE_BETWEEN) {
	chars.push_back(ch1);
	chars.push_back(ch2);
}
scanner_node::char_range::char_range(vector<char> chs) : chars(chs), type(RANGE_EXCEPTION) {}

bool scanner_node::char_range::is_containing(char ch) {
	switch (type) {
	case RANGE_SINGLE: {
		return chars[0] == ch;
		break;
	}

	case RANGE_BETWEEN: {
		return (chars[0] <= ch && ch <= chars[1]);
		break;
	}

	case RANGE_EXCEPTION: {
		bool checked = true;
		for (unsigned int i = 0; i < chars.size(); i++) {
			if (chars[i] == ch) {
				checked = false;
				break;
			}
		}
		return checked;
		break;
	}
	default: break;
	}

	return false;
}


scanner_node::scanner_node() {}

scanner_node::scanner_node(token* token) {
	token_ = token;
}

scanner_node::scanner_node(bool can_be_stripped, token* token) {
	can_be_stripped_ = can_be_stripped;
	token_ = token;
}

void scanner_node::feel_the_node() {
	get_identifiers();
	get_numbers();
	get_punctuators();

#if 0
	list_of_nodes.push_back(get_char());
	list_of_nodes.push_back(get_string());

#endif
}

scanner_node::token* scanner_node::get_token() {
	return token_;
}

void scanner_node::get_identifiers() {
	identifiers_node* i_fc = new identifiers_node(true, token_); //i_fc - identifier's first char
	add_to_map(char_range('a', 'z'), i_fc);
	add_to_map(char_range('A', 'Z'), i_fc);
	add_to_map(char_range('_'), i_fc);


	identifiers_node* i_nc = new identifiers_node(true, token_); //i_nc - identifier's next chars
	i_fc->add_to_map(char_range('a', 'z'), i_nc);
	i_fc->add_to_map(char_range('A', 'Z'), i_nc);
	i_fc->add_to_map(char_range('_'),      i_nc);
	i_fc->add_to_map(char_range('0', '9'), i_nc);

	i_nc->char_to_node = i_fc->char_to_node;
}

void scanner_node::get_numbers() {

	get_1st_zero();
	get_1st_not_zero();
	//numbers starting with dot will be added in get_punctruators()
}

void scanner_node::get_1st_zero() {
	int_node* n_fc = new int_node(true, token_); //n_fc - number's first char
	add_to_map(char_range('0'), n_fc);

	int_node* n_hexadec_fc = new int_node(false, token_); //n_hexadec_fc - hexadecimal number's 'x' char
	add_to_map(n_fc, char_range('x'), n_hexadec_fc);
	add_to_map(n_fc, char_range('X'), n_hexadec_fc);


	int_node* n_hexadec_nc = new int_node(true, token_); //n_hexadec_nc - hexadecimal number's next chars
	add_to_map(n_hexadec_fc, char_range('0', '9'), n_hexadec_nc);
	add_to_map(n_hexadec_fc, char_range('a', 'f'), n_hexadec_nc);
	add_to_map(n_hexadec_fc, char_range('A', 'F'), n_hexadec_nc);

	n_hexadec_nc->char_to_node = n_hexadec_fc->char_to_node;


	int_node* n_octal_nc = new int_node(true, token_); //n_octal_nc - octal number's next chars
	add_to_map(n_fc, char_range('0', '7'), n_octal_nc);

	add_to_map(n_octal_nc, char_range('0', '7'), n_octal_nc);

	get_dot(n_octal_nc);
	get_e(n_octal_nc);
}

void scanner_node::get_1st_not_zero() {
	int_node* n_fc = new int_node(true, token_); //n_fc - number's first char
	add_to_map(char_range('1', '9'), n_fc);

	add_to_map(n_fc, char_range('0', '9'), n_fc);

	get_dot(n_fc);
	get_e(n_fc);
}


void scanner_node::get_dot(scanner_node* parent) {
	dot_double_node *dot = new dot_double_node(true, token_);
	add_to_map(parent, char_range('.'), dot);

	get_numbers_after_dot(dot);
	get_e(dot);
}

void scanner_node::get_numbers_after_dot(scanner_node* parent) {
	dot_double_node *nums = new dot_double_node(true, token_);
	add_to_map(parent, char_range('0', '9'), nums);

	add_to_map(nums, char_range('0', '9'), nums);

	get_e(nums);
}

void scanner_node::get_e(scanner_node* parent) {
	exp_double_node *e = new exp_double_node(false, token_); //e - exponent
	add_to_map(parent, char_range('e'), e);
	add_to_map(parent, char_range('E'), e);

	exp_double_node *nums = new exp_double_node(true, token_);
	add_to_map(e, char_range('0', '9'), nums);
	add_to_map(nums, char_range('0', '9'), nums);


	exp_double_node *minus = new exp_double_node(false, token_); //minus before e
	add_to_map(e, char_range('-'), minus);
	add_to_map(minus, char_range('0', '9'), nums);
}

#if 0


scanner_node* scanner_node::get_char() {
	char_node* c_fc = new char_node(false, token_); //c_fc - char's first char
	add_char_range_to_node(c_fc, char_range('\''));
	char_node* c_lc = new char_node(true, token_); //c_lc - char's last char
	add_char_range_to_node(c_lc, char_range('\''));

	add_node_to_children(c_fc, c_lc);


	char_node* c_bs = new char_node(false, token_); //c_bs - char's backslash
	add_char_range_to_node(c_bs, char_range('\\'));

	add_node_to_children(c_fc, c_bs);

	vector<char> chars_exception;
	chars_exception.push_back('\'');
	chars_exception.push_back('\\');
	char_node* c_wo = new char_node(false, token_); //s_bs - string's backslash
	add_char_range_to_node(c_wo, char_range(chars_exception));

	add_node_to_children(c_fc, c_wo);

	add_node_to_children(c_wo, c_lc);


	vector<char> no_chars;
	char_node* c_all = new char_node(false, token_);
	add_char_range_to_node(c_all, char_range(no_chars));

	add_node_to_children(c_bs, c_all);
	add_node_to_children(c_all, c_lc);

	return c_fc;
}

scanner_node* scanner_node::get_string() {
	string_node* s_fc = new string_node(false, token_); //s_fc - string's first char
	add_char_range_to_node(s_fc, char_range('\"'));
	string_node* s_lc = new string_node(true, token_); //s_lc - string's last char
	add_char_range_to_node(s_lc, char_range('\"'));

	add_node_to_children(s_fc, s_lc);


	string_node* s_bs = new string_node(false, token_); //s_bs - string's backslash
	add_char_range_to_node(s_bs, char_range('\\'));

	add_node_to_children(s_fc, s_bs);


	vector<char> chars_exception;
	chars_exception.push_back('\"');
	chars_exception.push_back('\\');
	string_node* s_wo = new string_node(false, token_); //s_bs - string's backslash
	add_char_range_to_node(s_wo, char_range(chars_exception));

	add_node_to_children(s_fc, s_wo);

	add_node_to_children(s_wo, s_bs);
	add_node_to_children(s_wo, s_wo);
	add_node_to_children(s_wo, s_lc);


	vector<char> no_chars;
	string_node* s_all = new string_node(false, token_);
	s_all->list_of_chars.push_back(char_range(no_chars));


	add_node_to_children(s_all, s_wo);
	add_node_to_children(s_all, s_bs);
	add_node_to_children(s_all, s_lc);

	add_node_to_children(s_bs, s_all);

	return s_fc;
}
#endif

void scanner_node::get_single_comments(scanner_node* parent) {
	vector<char> except;
	except.push_back('\n');
	except.push_back(EOF);

	comments_node* open_com = new comments_node(true, token_); //open com
	add_to_map(parent, '/', open_com);

	comments_node* com = new comments_node(false, token_);
	add_to_map(open_com, char_range(except), com);
	add_to_map(com, char_range(except), com);

	comments_node* close_com = new comments_node(true, token_);
	add_to_map(com, char_range('\n'), close_com);
}

void scanner_node::get_multiple_comments(scanner_node* parent) {

	vector<char> except;
	except.push_back('*');

	comments_node* open_com = new comments_node(false, token_);
	add_to_map(parent, '*', open_com);

	comments_node* com = new comments_node(false, token_);
	add_to_map(open_com, char_range(except), com);
	add_to_map(com, char_range(except), com);

	comments_node* close1_com = new comments_node(false, token_);
	add_to_map(com, char_range('*'), close1_com);

	comments_node* close2_com = new comments_node(true, token_);
	add_to_map(close1_com, char_range('/'), close2_com);
}

scanner_node* scanner_node::add_pctr(char ch, token_punctuator_value value) {
	punctuator_node* pctr_node = new punctuator_node(true, token_, value);
	add_to_map(char_range(ch), pctr_node);
	
	return pctr_node;
}

scanner_node* scanner_node::add_to_pctr(scanner_node* node, char ch, token_punctuator_value value) {
	punctuator_node* pctr_node = new punctuator_node(true, token_, value);
	add_to_map(node, char_range(ch), pctr_node);

	return pctr_node;
}

void scanner_node::get_punctuators() {
	scanner_node* temp;

	add_pctr('{', PUNCTUATOR_curlybraceL);
	add_pctr('}', PUNCTUATOR_curlybraceR);
	add_pctr(';', PUNCTUATOR_semicolon);
	add_pctr(',', PUNCTUATOR_comma);
	add_pctr('(', PUNCTUATOR_braceL);
	add_pctr(')', PUNCTUATOR_braceR);
	add_pctr('?', PUNCTUATOR_ternaryQ);
	add_pctr(':', PUNCTUATOR_ternaryC);
	add_pctr('[', PUNCTUATOR_squarebraceL);
	add_pctr(']', PUNCTUATOR_squarebraceR);
	add_pctr('~', PUNCTUATOR_notBin);

	temp = add_pctr('.', PUNCTUATOR_field);
	get_numbers_after_dot(temp); // adding numbers starting with dot

	temp = add_pctr('*', PUNCTUATOR_asterisk);
	add_to_pctr(temp, '=', PUNCTUATOR_asteriskEqual);

	temp = add_pctr('/', PUNCTUATOR_div);
	add_to_pctr(temp, '=', PUNCTUATOR_divEqual);
	get_single_comments(temp); //adding comments
	get_multiple_comments(temp);

	temp = add_pctr('%', PUNCTUATOR_mod);
	add_to_pctr(temp, '=', PUNCTUATOR_modEqual);

	temp = add_pctr('!', PUNCTUATOR_not);
	add_to_pctr(temp, '=', PUNCTUATOR_notEqual);

	temp = add_pctr('^', PUNCTUATOR_xor);
	add_to_pctr(temp, '=', PUNCTUATOR_xorEqual);

	temp = add_pctr('|', PUNCTUATOR_orBin);
	add_to_pctr(temp, '=', PUNCTUATOR_orBinEqual);
	add_to_pctr(temp, '|', PUNCTUATOR_or);

	temp = add_pctr('&', PUNCTUATOR_andBin);
	add_to_pctr(temp, '=', PUNCTUATOR_andBinEqual);
	add_to_pctr(temp, '&', PUNCTUATOR_and);

	temp = add_pctr('+', PUNCTUATOR_plus);
	add_to_pctr(temp, '+', PUNCTUATOR_increment);
	add_to_pctr(temp, '=', PUNCTUATOR_plusEqual);

	temp = add_pctr('-', PUNCTUATOR_minus);
	add_to_pctr(temp, '-', PUNCTUATOR_decrement);
	add_to_pctr(temp, '=', PUNCTUATOR_minusEqual);
	add_to_pctr(temp, '>', PUNCTUATOR_fieldPtr);

	temp = add_pctr('<', PUNCTUATOR_less);
	add_to_pctr(temp, '=', PUNCTUATOR_lessEqual);
	temp = add_to_pctr(temp, '<', PUNCTUATOR_shiftL);
	add_to_pctr(temp, '=', PUNCTUATOR_shiftLEqual);

	temp = add_pctr('>', PUNCTUATOR_greater);
	add_to_pctr(temp, '=', PUNCTUATOR_greaterEqual);
	temp = add_to_pctr(temp, '>', PUNCTUATOR_shiftR);
	add_to_pctr(temp, '=', PUNCTUATOR_shiftREqual);

	temp = add_pctr('=', PUNCTUATOR_assign);
	add_to_pctr(temp, '=', PUNCTUATOR_equal);
}


scanner_node* scanner_node::process_and_get_new_child(char ch) {
	try {
		return char_to_node.at(ch);
	}
	catch (const std::out_of_range& oor) {
		return 0;
	}
}

void scanner_node::process_char(char ch) {}

bool scanner_node::can_finish() {
	return can_be_stripped_;
}

scanner_node::token* scanner_node::finish_processing() {
	return get_token();
}


identifiers_node::identifiers_node(bool can_be_stripped, token* token):
scanner_node(can_be_stripped, token) {
	initialize_keywords();
}

void identifiers_node::initialize_keywords() {
	keywords.insert("break");
	keywords.insert("char");
	keywords.insert("const");
	keywords.insert("continue");
	keywords.insert("do");
	keywords.insert("double");
	keywords.insert("else");
	keywords.insert("for");
	keywords.insert("if");
	keywords.insert("int");
	keywords.insert("return");
	keywords.insert("sizeof");
	keywords.insert("struct");
	keywords.insert("typedef");
	keywords.insert("void");
	keywords.insert("while");
}

void identifiers_node::process_char(char ch) {
	token_->text += ch;
}

scanner_node::token* identifiers_node::finish_processing() {
	token_->value.string_value = (char*)token_->text.c_str();

	set<char*>::iterator it;
	it = keywords.find(token_->value.string_value);
	if (it != keywords.end()) {
		cout << "FOUND";
		token_->type = TOKEN_KEYWORD;
	} else 
		token_->type = TOKEN_IDENTIFIER;

	return token_;
}


int_node::int_node() {};

int_node::int_node(bool can_be_stripped, token* token):
scanner_node(can_be_stripped, token) {}

void int_node::process_char(char ch) {
	token_->parts_.s_integer += ch;

	token_->text += ch;
}

class bad_str_to_convert{};

int str_to_int_with_scale(string string, int scale) {
	int res = 0;
	int num = 0;

	scanner_node::char_range nums('0', '9');
	scanner_node::char_range hexa_chars('a', 'f');
	for (int i = string.size() - 1; i >= 0; i--) {
		if (hexa_chars.is_containing(tolower(string[i]))) {
			num = tolower(string[i]) - 87; // 'a' - 87 == 10
		} else if (nums.is_containing(string[i])) {
			num = ((int)string[i] - '0');
		} else 
			throw(bad_str_to_convert());

		res += int(num * pow(scale, (string.size()) - i - 1));
	}

	return res;
}

scanner_node::token* int_node::finish_processing() {
	if (token_->parts_.s_integer[0] == '0') {
		if (token_->parts_.s_integer[1] == 'x' || token_->parts_.s_integer[1] == 'X') {
			token_->parts_.s_integer.erase(0, 2);
			token_->value.integer_value = str_to_int_with_scale(token_->parts_.s_integer, 16);
		} else {
			token_->parts_.s_integer.erase(0, 1);
			token_->value.integer_value = str_to_int_with_scale(token_->parts_.s_integer, 8);
		}
	} else {
		token_->value.integer_value = str_to_int_with_scale(token_->parts_.s_integer, 10);
	}

	token_->type = TOKEN_INTEGER;

	token_->parts_.clear();
	return get_token();
}


dot_double_node::dot_double_node() {}

dot_double_node::dot_double_node(bool can_be_stripped, token* token):
int_node(can_be_stripped, token) {}

void dot_double_node::process_char(char ch) {
	token_->parts_.s_frac += ch;

	token_->text += ch;
}

double get_frac(string frac) {
	if (frac[0] == '.') 
		frac.erase(0, 1);
	int power = -1 * frac.size();

	return str_to_int_with_scale(frac, 10)* pow(10, power);
}

scanner_node::token* dot_double_node::finish_processing() {
	token_->value.double_value = 0.0;

	token_->value.double_value += str_to_int_with_scale(token_->parts_.s_integer, 10);

	token_->value.double_value += get_frac(token_->parts_.s_frac);

	token_->type = TOKEN_DOUBLE;

	token_->parts_.clear();
	return get_token();
}


exp_double_node::exp_double_node(bool can_be_stripped, token* token):
dot_double_node(can_be_stripped, token) {}

void exp_double_node::process_char(char ch) {
	token_->parts_.s_exp += ch;

	token_->text += ch;
}

scanner_node::token* exp_double_node::finish_processing() {
	int factor = 1;

	token_->value.double_value = 0.0;

	token_->value.double_value += str_to_int_with_scale(token_->parts_.s_integer, 10);
	if (!token_->parts_.s_frac.empty()) {
		token_->value.double_value += get_frac(token_->parts_.s_frac);
	}

	token_->parts_.s_exp.erase(0, 1); //killing 'e'
	if (token_->parts_.s_exp[0] == '-') {
		factor = -1;
		token_->parts_.s_exp.erase(0, 1); //killing '-'
	}

	token_->value.double_value *= pow(10, factor * str_to_int_with_scale(token_->parts_.s_exp, 10));

	token_->type = TOKEN_DOUBLE;

	token_->parts_.clear();
	return get_token();
}


string_node::string_node(bool can_be_stripped, token* token):
scanner_node(can_be_stripped, token) {}

void string_node::process_char(char ch) {
	token_->text += ch;
}

scanner_node::token* string_node::finish_processing() {
	string string;
	for (unsigned int i = 1; i < token_->text.size() - 1; i++) {
		if (token_->text[i] != '\\') {
			string += token_->text[i];
		} else {
			string += token_->text[i+1]; //cast from escape
			i++;
		}
	}

	token_->value.string_value = new char[string.size() + 1];
	for (unsigned int i = 0; i < string.size(); i++) {
		token_->value.string_value[i] = string[i];
	}
	token_->value.string_value[string.size()] = '\0';

	
	token_->type = TOKEN_STRING;
	return get_token();
}

                          
char_node::char_node(bool can_be_stripped, token* token):
scanner_node(can_be_stripped, token) {}

void char_node::process_char(char ch) {
	token_->text += ch;
}

scanner_node::token* char_node::finish_processing() {
	if (token_->text[1] != '\\') {
		token_->value.char_value = token_->text[1];
	} else {
		token_->value.char_value = token_->text[2]; //cast from escape
	}

	token_->type = TOKEN_CHAR;
	return get_token();
}

                          
punctuator_node::punctuator_node(bool can_be_stripped, token* token, token_punctuator_value value):
scanner_node(can_be_stripped, token) {
	feel_map();
	value_ = value;
}

void punctuator_node::feel_map() {
	enum_to_name[PUNCTUATOR_curlybraceL] = "curlybraceL";
	enum_to_name[PUNCTUATOR_curlybraceR] = "curlybraceR";
	enum_to_name[PUNCTUATOR_semicolon] = "semicolon";
	enum_to_name[PUNCTUATOR_comma] = "comma";
	enum_to_name[PUNCTUATOR_braceL] = "braceL";
	enum_to_name[PUNCTUATOR_braceR] = "braceR";
	enum_to_name[PUNCTUATOR_ternaryQ] = "ternaryQ";
	enum_to_name[PUNCTUATOR_ternaryC] = "ternaryC";
	enum_to_name[PUNCTUATOR_squarebraceL] = "squarebraceL";
	enum_to_name[PUNCTUATOR_squarebraceR] = "squarebraceR";
	enum_to_name[PUNCTUATOR_xor] = "xor";
	enum_to_name[PUNCTUATOR_xorEqual] = "xorEqual";
	enum_to_name[PUNCTUATOR_asterisk] = "asterisk";
	enum_to_name[PUNCTUATOR_asteriskEqual] = "asteriskEqual";
	enum_to_name[PUNCTUATOR_notBin] = "notBin";
	enum_to_name[PUNCTUATOR_not] = "not";
	enum_to_name[PUNCTUATOR_notEqual] = "notEqual";
	enum_to_name[PUNCTUATOR_assign] = "assign";
	enum_to_name[PUNCTUATOR_equal] = "equal";
	enum_to_name[PUNCTUATOR_mod] = "mod";
	enum_to_name[PUNCTUATOR_modEqual] = "modEqual";
	enum_to_name[PUNCTUATOR_or] = "or";
	enum_to_name[PUNCTUATOR_orBin] = "orBin";
	enum_to_name[PUNCTUATOR_orBinEqual] = "orBinEqual";
	enum_to_name[PUNCTUATOR_andBin] = "andBin";
	enum_to_name[PUNCTUATOR_and] = "and";
	enum_to_name[PUNCTUATOR_andBinEqual] = "andBinEqual";
	enum_to_name[PUNCTUATOR_plus] = "plus";
	enum_to_name[PUNCTUATOR_increment] = "increment";
	enum_to_name[PUNCTUATOR_plusEqual] = "plusEqual";
	enum_to_name[PUNCTUATOR_minus] = "minus";
	enum_to_name[PUNCTUATOR_fieldPtr] = "fieldPtr";
	enum_to_name[PUNCTUATOR_decrement] = "decrement";
	enum_to_name[PUNCTUATOR_minusEqual] = "minusEqual";
	enum_to_name[PUNCTUATOR_less] = "less";
	enum_to_name[PUNCTUATOR_shiftL] = "shiftL";
	enum_to_name[PUNCTUATOR_lessEqual] = "lessEqual";
	enum_to_name[PUNCTUATOR_shiftLEqual] = "shiftLEqual";
	enum_to_name[PUNCTUATOR_greater] = "greater";
	enum_to_name[PUNCTUATOR_shiftR] = "shiftR";
	enum_to_name[PUNCTUATOR_greaterEqual] = "greaterEqual";
	enum_to_name[PUNCTUATOR_shiftREqual] = "shiftREqual";
	enum_to_name[PUNCTUATOR_div] = "div";
	enum_to_name[PUNCTUATOR_divEqual] = "divEqual";
	enum_to_name[PUNCTUATOR_field] = "field";
}

void punctuator_node::process_char(char ch) {
	token_->text += ch;
}

scanner_node::token* punctuator_node::finish_processing() {
	token_->value.string_value = enum_to_name[value_];
	token_->type = TOKEN_PUNCTUATOR;

	return get_token();
}


comments_node::comments_node(bool can_be_stripped, token* token):
scanner_node(can_be_stripped, token) {}

void comments_node::process_char(char ch) {
	token_->text += ch;
}

scanner_node::token* comments_node::finish_processing() {
	token_->value.string_value = (char*)token_->text.c_str();
	token_->type = TOKEN_COMMENT;
	return get_token();
}
