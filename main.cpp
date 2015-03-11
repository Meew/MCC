#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <list>
#include <stdlib.h>

#include "scanner/mcc_scanner.h"

#define DEBUG 1

using namespace std;

/*
Доделать:
destructors
char/string
*/


bool analys_params(int argc, char* argv[]) { //luchshe analyze

	if (argc == 1) {
		cout << "Welcome to My-C-compiler!" << endl << "Use -l to split your code to lexemes." << endl << endl;
		return 0;
	}

	string flag = argv[1];
	if (flag == "-l") {
		mcc_scanner scanner(argv[2]);

		scanner.process_all();
	}

    return 1;
}

int main(int argc, char* argv[]) {
	if (DEBUG) { //lalka-kopalka
		//mcc_scanner scanner("D:\\Work\\MCC\\all_tests\\barsukov_tests\\dot_in_numbers.c");
		//mcc_scanner scanner("D:\\Work\\MCC\\all_tests\\barsukov_tests\\ints.c");
		//mcc_scanner scanner("D:\\Work\\MCC\\all_tests\\barsukov_tests\\barsukov_01.c");
		//mcc_scanner scanner("D:\\Work\\MCC\\all_tests\\barsukov_tests\\dec_dot_minus_exp.c");
		//mcc_scanner scanner("D:\\Work\\MCC\\all_tests\\barsukov_tests\\octal_dot_e.c");
		mcc_scanner scanner("D:\\Work\\MCC\\all_tests\\barsukov_tests\\simple_program.c");
		//mcc_scanner scanner("D:\\Work\\MCC\\all_tests\\davidyuk-punctuator\\braceR.c");
		//mcc_scanner scanner("D:\\Work\\MCC\\all_tests\\davidyuk_comments.c");
		scanner.process_all();
		system("pause");
	} else
		analys_params(argc, argv);


	return 0;
}