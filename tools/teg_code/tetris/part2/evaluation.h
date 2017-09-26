#ifndef __EVALUATION_H__
#define __EVALUATION_H__

#include "defs.h"

namespace ipx {

	int evaluate_func(russia_game_t *game, piece_shape_t *ps, int row, int col);
	int priority_selection(russia_game_t *game, int r_index, int row, int col);

	bool is_full_row_status(russia_game_t *game, int row);
	int get_landing_height(russia_game_t *game, piece_shape_t *ps, int row, int col);

	int get_erode_piece_cells_metric(russia_game_t *game, piece_shape_t *ps, int row, int col);
	int get_board_rows_removed(russia_game_t *game, piece_shape_t *ps, int row, int col);

	int get_board_row_transitions(russia_game_t *game, piece_shape_t *ps, int row, int col);
	int get_board_col_transitions(russia_game_t *game, piece_shape_t *ps, int row, int col);

	int get_board_buried_holes(russia_game_t *game, piece_shape_t *ps, int row, int col);
	int get_board_wells(russia_game_t *game, piece_shape_t *ps, int row, int col);


} // ipx

#endif // __EVALUATION_H__
