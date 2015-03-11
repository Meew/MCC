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

void show_help() {
	cout << "Welcome to My-C-compiler!" << endl;
	cout << "This complier was made by Maxim Barsukov! Group B8303a!" << endl << endl;

	cout << "Use -h to get help." << endl;
	cout << "Use -l to split your code to lexemes." << endl << endl;
	
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
					cout << scanner.get() << endl;
				}
			}
			catch (mcc_scanner::bad_token) {
				cerr << "Bad lexeme. Current position - " << scanner.get_position() << endl;
			}
		} else {
			cout << "Error: Unknown argument." << endl;
		}
	} else {
		cout << "Too many arguments." << endl;
	}

    return 1;
}

int main(int argc, char* argv[]) {
	//analyze_params(argc, argv);

#if 1
	mcc_scanner scanner("C:\\Users\\Meew\\Dropbox\\MCC\\MCC\\all_tests\\barsukov_tests\\comments.c");
	try {
		while (true) {
			if (scanner.next() == 0) {
				break;
			}
			cout << scanner.get() << endl;
		}
	}
	catch (mcc_scanner::bad_token) {
		cerr << "Bad lexeme. Current position - " << scanner.get_position() << endl;
	}	
#endif

	system("pause");
	return 0;
}