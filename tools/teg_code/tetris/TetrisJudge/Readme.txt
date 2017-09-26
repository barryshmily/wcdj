TetrisJudge 是比赛评测程序，运行环境为 Linux x86_64。 请大家使用这个评测程序来测试自己的程序。
运行结束后会生成result.txt, 可以打开文件查看整个游戏过程。
如果发现评测程序有任何问题，请反馈给组委会处理。

参数说明：
Usage:
   ./TetrisJudge -p <player program> -o <obstacle program> [-r <result file>]
Options:
   -p  Specify the player program.
   -o  Specify the obstacle program.
   -r  Specify the result file to output. default: 'result.txt'
   -h  Display this information
Sample:
   ./TetrisJudge -p ./part1/run.sh -o ./part2/run.sh


组委会测试时，评测程序当前路径为：SVN下的trunk目录。
