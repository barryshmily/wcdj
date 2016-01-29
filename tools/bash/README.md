#Tips

**(1) Bash shortcuts**

命令 | 作用
:----|:----
CTRL-c |  Stop current command
CTRL-z |  Sleep program
CTRL-a |  Go to start of line
CTRL-e |  Go to end of line 
CTRL-u |  Cut from start of line
CTRL-k |  Cut to end of line
CTRL-r |  Search history
!!     |  Repeat last command
!abc   |  Run last command starting with abc
!abc:p |  Print last command starting with abc
!$     |  Last argument of previous command
!*     |  All arguments of previous command
^abc^123 | Run previous command, replacing abc with 123

**(2) Bash Variables**

环境变量 | 含义
:----|:----
env | Show environment variables
echo $NAME | Output value of $NAME variable
export NAME=value | Set $NAME to value
$PATH | Executable search path
$HOME | Home directory
$SHELL | Current shell

**(3) IO Redirection**

命令 | 作用
:----|:----
cmd < file | Input of cmd from file
cmd1 < cmd2 | Output of cmd2 as file input to cmd1
cmd > file | Standard output (stdout) of cmd to file
cmd > /dev/null | Discard stdout of cmd
cmd >> file | Append stdout to file
cmd 2>file | Error output (stderr) of cmd to file
cmd 1>&2 | stdout to same place as stderr
cmd 2>&1 | stderr to same place as stdout
cmd &>file | every output of cmd to file

**(4) Search Files**

命令 | 作用
:----|:----
grep pattern files | Search for pattern in files
grep -i | Case insensitive search
grep -r | Recursive search
grep -v | Inverted search
grep -o | Show matched part of file only
find /dir/ -name name* | Find files starting with name in dir
find /dir/ -user name | Find files owned by name in dir name
find /dir/ -mmin num | Find files modified less than num minutes ago in dir
whereis command | Find binary/source/manual for command
locate file | Find file (quick search of system index)

**(5) File Operations**

命令 | 作用
:----|:----
touch file1 | Create file1
cat file1 file2 | Concatenate files and output
less file1 | View and paginate file1
file file1 | Get type of file1
cp file1 file2 | Copy file1 to file2
mv file1 file2 | Move file1 to file2
rm file1 | Delete file1
head file1 | Show first 10 lines of file1
tail file1 | Show last 10 lines of file1
tail -f file1 | Output last lines of file1 as it changes

**(6) Pipes**

命令 | 作用
:----|:----
cmd1 \| cmd2 | stdout of cmd1 to cmd2
cmd1 \|& cmd2 | stderr of cmd1 to cmd2

**(7) Command Lists**

命令 | 作用
:----|:----
cmd1;cmd2 | Run cmd1 then cmd2
cmd1 && cmd2 | Run cmd2 if cmd1 is successful
cmd1 \|\| cmd2 | Run cmd2 if cmd1 is not successful
cmd& | Run cmd in a subshell


*Refer*

[http://cheatography.com/davechild/cheat-sheets/linux-command-line/](http://cheatography.com/davechild/cheat-sheets/linux-command-line/)




















