#ifndef __DEFS_H__
#define __DEFS_H__
namespace ipx {

const int BOARD_ROW = 32;
const int BOARD_COL = 14;

const int SHAPE_BOX = 4;

const int GAME_ROW = 30;
const int GAME_COL = 12;

const int SHAPE_COUNT = 7;
const int MAX_SHAPE = 4;

const int SENTINEL = -999999999;



#define __LOG(format, ...) \
	do{ \
		char __log_buff__[1024] = {0}; \
		snprintf(__log_buff__, sizeof(__log_buff__), format, ##__VA_ARGS__); \
		FILE* __fp=fopen("gerry.log","w+"); \
		fwrite(__log_buff__, strlen(__log_buff__), 1 ,__fp ); \
		fclose(__fp); \
	} while(0)

#define __LOG2(format, ...) \
	do{ \
		char __log_buff__[2048] = {0}; \
		snprintf(__log_buff__, sizeof(__log_buff__), "[%s %d]" format, __FILE__, __LINE__, ##__VA_ARGS__); \
		FILE* __fp=fopen("gerry.log","w+"); \
		fwrite(__log_buff__, strlen(__log_buff__), 1 ,__fp ); \
		fclose(__fp); \
	} while(0)

typedef enum _shape_t
{
	S_NULL = 0,
	S_S = 1,
	S_Z = 2,
	S_L = 3,
	S_J = 4,
	S_I = 5,
	S_O = 6,
	S_T = 7,
	S_B // 边界特殊类型

} shape_t;

enum PIECE_STATUS
{
	S_UNOCCUPIED = 0, // .
	S_OCCUPIED = 1    // #
};

// 游戏区域
typedef struct _russia_game_t
{
	int board[BOARD_ROW][BOARD_COL];  // 14 * 32
	int top_row;                      // 当前最高行所在的位置, 作为约束
	int old_top_row;
	int score;                        // 当前得分
	int lines;                        // 当前消除的行数

} russia_game_t;

// 每种具体板块形态的4*4矩阵定义
// shape中值1表示对应的小方格是板块形态的有效格子，而0表示无效
typedef struct _piece_shape_t
{
	int r_index;
	int shape[SHAPE_BOX][SHAPE_BOX];   // 4 * 4
	int width;                         // 宽度
	int height;                        // 高度

	int init_left_col;   // 初始方格最左边列位置


} piece_shape_t;

// 定义和存储一个板块的所有形态信息
typedef struct _piece_all_shape_t
{
	piece_shape_t r_shape[MAX_SHAPE];  // 合理的旋转形态
	int r_count;                       // 板块旋转时可能产生的不同形态的个数

} piece_all_shape_t;

typedef struct _evaluate_result_t
{
	int r_index;
	int row;
	int col;
	int value;
	int prs;

} evaluate_result_t;

} // ipx

#endif // __DEFS_H__
