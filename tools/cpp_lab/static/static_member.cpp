#include<stdio.h>
#include "GamePlayer.h"

//const int GamePlayer::NumTurns;   // NumTurns的定义

#if 0
class GamePlayer {

public:
	void set_scores() {
		for (int i = 0; i != NumTurns; ++i) {
			scores[i] = i;
		}
	}
	void get_scores() {
		for (int i = 0; i != NumTurns; ++i) {
			printf("%d ", scores[i]);
		}
		printf("\n");
	}

	static int get_numturns() {
		printf("addr GamePlayer::NumTurns[%p]\n", &GamePlayer::NumTurns);
		return GamePlayer::NumTurns;
	}

private:
	static const int NumTurns = 5;       // 常量声明
	int scores[NumTurns];                // 使用该常量
};
#endif

int main()
{
	printf("GamePlayer::NumTurns[%d]\n", GamePlayer::get_numturns());
	//printf("NumTurns[%d]\n", GamePlayer::NumTurns);// error

	GamePlayer player;
	player.set_scores();
	player.get_scores();

	GamePlayer player2;
	printf("player.NumTurns[%d] player2.NumTurns[%d]\n", 
			player.get_numturns(), player2.get_numturns());

	return 0;
}
/*
addr GamePlayer::NumTurns[0x102092f30]
GamePlayer::NumTurns[5]
0 1 2 3 4 
addr GamePlayer::NumTurns[0x102092f30]
addr GamePlayer::NumTurns[0x102092f30]
player.NumTurns[5] player2.NumTurns[5]
*/
