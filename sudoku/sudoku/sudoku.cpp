#include <fstream>
#include <iostream>
#include <time.h>

class sudoku {
public:
	int check() {
		int x;
		for (int i = 0; i < 81; i++) {
			x = sudo[i];
			sudo[i] = 0;
			if (!sudo_insert(i, x)) {
				return 0;
			}
			sudo[i] = x;
		}
		return 1;
	}
	void sudu_generation(int num, int first_num, char sudo_out_string[]) {
		/*
		num between 0-int-1, first_num between 1-9
		sudu_out check if useful, if can write
		*/
		sudo_out.open(sudo_out_string);
		if (!sudo_out.is_open()) {
			std::cout << "ERROR: file can not open" << std::endl;
		}
		sudo_gene_init(first_num);
		sudo_gene_begin(num, first_num);
		//	sudo_generation_loop(0, num);//第一个数据结构中的数据,起点
		sudo_to_file_flush();
		sudo_out.close();
	}
	void sudo_solve(char sudo_in_string[]) {	//	错误处理..
		char buf[20];
		bool ef = true;
		sudo_in.open(sudo_in_string);
		sudo_out.open("sudoku.txt");
		if (!sudo_in.is_open() | !sudo_out.is_open()) {
			std::cout << "ERROR: file can not open" << std::endl;
		}
		do {
			for (int i = 0; i < 9; i++) {
				if (sudo_in.getline(buf, 20)) {
					sscanf_s(buf, "%d %d %d %d %d %d %d %d %d", &sudo[i * 9], &sudo[i * 9 + 1], &sudo[i * 9 + 2], &sudo[i * 9 + 3], &sudo[i * 9 + 4], &sudo[i * 9 + 5], &sudo[i * 9 + 6], &sudo[i * 9 + 7], &sudo[i * 9 + 8]);
				}
				else {
					ef = false;
				}
			}
			if (!ef) {
				break;
			}
			sudo_solve_new();
			sudo_solve_begin();
		} while (sudo_in.getline(buf, 20));
		sudo_to_file_flush();
		sudo_in.close();
		sudo_out.close();
	}
private:
	std::ofstream sudo_out;
	std::ifstream sudo_in;
	int gene_row[9] = { 0 };
	int gene_hasput[9] = { 0 };
	int gene_33[81] = { 0 };
	int sudo[9 * 9] = { 0 };//ATTENTION
							/*	void sudu_generation_init(int first_num) {
							init the random of num(just small to large is ok, random list of the num position
							}*/
	int rand_posi_order[81 * 9];
	int rand_num_order[9];
	int x1[9] = { 0400,0200,0100,0040,0020,0010,0004,0002,0001 };
	char sudobuf[100000];
	int sudolen = 0;
	void print_arr_rand() {
		sudo_out << "rand_num_order" << std::endl;
		for (int i = 0; i < 9; i++) {
			sudo_out << rand_num_order[i] << " ";
		}
		sudo_out << std::endl;
		sudo_out << "rand_posi_order" << std::endl;
		for (int i = 0; i < 81; i++) {
			for (int j = 0; j < 9; j++) {
				sudo_out << rand_posi_order[i * 9 + j] << " ";
			}
			sudo_out << std::endl;
		}
	}
	void sudo_gene_init(int first_num) {
		int i = 0;
		srand(clock());
		for (i = 0; i < 81; i++) {
			sudo_rand(&rand_posi_order[i * 9], 0, 8);
		}
		sudo_rand(rand_num_order, 0, 8);
		for (i = 0; rand_num_order[i] != first_num - 1; i++);
		rand_num_order[i] = rand_num_order[0];
		rand_num_order[0] = first_num - 1;
	}
	void sudo_rand(int arr[], int begin, int end) {
		int x = 0, k;
		for (int i = end; i >= begin; i--, x++) {
			arr[x] = i;
		}
		for (int i = 0; i < x; i++) {
			k = rand() % x;
			swap(arr[i], arr[k]);
		}
	}
	void swap(int & a, int & b) {
		int c;
		c = a;
		a = b;
		b = c;
	}
	void sudo_gene_begin(int num, int first_num) {
		int x = 0;
		sudo_gene_loop(0, x, num);
	}
	void sudo_gene_loop(int order, int & now_num, int target_num) {
		int canset = 0;
		int depth = order % 9;
		int order_num = order / 9;
		int num = rand_num_order[order_num];//ATTENTION 是否可以直接除
		int i;
		if (order == 0) {
			sudo[0] = num + 1;
			gene_row[num] = gene_row[num] | 0400;
			gene_hasput[depth] = gene_hasput[depth] | 0400;
			gene_33[0] = 0700;
			sudo_gene_loop(order + 1, now_num, target_num);
		}
		else {
			if (now_num == target_num) return;
			//	sudu_to_file();
			canset = gene_row[num] | gene_hasput[depth] | gene_33[order_num * 9 + depth / 3];
			for (int i1 = 1; i1 < 10; i1++) {
				i = rand_posi_order[order * 9 + i1 - 1];	//change the name of i
				if (!(canset & x1[i])) {
					//放入数据表
					//sudu_to_file();
					if (order == 80) {
						sudo[depth * 9 + i] = num + 1;
						sudo_to_file();
						//						sudu[depth * 9 + i] = 0;
						now_num++;
						return;
						//	sudu_out << now_num;
						//输出到文件 //ATTENTION 考虑缓存、效率 
					}
					//更新冲突表 
					sudo_insert_0(i, num, depth, order_num);
					sudo_gene_loop(order + 1, now_num, target_num);
					if (now_num == target_num) return;
					sudo_delete_0(i, num, depth, order_num);
				}
			}
		}
	}
	void sudo_delete_0(int i, int num, int depth, int order_num) {
		//ATTENTION 是否考虑回退模式？栈存储？ 而不是再计算一遍 
		sudo[depth * 9 + i] = 0;//放入数据表
								//更新冲突表 
		gene_row[num] = gene_row[num] & (~x1[i]);
		gene_hasput[depth] = gene_hasput[depth] & (~x1[i]);
		if (depth < 3) {
			if (i <= 2) {
				gene_33[order_num * 9 + 0] &= 0077;
			}
			else if (i <= 5) {
				gene_33[order_num * 9 + 0] &= 0707;
			}
			else if (i <= 8) {
				gene_33[order_num * 9 + 0] &= 0770;
			}
		}
		else if (depth < 6) {
			if (i <= 2) {
				gene_33[order_num * 9 + 1] &= 0077;
			}
			else if (i <= 5) {
				gene_33[order_num * 9 + 1] &= 0707;
			}
			else if (i <= 8) {
				gene_33[order_num * 9 + 1] &= 0770;
			}
		}
		else if (depth < 9) {
			if (i <= 2) {
				gene_33[order_num * 9 + 2] &= 0077;
			}
			else if (i <= 5) {
				gene_33[order_num * 9 + 2] &= 0707;
			}
			else if (i <= 8) {
				gene_33[order_num * 9 + 2] &= 0770;
			}
		}
		sudo[depth * 9 + i] = 0; //后期可删 
	}
	void sudo_insert_0(int i, int num, int depth, int order_num) {
		sudo[depth * 9 + i] = num + 1;
		gene_row[num] = gene_row[num] | x1[i];
		gene_hasput[depth] = gene_hasput[depth] | x1[i];
		if (depth < 3) {
			if (i <= 2) {
				gene_33[order_num * 9 + 0] |= 0700;
			}
			else if (i <= 5) {
				gene_33[order_num * 9 + 0] |= 0070;
			}
			else if (i <= 8) {
				gene_33[order_num * 9 + 0] |= 0007;
			}
		}
		else if (depth < 6) {
			if (i <= 2) {
				gene_33[order_num * 9 + 1] |= 0700;
			}
			else if (i <= 5) {
				gene_33[order_num * 9 + 1] |= 0070;
			}
			else if (i <= 8) {
				gene_33[order_num * 9 + 1] |= 0007;
			}
		}
		else if (depth < 8) {
			if (i <= 2) {
				gene_33[order_num * 9 + 2] |= 0700;
			}
			else if (i <= 5) {
				gene_33[order_num * 9 + 2] |= 0070;
			}
			else if (i <= 8) {
				gene_33[order_num * 9 + 2] |= 0007;
			}
		}
	}
	void sudo_to_file() {
		if (sudolen > 99800) {
			sudobuf[sudolen] = '\0';
			sudo_out << sudobuf;
			sudolen = 0;
		}
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 8; j++) {
				sudobuf[sudolen++] = sudo[i * 9 + j] + '0';
				sudobuf[sudolen++] = ' ';
			}
			sudobuf[sudolen++] = sudo[i * 9 + 8] + '0';
			sudobuf[sudolen++] = '\n';
		}
		sudobuf[sudolen++] = '\n';
	}
	void sudo_to_file_flush() {
		sudobuf[sudolen] = '\0';
		sudo_out << sudobuf;
		sudolen = 0;
	}
	void sudo_solve_begin() {

		int x = 0;
		sudo_solve_init();
		sudo_solve_loop0(0, x);
	}
	void sudo_solve_new() {
		for (int i = 0; i < 9; i++) {
			gene_row[i] = 0;
		}
		for (int i = 0; i < 9; i++) {
			gene_hasput[i] = 0;
		}
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				gene_33[i * 9 + j] = 0;
			}
		}
	}
	void sudo_solve_loop0(int order, int & now_num) {
		int canset = 0;
		int depth = order % 9;
		int order_num = order / 9;
		//	int num = rand_num_order[order_num];//ATTENTION 是否可以直接除
		int i;
		//		sudu_out << "loop ! 2: " << order << " " << num << " " << depth << " " << *now_num << " " << target_num << "\n";
		if (now_num == 1) return;
		if (!check_line(order)) {	//order_num 是否可插入depth行 0-8
			if (order == 80) {
				sudo_to_file();
				now_num = 1;
				return;
			}
			sudo_solve_loop0(order + 1, now_num);
		}
		else {
			if (now_num == 1) return;
			canset = gene_row[order_num] | gene_hasput[depth] | gene_33[order_num * 9 + depth / 3];
			for (int i1 = 1; i1 < 10; i1++) {
				i = i1 - 1;	//change the name of i
				if (!(canset & x1[i])) {
					//放入数据表
					if (order == 80) {
						sudo[depth * 9 + i] = order_num + 1;
						sudo_to_file();
						sudo[depth * 9 + i] = 0;
						now_num++;
						return;
						//	sudu_out << now_num;
						//输出到文件 //ATTENTION 考虑缓存、效率 
					}
					//更新冲突表 
					sudo_insert_0(i, order_num, depth, order_num);
					//		sudu_to_file();
					sudo_solve_loop0(order + 1, now_num);
					if (now_num == 1) return;
					sudo_delete_0(i, order_num, depth, order_num);
				}
			}
		}
	}
	void sudo_solve_loop(int order, int &num) {
		//如果该格是最高一格且无可用解，提示无解并退出
		//如果该格标记为重置，更新可用数据表并设置开始指针
		//如果该格可用解到最末了，标记，sudo_solve_loop()上一个
		/*
		·如果该格是最后一格，#check,测试用，输出结果并退出
		else 标记下一个可用解 sudu_solve_loop 下一个
		*/
		if (sudo[order] != 0) {
			if (order == 80) {
				sudo_to_file();
				num = 1;
				return;
			}
			sudo_solve_loop(order + 1, num);
		}
		else {
			for (int i = 1; i < 10; i++) {
				if (num == 1) return;
				if (sudo_insert(order, i)) {
					sudo[order] = i;
					//					sudo_to_file();
					if (order == 80) {
						sudo_to_file();
						num = 1;
						return;
					}
					else {
						sudo_solve_loop(order + 1, num);
						sudo[order] = 0;
					}
				}
			}
		}
	}
	int sudo_block_num(int posi) {
		int col = posi % 9;
		int row = (posi - col) / 9;
		col = (col - col % 3) / 3;
		row = (row - row % 3) / 3;
		return row * 3 + col;
	}
	void sudo_solve_init() {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (sudo[i * 9 + j] != 0) {
					sudo_insert_0(j, sudo[i * 9 + j] - 1, i, sudo[i * 9 + j] - 1);
				}
			}
		}
		//包括将表移到指定位置，填入可填的数字，设置回溯顺序
	}
	void sudu_solve_fillin() {
		//对数据的可用数据为1的值直接填入。
		//考虑多次生成数据并填入直到无法确定
	}
	int sudo_insert(int posi, int num) {
		int block, col, row, x, y;
		x = posi % 9;
		for (int i = 0; i < 9; i++) {
			if (sudo[i * 9 + x] == num) {
				return 0;
			}
		}
		y = (posi - x) / 9;
		for (int i = 0; i < 9; i++) {
			if (sudo[y * 9 + i] == num) {
				return 0;
			}
		}
		block = sudo_block_num(posi);
		col = block % 3;
		row = (block - col) / 3;
		for (int i = col * 3; i < col * 3 + 3; i++) {
			for (int j = row * 3; j < row * 3 + 3; j++) {
				if (sudo[j * 9 + i] == num) return 0;
			}
		}
		return 1;
	}
	int check_line(int order) {
		int depth = order % 9;
		int order_num = order / 9;
		int canset;
		int i;
		for (int i = 0; i < 9; i++) {
			if (sudo[depth * 9 + i] == order_num + 1) {
				return 0;
			}
		}
		return 1;
	}
};