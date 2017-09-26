#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <unistd.h>
#include "piece_def.h"
#include "evaluation.h"

#ifdef _WIN32
#include <windows.h>
#endif

//#define MACRO_RELEASE
//#define MACRO_PROC
//#define MACRO_MORE // optimize, 此种方式对左右移动顺序有要求，暂不使用

//#define _CRT_SECURE_NO_WARNINGS

// iphone X
using namespace ipx;

int top_row1 = 18;
int game_board1[GAME_ROW][GAME_COL] =
{
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 },
	{ 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1 },
	{ 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1 },
	{ 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 },
};

static int g_eliminate_score[] = { 0, 100, 200, 400, 800, 0, 0, 0, 0 };
static int g_eliminate_time_us = 500000;
static int g_proc_time_us = 500000;

int g_cur_score = 0;
int g_cur_piece, g_next_piece;
int g_cur_game_board[GAME_ROW][GAME_COL] = { {0} };
int g_cur_top_row = GAME_ROW;
std::string g_path;

static void print_cur_info() {
	std::cout << "g_cur_score: " << g_cur_score
	          << " g_cur_piece: " << g_cur_piece
	          << " g_next_piece: " << g_next_piece;

	for (int i = 0; i < GAME_ROW; ++i) {
		for (int j = 0; j < GAME_COL; ++j) {
			std::cout << g_cur_game_board[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

static int error_handle(const char *s)
{
	//perror(s);
	exit(1);
}

static void my_sleep(int us)
{
#ifdef __linux__
	if (us <= 1000000) {
		usleep(us);   // us
	} else {
		sleep(us / 1000000);
	}
#elif _WIN32
	Sleep(us / 1000); // ms
#endif
}

int init_board(russia_game_t *game)
{
	memset(game->board, S_NULL, BOARD_ROW * BOARD_COL * sizeof(int));

	game->top_row = GAME_ROW;
	game->old_top_row = -1;
	game->score = 0;
	game->lines = 0;

	for (int i = 0; i < BOARD_ROW; ++i) {
		game->board[i][0] = S_B;
		game->board[i][BOARD_COL - 1] = S_B;
	}
	for (int j = 1; j < BOARD_COL - 1; ++j) {
		game->board[0][j] = S_B;
		game->board[BOARD_ROW - 1][j] = S_B;
	}
	return 0;
}

int test_board(russia_game_t *game, int top_row, int board[GAME_ROW][GAME_COL])
{
	memset(game->board, S_NULL, BOARD_ROW * BOARD_COL * sizeof(int));

	game->top_row = top_row;
	game->old_top_row = -1;
	game->score = 0;
	game->lines = 0;

	for (int i = 0; i < BOARD_ROW; ++i) {
		game->board[i][0] = S_B;
		if (i >= 1 && i <= GAME_ROW) {
			for (int k = 0; k < GAME_COL; ++k) {
				game->board[i][k + 1] = board[i - 1][k];
			}
		}
		game->board[i][BOARD_COL - 1] = S_B;
	}

	for (int j = 1; j < BOARD_COL - 1; ++j) {
		game->board[0][j] = S_B;
		game->board[BOARD_ROW - 1][j] = S_B;
	}
	return 0;
}

void generate_shape_by_random(std::vector<shape_t>& shape_vec)
{
	const int count = 2000000; // row : piece totals = 1 : 2.5
	shape_t last = S_NULL; // 0
	srand((unsigned)time(NULL));

	shape_vec.clear();
	int i = 0;
	while (i < count) {
		shape_t t = static_cast<shape_t>(rand() % SHAPE_COUNT + 1); // 1 ... 7
		if (t != last) {
			shape_vec.push_back(t);
			last = t;
			++i;
		}
	}
}

void print_game(russia_game_t *game)
{
	for (int i = 0; i < BOARD_ROW; ++i) {
		for (int j = 0; j < BOARD_COL; ++j) {
#ifndef MACRO_LOG
			std::cout << shape_char[game->board[i][j]] << ' ';
#else
			__LOG("%c ", shape_char[game->board[i][j]]);
#endif
		}
#ifndef MACRO_LOG
		std::cout << std::endl;
#else
		__LOG("\n");
#endif
	}
}

void print(russia_game_t *game, shape_t next)
{
	for (int i = 0; i < BOARD_ROW; ++i) {
		for (int j = 0; j < BOARD_COL; ++j) {
			std::cout << shape_char[game->board[i][j]] << ' ';
		}

		// TODO
		if ((next != S_NULL) && (i <= SHAPE_BOX/* 4 */)) {
			std::cout << "\t";
			if (i == 0) {
				std::cout << "next : ";
			} else {
				piece_all_shape_t *as = &g_shapes[next - 1];// 下标从0开始
				piece_shape_t *ps = &as->r_shape[0];
				for (int k = 0; k < SHAPE_BOX; ++k) {
					std::cout << shape_char[ps->shape[i - 1][k]] << ' ';
				}
			}
		}
		std::cout << std::endl;
	}
	std::cout << "Total Lines : " << game->lines << ", Total Socre: " << game->score << std::endl;
}

int get_touch_start_row(russia_game_t *game, piece_shape_t *ps)
{
	return game->top_row - ps->height;  // ex. 20 - 2 = 18
}

bool can_shape_movedown(russia_game_t *game, piece_shape_t *ps, int row, int col)
{
	int shape[SHAPE_BOX][SHAPE_BOX] = { { 0 } }; // 4 * 4

	if ((row + ps->height) >= GAME_ROW) // 已经到底部了
		return false;

	++row;
	//std::cout << "row: " << row << ", col: " << col << "\n";

	for (int i = 0; i < ps->height; ++i) {
		for (int j = 0; j < ps->width; ++j) {
			shape[i][j] = game->board[row + i + 1][col + j + 1] + ps->shape[i][j];
			if (shape[i][j] > 1 && shape[i][j] != S_B) {
				return false;
			}
		}
	}

	return true;
}

void add_shape_on_game(russia_game_t *game, piece_shape_t *ps, int row, int col, bool tmp)
{
	for (int i = 0; i < ps->height; ++i) {
		for (int j = 0; j < ps->width; ++j) {
			game->board[row + i + 1][col + j + 1] += ps->shape[i][j];
		}
	}

	if (tmp) {
		game->old_top_row = game->top_row;
	} else {
		game->old_top_row = -1;
	}

	if (game->top_row >= row) {
		game->top_row = row;
		//if(tmp)
		//std::cout << "add_shape_on_game change top " << game->old_top_row << " -> " << game->top_row << std::endl;
	}
}

void remove_shape_from_game(russia_game_t *game, piece_shape_t *ps, int row, int col)
{
	for (int i = 0; i < ps->height; ++i) {
		for (int j = 0; j < ps->width; ++j) {
			// 12 * 22 与 4 * 4 对称相减
			game->board[row + i + 1][col + j + 1] -= ps->shape[i][j];
		}
	}

	if (game->old_top_row != -1) {
		//std::cout << "remove_shape_from_game change top " << game->top_row << " -> " << game->old_top_row << std::endl;
		game->top_row = game->old_top_row;
	}
}

bool check_valid(russia_game_t *game, piece_shape_t *ps, int row, int col)
{
	int shape[SHAPE_BOX][SHAPE_BOX] = { { 0 } }; // 4 * 4

	bool at_least_one_piece_at_bottom = false;

	for (int i = 0; i < ps->height; ++i) {
		for (int j = 0; j < ps->width; ++j) {
			shape[i][j] = game->board[row + i + 1][col + j + 1] + ps->shape[i][j];
			if (shape[i][j] > 1 && shape[i][j] != S_B) {
				return false;
			}
			if (i == ps->height - 1
			        && ps->shape[i][j] == S_OCCUPIED /*当前方块最底层方格非空*/
			        && (game->board[row + i + 2/*当前方块下一行是否有支撑块*/][col + j + 1] == S_OCCUPIED || game->board[row + i + 2/*当前方块下一行是否有支撑块*/][col + j + 1] == S_B)) {
				at_least_one_piece_at_bottom = true;
			}
			// T, S, Z
			if (ps->height > 1
			        && i == ps->height - 2/*方块的倒数第二行*/
			        && (game->board[row + i + 2][col + j + 1] + ps->shape[i][j] > 1 && game->board[row + i + 2][col + j + 1] + ps->shape[i][j] != S_B)) {
				at_least_one_piece_at_bottom = true;
			}
		}
	}
	if (at_least_one_piece_at_bottom) {
		return true;
	}
	return false;
}

void calc(russia_game_t *game, piece_shape_t *ps, int row, int col, evaluate_result_t *result)
{
	add_shape_on_game(game, ps, row, col, true);

	int values = evaluate_func(game, ps, row, col);
	int prs = priority_selection(game, ps->r_index, row, col);

	// 遍历过程
#ifdef MACRO_PROC
	print_game(game);
#ifndef MACRO_LOG
	std::cout << "current : r_index : " << ps->r_index << ", row = " << row <<
	          ", col = " << col << ", values = " << values <<
	          ", prs = " << prs <<
	          " top_row = " << game->top_row << std::endl;
	std::cout << "Total Lines : " << game->lines << ", Total Socre: " << game->score << std::endl;
#else
	__LOG("current : r_index[%d] row[%d] col[%d] values[%d] prs[%d] top_row[%d]\n",
	      ps->r_index, row, col, values, prs, game->top_row);
	__LOG("Total Lines: %d, Total Socre: %d\n", game->lines, game->score);
#endif
	my_sleep(g_proc_time_us);
#endif

	remove_shape_from_game(game, ps, row, col);

	//std::cout << "r_index : " << ps->r_index << ", row = " << row << ", col = " << col << ", value = " << values << std::endl;
	// 记录估值最大的
	if ((values > result->value)/* first or compare */ || ((values == result->value) && (prs > result->prs))) {
		result->row = row;
		result->col = col;
		result->value = values;
		result->prs = prs;
	}
}

// rtn >= 0表示可以放下这个形状，< 0表示无法放下这个形状，可能到顶了
int evaluate_shape(russia_game_t *game, piece_shape_t *ps, evaluate_result_t *result)
{
	// [0], 1, ... 18(row), 19, 20, [21]
	int start_row = get_touch_start_row(game, ps); // ex. 20 - 2 = 18
	if (start_row < 0)
		return -1;

	// 从左向右遍历 ->
	for (int col = 0; col < (GAME_COL - ps->width + 1); ++col) {
		int row = start_row;

		// 下移，直到停下
		while (can_shape_movedown(game, ps, row, col)) {
			++row;
		}
		//std::cout << "out row: " << row << "\n";

#if 0
		// debug
		static int new_index = -1;
		if (new_index != ps->r_index) {
			new_index = ps->r_index;
			std::cout << "new\n";
		}
#endif

#ifdef MACRO_MORE
		// 补充没有上面缺少的列移动情况，但会有重复计算
		// O                 O
		// O   O O     =>    O O O       MACRO_MORE 会考虑此种情况
		// O O   O O         O O O O
		// 左移或右移，即水平遍历，且下面还要有一个方块以支撑
		// 但是仍然有没有考虑到的摆放位置，比如：
		//  O   O O O      O O O O
		//  O O   O O  =>  O O O O       此情况仍然没有考虑
		//  O   O O O      O O O O
		//      O O O        O O O
		// T是可以放进右边的间隙中的，但是并没有考虑到这种情况
		int col_more = 0;
		for (; col_more < (GAME_COL - ps->width + 1); ++col_more) {

			// 对每种情况检查是否合法
			if (check_valid(game, ps, row, col_more)) {
				evaluate_result_t evr = { result->r_index/*r_index*/, 0, 0, SENTINEL, SENTINEL };
				calc(game, ps, row, col_more, &evr);

				if ((evr.value > result->value)
				        || ((evr.value == result->value) && (evr.prs > result->prs))) {
					*result = evr;
				}
			}
		}

#else
		calc(game, ps, row, col, result);
#endif

	}// for

	return 1;// no use
}

void clear_row_status(russia_game_t *game, int row)
{
	for (int i = 0; i < GAME_COL; ++i) {
		game->board[row + 1][i + 1] = S_NULL;
	}
}

void copy_row_status(russia_game_t *game, int from, int to)
{
	for (int i = 0; i < GAME_COL; ++i) {
		game->board[to + 1][i + 1] = game->board[from + 1][i + 1];
	}
}

void eliminate_row(russia_game_t *game,  int row)
{
	if ((row == 0) || (row == game->top_row)) {
		clear_row_status(game, row);
	} else {
		for (int i = row; i > game->top_row; --i) { // 消除的是下面的行，因此要把上面的行依次往下移动
			copy_row_status(game, i - 1, i);
		}
		clear_row_status(game, game->top_row);
	}
}

void put_shape_in_place(russia_game_t *game, piece_shape_t *ps, int row, int col)
{
	add_shape_on_game(game, ps, row, col, false);

#ifndef MACRO_RELEASE
	print_game(game);
	my_sleep(g_proc_time_us);
#endif

	bool done = false;
	int max_row = row + ps->height;
	int total = 0;
	do {
		done = false;
		for (int i = game->top_row; i < max_row; ++i) {
			if (is_full_row_status(game, i)) {

				eliminate_row(game, i);

#ifndef MACRO_RELEASE
				my_sleep(g_eliminate_time_us);
				print_game(game);
#endif

				++game->top_row;
				++total;
				done = true;
				break;
			}
		}
	} while (done);

	game->score += g_eliminate_score[total];
	game->lines += total;
}

std::string calc_move_path(piece_shape_t *best_ps, evaluate_result_t best_r)
{
#if 0
	typedef struct _evaluate_result_t
	{
		int r_index;
		int row;
		int col;
		int value;
		int prs;

	} evaluate_result_t;
#endif
	int l_n;   // 左移次数
	int r_n;   // 右移次数
	int c_n;   // 顺时针旋转次数
	int a_n;   // 逆时针旋转次数
	int d_n;   // 下落次数
	int b_n;   // BT，花费10积分购买一个T形方块，用以替换当前回合的方块 TODO

	l_n = r_n = c_n = a_n = d_n = b_n = 0;
	static char path[256] = { 0 };
	memset(path, 0x0, sizeof(path));
	int len = 0;

	// TODO BT

	// 先计算是否旋转
	if (best_r.r_index > 0) {
		c_n = best_r.r_index; // 已按顺时针排序
		len += snprintf(path, sizeof(path), "C%d,", c_n);
	}

	// piece_def.cpp中定义的init_left_col是从1开始计数
	// 再判断是否左移
	if (best_r.col + 1 < best_ps->init_left_col) {
		l_n = best_ps->init_left_col - (best_r.col + 1);
		if (l_n) {
			len += snprintf(path + len, sizeof(path), "L%d,", l_n);
		}

	} else if (best_r.col + 1 > best_ps->init_left_col) {
		// 右移
		r_n = (best_r.col + 1) - best_ps->init_left_col;
		len += snprintf(path + len, sizeof(path), "R%d,", r_n);

	} // 相等不用移动

	// 最后计算下移个数
	d_n = best_r.row + 1;
	len = snprintf(path + len, sizeof(path), "D%d", d_n);

	return path;
}

std::string calc_buy_next_piece(russia_game_t *game)
{
	std::string empty_piece;
	std::string next_piece;

	int row = 0;
	int col = 0; int pre_col = -1;
	bool is_buy = false;
	int satisfy_line_cnt = 0;
	int satisfy_beg = 0;
	int satisfy_end = 0;

	for (int i = BOARD_ROW - 2/*last row*/; i > 0; --i) {
		int null_piece_cnt = 0;
		for (int j = 1; j < BOARD_COL - 1; ++j) {
			//std::cout << shape_char[game->board[i][j]] << ' ';
			if (game->board[i][j] == S_UNOCCUPIED) {
				++null_piece_cnt;
				row = i;
				col = j;
			}
		}
		//std::cout << "row: " << row << ", col: " << col << ", null_piece_cnt: " << null_piece_cnt << std::endl;

		// 只考虑等于1的情况购买
		if (null_piece_cnt == 1) {

			if (pre_col == -1) {
				//std::cout << "first **: satisfy_line_cnt" << satisfy_line_cnt << ", row: " << row << ", col: " << col << ", null_piece_cnt: " << null_piece_cnt << std::endl;
				++satisfy_line_cnt;
				satisfy_beg = i;
				pre_col = col;  // 记录第一个空格位置

			} else {
				if (col != pre_col) {
					pre_col = col; // 只要不一样就更新，始终记录最上面的空格位置
					satisfy_beg = i;
					satisfy_line_cnt = 1; // 重新计数
				} else {
					//std::cout << "old **: satisfy_line_cnt" << satisfy_line_cnt << ", row: " << row << ", col: " << col << ", null_piece_cnt: " << null_piece_cnt << std::endl;
					++satisfy_line_cnt;
					satisfy_end = i;
				}
			}
		}
	}

	//std::cout << "end satisfy_line_cnt:" << satisfy_line_cnt << "\n";
	//std::cout << "end satisfy_beg:" << satisfy_beg << "\n";
	//std::cout << "end satisfy_end:" << satisfy_end << "\n";
	// 消除1行，获得10分
	// 消除2行，获得30分
	// 消除3行，获得60分
	// 消除4行，获得100分
	// 购买方块，扣掉20分

	// 四行的情况很难存在，三行大概有10次左右
	if (satisfy_line_cnt >= 3) {

		//__LOG("satisfy_line_cnt: %d\n", satisfy_line_cnt);
		//print_game(game);

		// 判断是否连续
		if (satisfy_beg - satisfy_end + 1 == satisfy_line_cnt) {

			__LOG("satisfy_line_cnt: %d satisfy_beg: %d satisfy_end: %d pre_col: %d\n", \
				satisfy_line_cnt, satisfy_beg, satisfy_end, pre_col);
			print_game(game);

			// 这个空格上面没有被遮盖
			bool is_covered = false;
#if 1
			for (int i = satisfy_end - 1; i > 0; --i) {
				if (game->board[i][pre_col] == S_OCCUPIED) {
					is_covered = true;
				}
			}
#endif

			if (!is_covered) {
				// 购买一个I
				next_piece = "BI";
				return next_piece;
			}
		}

	}
	return empty_piece;
}

bool ai_player(russia_game_t *game, shape_t shape)
{
	bool find = false;
	evaluate_result_t best_r = { 0, 0, 0, SENTINEL, SENTINEL };
	piece_shape_t *best_ps = NULL;

	piece_all_shape_t *as = &g_shapes[shape - 1];

	for (int i = 0; i < as->r_count; ++i) {
		piece_shape_t *ps = &as->r_shape[i];
		evaluate_result_t evr = { i, 0, 0, SENTINEL, SENTINEL };

		evaluate_shape(game, ps, &evr);
		if ((evr.value > best_r.value)
		        || ((evr.value == best_r.value) && (evr.prs > best_r.prs))) {
			find = true;
			best_r = evr;
			best_ps = ps;
		}
	}

	if (find) {
		put_shape_in_place(game, &as->r_shape[best_r.r_index], best_r.row, best_r.col);

#if 0
		__LOG("find: r_index: %d, row: %d, col: %d, value: %d, top_row: %d\n",
		      best_r.r_index, best_r.row, best_r.col, best_r.value, game->top_row);
		print_game(game);
#endif

#ifndef MACRO_RELEASE
		std::cout << "find : r_index : " << best_r.r_index << ", row = " << best_r.row <<
		          ", col = " << best_r.col << ", value = " << best_r.value <<
		          " top_row = " << game->top_row << std::endl;
		std::cout << "Total Lines : " << game->lines << ", Total Socre: " << game->score << std::endl;
#endif

		// 根据最终位置计算移动序列
		g_path += calc_move_path(best_ps, best_r);
		std::cout << g_path << std::endl;

	} else {
#ifndef MACRO_LOG
		std::cout << "---------------- GAME OVER ----------------\n";
#else
		__LOG("---------------- GAME OVER ----------------\n");
#endif
	}

	return find;
}

int proc(russia_game_t *game, shape_t s)
{
	if (!ai_player(game, s)) {
		exit(0);
		//std::cout << "Failed!" << std::endl;
	}
	return 0;
}

int proc_random(russia_game_t *game)
{
	std::vector<shape_t> shape_vec;
	generate_shape_by_random(shape_vec);

	for (int i = 0; i < static_cast<int>(shape_vec.size()); ++i) {
		//print(game, shape_vec[i]);
		if (!ai_player(game, shape_vec[i])) {
#ifndef MACRO_LOG
			std::cout << "Failed at: " << i + 1 << " pieces!" << std::endl;
#else
			__LOG("Failed at: %d pieces!\n", i + 1);
#endif
			break;
		}
	}
	return 0;
}

int proc_z_s(russia_game_t *game)
{
	for (int i = 0; ; ++i) {
		if (i % 2) {
			if (!ai_player(game, S_S)) {
#ifndef MACRO_LOG
				std::cout << "Failed!" << std::endl;
#else
				__LOG("Failed!\n");
#endif
				break;
			}
		} else {
			if (!ai_player(game, S_Z)) {
#ifndef MACRO_LOG
				std::cout << "Failed!" << std::endl;
#else
				__LOG("Failed!\n");
#endif
				break;
			}
		}
	}
	return 0;
}

int proc_s_z(russia_game_t *game)
{
	for (int i = 1; ; ++i) {
		if (i % 2) {
			if (!ai_player(game, S_S)) {
#ifndef MACRO_LOG
				std::cout << "Failed!" << std::endl;
#else
				__LOG("Failed!\n");
#endif
				break;
			}
		} else {
			if (!ai_player(game, S_Z)) {
#ifndef MACRO_LOG
				std::cout << "Failed!" << std::endl;
#else
				__LOG("Failed!\n");
#endif
				break;
			}
		}
	}
	return 0;
}

int proc_some_case(russia_game_t *game, shape_t s)
{
	if (!ai_player(game, s)) {
#ifndef MACRO_LOG
		std::cout << "Failed!" << std::endl;
#else
		__LOG("Failed!\n");
#endif
	}
	return 0;
}

int char_to_int(char c)
{
#if 0
	S_NULL = 0,
	S_S = 1,
	S_Z = 2,
	S_L = 3,
	S_J = 4,
	S_I = 5,
	S_O = 6,
	S_T = 7,
	S_B // 边界特殊类型
#endif
	if (c == 'S') {
		return S_S;
	}
	else if (c == 'Z') {
		return S_Z;
	}
	else if (c == 'L') {
		return S_L;
	}
	else if (c == 'J') {
		return S_J;
	}
	else if (c == 'I') {
		return S_I;
	}
	else if (c == 'O') {
		return S_O;
	}
	else if (c == 'T') {
		return S_T;
	}
	else {
		return S_NULL; // error
	}
}

int create_board_from_std(int argc, char **argv)
{
	char game_board[GAME_ROW][GAME_COL] = { {0} };

	FILE *fp = stdin;

	// 当前分数
	if (fscanf(fp, "%d", &g_cur_score) != 1) {
		return error_handle("part1: get score fail");
	}

	// 当前方格 下一个方格
	char piece_info[8] = { 0 };
	if (fscanf(fp, "%s", piece_info) != 1) {
		return error_handle("get piece info fail");
	}
	char cur_piece = '\0';
	char next_piece = '\0';
	sscanf(piece_info, "%c%c", &cur_piece, &next_piece);
	g_cur_piece = char_to_int(cur_piece);    // 当前方格
	g_next_piece = char_to_int(next_piece);  // 下一个方

	// 当前局面
	for (int i = 0; i < GAME_ROW; ++i) {
		if (fscanf(fp, "%s", game_board[i]) != 1)
			return error_handle("get g_cur_game_board fail");
	}

	// 转换 #->1 .->0
	for (int i = 0; i < GAME_ROW; ++i) {
		for (int j = 0; j < GAME_COL; ++j) {
			if (game_board[i][j] == '.') {
				g_cur_game_board[i][j] = S_UNOCCUPIED;
			}
			else if (game_board[i][j] == '#') {
				g_cur_game_board[i][j] = S_OCCUPIED;
			}
			else {
				return error_handle("get game_board fail");
			}
		}
	}

	//print_cur_info();
	return 0;
}

int create_board_from_std2(int argc, char **argv)
{
	char game_board[GAME_ROW][GAME_COL] = { {0} };

	std::vector<std::string> input;
	int read_line = 0;
	std::string str;

	while (getline(std::cin, str)) {
		input.push_back(str);
		++read_line;
		if (read_line >= BOARD_ROW) break;
	}
	for (int i = 0; i < GAME_ROW; ++i) {
		for (int j = 0; j < GAME_COL; ++j) {

		}
	}

	// 当前分数
	g_cur_score = atoi(input[0].c_str());

	// 当前方格 下一个方格
	char cur_piece = '\0';
	char next_piece = '\0';
	sscanf(input[1].c_str(), "%c%c", &cur_piece, &next_piece);
	g_cur_piece = char_to_int(cur_piece);    // 当前方格
	g_next_piece = char_to_int(next_piece);  // 下一个方

	// 当前局面
	for (int i = 0; i < GAME_ROW; ++i) {
		snprintf(game_board[i], GAME_COL, "%s", input[i + 2].c_str());
	}

	// 转换 #->1 .->0
	for (int i = 0; i < GAME_ROW; ++i) {
		for (int j = 0; j < GAME_COL; ++j) {
			if (game_board[i][j] == '.') {
				g_cur_game_board[i][j] = S_UNOCCUPIED;
			}
			else if (game_board[i][j] == '#') {
				g_cur_game_board[i][j] = S_OCCUPIED;
			}
			else {
				return error_handle("get game_board fail");
			}
		}
	}

	//print_cur_info();
	return 0;
}

// 摆放程序
int main(int argc, char **argv)
{
	/* INPUT:
	 * 32行（以\n换行）
	 *
	 * L1 分数
	 * L2 本次方块 下一个方块
	 * L3 当前游戏局面  12 * 30
	 * L4 ...
	 *
	 */

	russia_game_t game;

	// us
	g_proc_time_us = 500000;
	g_eliminate_time_us = 500000;

	int test_scene = 0;
	bool is_first = true;

	switch (test_scene) {

	case 0:
		for (;;) {
			setbuf(stdout, NULL);
			setbuf(stdin, NULL);
			g_path.clear();

			create_board_from_std(argc, argv);
			if (is_first) {
				is_first = false;
				test_board(&game, g_cur_top_row, g_cur_game_board);
			} else {
				test_board(&game, game.top_row, g_cur_game_board);
			}
			// 计算是否购买替换当前方块，目前只替换I
			// 三行或四行的状态不多，效果不明显
			if ((shape_t)g_cur_piece != S_I) {
				std::string next_piece = calc_buy_next_piece(&game);
				if (!next_piece.empty()) {
					g_path = next_piece + std::string(",");
					g_cur_piece = S_I;
				}
			}
			proc(&game, (shape_t)g_cur_piece);
		}
		break;

	case 1:
		init_board(&game);
		proc_random(&game);
		break;

	case 2:
		test_board(&game, top_row1, game_board1);
		proc(&game, S_O);
		break;

	case 3:
		test_board(&game, top_row1, game_board1);
		std::cout << "buy_next_piece: " << calc_buy_next_piece(&game) << "\n";
		break;

	default:
		std::cout << "test_scene: " << test_scene << " is invalid\n";
		return -1;

	}

	print(&game, S_NULL);
	return 0;
}
