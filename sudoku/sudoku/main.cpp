#include <iostream>
#include <string>
#include "sudoku.h"

int strtoInt(char str[]) {
	int sum = 0;
	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i]<'0' | str[i]>'9') {
			return -1;
		}
		else {
			sum = sum * 10 + str[i] - '0';
		}
	}
	return sum;
}
int main(int argc, char *argv[]) {
	/*	if (argc <= 1) {
	std::cout <<"argc:0 --pls put in something";
	}
	else if (argc == 2) {
	std::cout << "start generation";
	}
	else {
	std::cout << "argc:" << argc << " --not do";
	}*/
	int num;
	/*	suduku sudu;
	sudu.sudu_solve("sudup.txt");*/
	/*	sudu.sudu_generation(1000, 1, "suduku.txt");
	std::cout << "check: "<<sudu.check()<<std::endl;
	std::cout << "check: " << strtoInt("a12") << std::endl;
	std::cout << "check: " << strtoInt("1002") << std::endl;*/
	//	suduku sudu;
	//	sudu.sudu_generation(1000000, 5, "suduku.txt");
	//	sudu.sudu_solve("sudup.txt");
	//	sudu.sudu_solve("a");
	//	int x =sudu.check();
	if (argc != 3) {
		std::cout << "input error: should be -c positive int or -s filename" << std::endl;
		return 0;
	}
	else {
		sudoku sudo;
		if (strcmp(argv[1], "-c") == 0) {
			num = strtoInt(argv[2]);
			if (num < 0) {
				std::cout << "input error: should be -c positive int" << std::endl;
				return 0;
			}
			sudo.sudu_generation(num, 5, "sudoku.txt");
		}
		else if (strcmp(argv[1], "-s") == 0) {
			sudo.sudu_solve(argv[2]);
		}
	}
	return 0;
}