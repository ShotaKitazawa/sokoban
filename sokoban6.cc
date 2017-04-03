#include <dirent.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "stage.h"

using namespace std;

string initstage();
void printwin();

int main(int argc, char* argv[]) {
	string s = initstage();
	if (s == "") {
		cerr << "invalid input." << endl;
		return 1;
	}
	Stage* a = new Stage(s);
	if (a->errorflag) {
		return 1;
	}

	a->print();
	int key;
	while (true) {
		key = getch();
		switch (key) {
			case KEY_LEFT:
				a->moving(0);
				break;
			case KEY_RIGHT:
				a->moving(1);
				break;
			case KEY_UP:
				a->moving(2);
				break;
			case KEY_DOWN:
				a->moving(3);
				break;
			case 'h':
				a->moving(0);
				break;
			case 'l':
				a->moving(1);
				break;
			case 'k':
				a->moving(2);
				break;
			case 'j':
				a->moving(3);
				break;
			case 'b':
				a->goback();
				break;
			case 'q':
				endwin();
				return 0;
			case 'r':
				delete a;
				Stage* a = new Stage(s);
				break;
		}
		a->print();
		if (a->beat()) {
			ifstream win("./win.dat");
			if (win) {
				endwin();
				printwin();
				return 0;
			} else {
				endwin();
				cout << "W I N ! !" << endl;
				return 0;
			}
		}
	}
}

string initstage() {
	initscr();
	keypad(stdscr, TRUE);
	curs_set(0);

	const char* path = "./warehouse/";
	DIR* dp;
	dirent* entry;
	int y = 0;
	int ascii = 48;
	vector<string> datpath;
	string tmp;

	dp = opendir(path);
	if (dp == NULL) exit(1);
	readdir(dp);
	readdir(dp);
	do {
		entry = readdir(dp);
		if (entry != NULL) {
			move(0 + 2 * y, 0);
			addch(ascii);
			addch(':');
			addch(' ');
			tmp = entry->d_name;
			addstr(entry->d_name);
			datpath.push_back("./warehouse/" + tmp);
			y++;
			if (ascii < 57)
				ascii++;
			else
				break;
		}
	} while (entry != NULL);
	refresh();
	int key = getch();
	clear();
	endwin();
	if (key-48 >= 0 && key-48 < 10)
		return datpath[key - 48];
	else
		return "";
}

void printwin() {
	initscr();
	keypad(stdscr, TRUE);
	curs_set(0);

	clear();
	ifstream ifs("./win.dat");
	int x = 0;
	int y = 0;
	string str;

	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(1));

	while (getline(ifs, str)) {
		while (char c = str[x]) {
			move(y, x);
			addch(c);
			x++;
		}
		x=0;
		y++;
	}
	refresh();
	getch();
	endwin();
}
