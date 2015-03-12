#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <list>
#include <stdlib.h>

#include "scanner/mcc_scanner.h"

using namespace std;

ofstream out("lex.out");

void show_help() {
	cout << "Welcome to My-C-compiler!" << endl;
	cout << "This complier was made by Maxim Barsukov! Group B8303a!" << endl << endl;

	cout << "Use -h to get help." << endl;
	cout << "Use -l to split your code to lexemes." << endl;
	cout << "Use -s to parse your code." << endl << endl;
	
	return;
}

bool analyze_params(int argc, char* argv[]) {

	if (argc == 1) {
		show_help();

	} else if (argc == 2) {
		if (argv[1][1] == 'h') {
			show_help();
		} else {
			cout << "Error: Unknown argument." << endl;
		}

	} else if (argc == 3) { 
		if (argv[1][1] == 'l') {
			mcc_scanner scanner(argv[2]);
			try {
				while (true) {
					if (scanner.next() == 0) {
						break;
					}
					out << scanner.get() << endl;
				}
			}
			catch (mcc_scanner::bad_token) {
				cerr << "Bad lexeme. Current position - " << scanner.get_position() << endl;
			}
		} else if (argv[1][1] == 's') {
			mcc_scanner scanner(argv[2]);
			//mcc_parser parser(&scanner, ...);
		} else {
			cerr << "Error: Unknown argument." << endl;
		}
	} else {
		cerr << "Too many arguments." << endl;
	}

    return 1;
}

int main(int argc, char* argv[]) {

	analyze_params(argc, argv);

#if 0
	mcc_scanner scanner("C:\\Users\\Meew\\Dropbox\\MCC\\MCC\\all_tests\\davidyuk_test_01.c");
	try {
		while (true) {
			if (scanner.next() == 0) {
				break;
			}
			out << scanner.get() << endl;
		}
	}
	catch (mcc_scanner::bad_token) {
		cerr << "Bad lexeme. Current position - " << scanner.get_position() << endl;
	}	

	system("pause");
#endif

	return 0;
}