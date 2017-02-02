
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
		//printf("addr GamePlayer::NumTurns[%p]\n", &GamePlayer::NumTurns);
		return GamePlayer::NumTurns;
	}

private:
	//static const int NumTurns = 5;   // 常量声明
#if 1
	enum {
		NumTurns = 5,    // 令NumTurns成为5的一个记号名称
	};
#endif
	int scores[NumTurns];            // 使用该常量
};
