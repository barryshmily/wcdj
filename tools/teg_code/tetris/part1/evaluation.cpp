#include "evaluation.h"

static int sum_n[] = {0, 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78,
                      91, 105, 120, 136, 153, 171, 190, 210
                     };

int ipx::evaluate_func(russia_game_t *game, piece_shape_t *ps, int row, int col)
{
	int value = 0;

	int lh = get_landing_height(game, ps, row, col);
	int epcm = get_erode_piece_cells_metric(game, ps, row, col);
	int brt = get_board_row_transitions(game, ps, row, col);
	int bct = get_board_col_transitions(game, ps, row, col);
	int bbh = get_board_buried_holes(game, ps, row, col);
	int bw = get_board_wells(game, ps, row, col);

	int rows_removed = get_board_rows_removed(game, ps, row, col);

	//value = (-1) * lh + epcm - brt - bct - (4 * bbh) - bw; // 4
	value = (-1) * lh + epcm - (4 * bbh) - (1 * bw);   // 1
	//value = (-1) * lh + epcm - (0.5 * brt) - (0.5 * bct) - (6 * bbh) - (1 * bw); // 2
	//value = (-1) * lh + epcm - brt - bct - (8 * bbh) - (2 * bw); // 3
	
#if 0
	value = (-4.500158825082766) * lh 
		+ (3.4181268101392694) * rows_removed
		+ (-3.2178882868487753) * brt 
		+ (-9.348695305445199) * bct 
		+ (-7.899265427351652) * bbh 
		+ (-3.3855972247263626) * bw; // 5
#endif

	return value;
}

// r_index代表旋转次数，实际上就是r_shape数组的下标
// 落于左侧，priority = 100 * 落子水平平移格子数 + 10 + 落子旋转次数
// 落于右侧，priority = 100 * 落子水平平移格子数 + 落子旋转次数
int ipx::priority_selection(russia_game_t *game, int r_index, int row, int col)
{
	int priority = 0;
	if (col < GAME_COL / 2) {
		priority = 100 * ((GAME_COL / 2 - 1) - col) + 10 + r_index;
	} else {
		priority = 100 * (col - (GAME_COL / 2)) + r_index;
	}
	return priority;
}

bool ipx::is_full_row_status(russia_game_t *game, int row)
{
	for (int i = 0; i < GAME_COL; ++i) {
		if (game->board[row + 1][i + 1] == 0)
			return false;
	}
	return true;
}

int ipx::get_landing_height(russia_game_t *game, piece_shape_t *ps, int row, int col)
{
	//return GAME_ROW - game->top_row;
	return GAME_ROW - row; // ex. 20 - 18 = 2
	//return game->top_row;
}

int ipx::get_erode_piece_cells_metric(russia_game_t *game, piece_shape_t *ps, int row, int col)
{
	int erode_row = 0;      // 消除的行数
	int erode_shape = 0;    // 消除的小方格

	// 从板块最顶层开始向下判断，-> 0, 1, ... 18, 19
	// 当前最高行所在的位置, 作为约束
	int i = game->top_row;
	while (i < GAME_ROW) {
		if (is_full_row_status(game, i)) {
			++erode_row;
			if ((i >= row) && (i <= row + ps->height)) {
				int sline = i - row;
				for (int j = 0; j < ps->width; ++j) {
					if (ps->shape[sline][j] != S_UNOCCUPIED/* 0 */) {
						++erode_shape;
					}
				}
			}
		}
		++i;
	}
	return erode_row * erode_shape; // ex. 0 * 0
}

int ipx::get_board_rows_removed(russia_game_t *game, piece_shape_t *ps, int row, int col)
{
	int erode_row = 0;      // 消除的行数

							// 从板块最顶层开始向下判断，-> 0, 1, ... 18, 19
							// 当前最高行所在的位置, 作为约束
	int i = game->top_row;
	while (i < GAME_ROW) {
		if (is_full_row_status(game, i)) {
			++erode_row;
		}
		++i;
	}
	return erode_row;
}

int ipx::get_board_row_transitions(russia_game_t *game, piece_shape_t *ps, int row, int col)
{
	int transtions = 0;

	// 从板块最顶层开始向下判断，-> 0, 1, ... 18, 19
	// 当前最高行所在的位置, 作为约束
	for (int i = game->top_row; i < GAME_ROW; ++i) {
		for (int j = 0; j < BOARD_COL/* 12 */ - 1; ++j) {
		    // 左右相邻两个不同，则旋转
			if (game->board[i + 1][j] != S_UNOCCUPIED && game->board[i + 1][j + 1] == S_UNOCCUPIED) {
				++transtions;
			}
			if (game->board[i + 1][j] == S_UNOCCUPIED && game->board[i + 1][j + 1] != S_UNOCCUPIED) {
				++transtions;
			}
		}
	}
	return transtions;
}

int ipx::get_board_col_transitions(russia_game_t *game, piece_shape_t *ps, int row, int col)
{
	int transtions = 0;

	// 上下相邻两个不同，则旋转
	for (int j = 0; j < GAME_COL/* 10 */; ++j) {
		for (int i = game->top_row; i < GAME_ROW; ++i) {
			if (game->board[i + 1][j + 1] != S_UNOCCUPIED && game->board[i + 2][j + 1] == S_UNOCCUPIED) {
				++transtions;
			}
			if (game->board[i + 1][j + 1] == S_UNOCCUPIED && game->board[i + 2][j + 1] != S_UNOCCUPIED) {
				++transtions;
			}
		}
	}
	return transtions;
}

int ipx::get_board_buried_holes(russia_game_t *game, piece_shape_t *ps, int row, int col)
{
	int holes = 0;

	// 按列遍历可能存在的所有空洞
	for (int j = 0; j < GAME_COL/* 10 */; ++j) {
		int  i = game->top_row;

		// 先找到最下面一个空格
		while (game->board[i + 1][j + 1] == S_UNOCCUPIED && i < GAME_ROW/* 20 */) {
			++i;
		}

		// 再判断是否是空洞
		while (i < GAME_ROW) {
			if (game->board[i + 1][j + 1] == S_UNOCCUPIED) {
				++holes;
			}
			++i;
		}
	}
	return holes;
}

int ipx::get_board_wells(russia_game_t *game, piece_shape_t *ps, int row, int col)
{
	int wells = 0;
	int sum = 0;

	for (int j = 0; j < GAME_COL; ++j) {
		for (int i = game->top_row; i <= GAME_ROW; ++i) {
			if (game->board[i + 1][j + 1] == S_UNOCCUPIED) {
				if (game->board[i + 1][j] != S_UNOCCUPIED && game->board[i + 1][j + 2] != S_UNOCCUPIED) {
					++wells;
				}
			} else {
				sum += sum_n[wells]; // 深度连加之和，查表
				wells = 0;
			}
		}
	}
	return sum;
}

