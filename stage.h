#pragma once

#include <ncurses.h>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

class Stage {
       private:
	vector<vector<char> > warehouse;
	vector<vector<int> > baggage;
	int own[2];
	int height;
	int width;
	vector<int> way;
	vector<int> flag;
	vector<vector<int> > prebaggage;

	void erase(vector<char>& x) {
		while (!x.empty()) x.pop_back();
	}
	void erase(vector<int>& x) {
		while (!x.empty()) x.pop_back();
	}
	bool check(int y, int x) {
		// 移動先が壁
		if (warehouse[own[0] + y][own[1] + x] == '#') return false;
		// 移動先が荷物
		for (vector<int>::size_type i = 0; i < baggage.size(); i++) {
			if (baggage[i][0] == own[0] + y &&
			    baggage[i][1] == own[1] + x) {
				// 荷物の移動先が壁
				if (warehouse[own[0] + 2 * y][own[1] + 2 * x] ==
				    '#')
					return false;
				// 荷物の移動先が荷物
				for (vector<int>::size_type j = 0;
				     j < baggage.size(); j++) {
					if (baggage[j][0] == own[0] + 2 * y &&
					    baggage[j][1] == own[1] + 2 * x)
						return false;
				}
				// そうでないなら動かす(ゴールはprintで考慮)
				baggage[i][0] = own[0] + 2 * y;
				baggage[i][1] = own[1] + 2 * x;
				vector<int> tmp;
				tmp.push_back(baggage[i][0]);  //移動先を格納
				tmp.push_back(baggage[i][1]);  //
				prebaggage.push_back(tmp);     //
				flag.push_back(1);
				return true;
			}
		}
		// 移動先がそれ以外
		flag.push_back(0);
		return true;
	}
	void checkback(int y, int x) {
		int f = 0;
		if (flag.back()) {
			for (vector<int>::size_type i = 0; i < baggage.size();
			     i++) {
				if (baggage[i][0] == prebaggage.back()[0] &&
				    baggage[i][1] == prebaggage.back()[1]) {
					baggage[i][0] += y;
					baggage[i][1] += x;
					f = 1;
				}
			}
		}
		if (f) prebaggage.pop_back();
		own[0] += y;
		own[1] += x;
		flag.pop_back();
	}

       public:
	int errorflag;

	Stage(string& s) {
		initscr();
		keypad(stdscr, TRUE);
		curs_set(0);

		start_color();
		init_pair(1, COLOR_RED, COLOR_BLACK);
		init_pair(2, COLOR_YELLOW, COLOR_BLACK);
		init_pair(3, COLOR_BLUE, COLOR_BLACK);
		init_pair(4, COLOR_WHITE, COLOR_BLACK);

		ifstream ifs(s.c_str());
		string str;
		width = 0;
		height = 0;
		int y = 0;
		errorflag = 0;

		vector<char> ctmp;
		vector<int> itmp;
		while (getline(ifs, str)) {
			int x = 0;
			while (char c = str[x]) {
				switch (c) {
					case '#':  // warehouse
						// に壁の座標をいれる
						ctmp.push_back('#');
						break;
					case ' ':  // warehouse
						// にスペースを入れる
						ctmp.push_back(' ');
						break;
					case '$':  // warehouse
						// にスペースを入れる,
						// baggage
						// に荷物の座標をいれる
						ctmp.push_back(' ');
						itmp.push_back(y);
						itmp.push_back(x);
						baggage.push_back(itmp);
						erase(itmp);
						break;
					case '.':  // warehouse
						// にゴールの座標をいれる
						ctmp.push_back('.');
						break;
					case '@':  // warehouse
						// にスペースを入れる, own
						// に自分の座標をいれる
						ctmp.push_back(' ');
						own[0] = y;
						own[1] = x;
						break;
					case '+':  // warehouse
						   // にゴールをいれる,
						// own に自分の座標をいれる
						ctmp.push_back('.');
						own[0] = y;
						own[1] = x;
						break;
					case '*':  // warehouse
						   // にゴールをいれる
						   // baggage に
						// 荷物の座標をいれる
						ctmp.push_back('.');
						itmp.push_back(y);
						itmp.push_back(x);
						baggage.push_back(itmp);
						erase(itmp);
						break;

					default:
						endwin();
						cerr << "object is not fit."
						     << endl;
						errorflag = 1;
						break;
				}
				if (errorflag) break;
				x++;
			}
			if (errorflag) break;
			if (width < x) width = x;
			warehouse.push_back(ctmp);
			y++;
			erase(ctmp);
		}
		height = y;
	}

	void moving(int n) {
		switch (n) {
			case 0:  // LEFT
				if (own[1] > 0) {
					if (check(0, -1)) {
						way.push_back(0);
						own[1]--;
					}
				}
				break;
			case 1:  // RIGHT
				if (own[1] < width - 1) {
					if (check(0, 1)) {
						way.push_back(1);
						own[1]++;
					}
				}
				break;
			case 2:  // UP
				if (own[0] > 0) {
					if (check(-1, 0)) {
						way.push_back(2);
						own[0]--;
					}
				}
				break;
			case 3:  // DOWN
				if (own[0] < height - 1) {
					if (check(1, 0)) {
						way.push_back(3);
						own[0]++;
					}
				}
				break;
			default:
				break;
		}
	}

	void print() {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				move(y, x);
				attron(COLOR_PAIR(1));
				addch(warehouse[y][x]);
			}
		}
		for (vector<int>::size_type i = 0; i < baggage.size(); i++) {
			move(baggage[i][0], baggage[i][1]);
			attron(COLOR_PAIR(2));
			addch('$');
		}
		move(own[0], own[1]);
		attron(COLOR_PAIR(4));
		addch('@');
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (warehouse[y][x] == '.') {
					if (y == own[0] && x == own[1]) {
						move(y, x);
						attron(COLOR_PAIR(3));
						addch('+');
					} else {
						for (vector<int>::size_type i =
							 0;
						     i < baggage.size(); i++) {
							if (y ==
								baggage[i][0] &&
							    x ==
								baggage[i][1]) {
								move(y, x);
								attron(
								    COLOR_PAIR(
									3));
								addch('*');
							}
						}
					}
				}
			}
		}
		refresh();
	}
	bool beat() {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (warehouse[y][x] != '.') {
					for (vector<int>::size_type i = 0;
					     i < baggage.size(); i++) {
						if (y == baggage[i][0] &&
						    x == baggage[i][1])
							return 0;
					}
				}
			}
		}
		return 1;
	}

	void goback() {
		if (!way.empty()) {
			switch (way.back()) {
				case 0:
					checkback(0, 1);
					break;
				case 1:
					checkback(0, -1);
					break;
				case 2:
					checkback(1, 0);
					break;
				case 3:
					checkback(-1, 0);
					break;
			}
			way.pop_back();
			return;
		}
		return;
	}
};
