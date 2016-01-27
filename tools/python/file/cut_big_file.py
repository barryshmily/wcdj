import sys

chunk_size = 3500000  # 按行数分割
fout = None
filename = sys.argv[1] # 要分割的文件
f = open(filename,'r')


for (i, line) in enumerate(f.readlines()):
    if i % chunk_size == 0:
        if fout:
            fout.close()
        fout = open('outfile%d.txt' % (i/chunk_size), 'w')
    fout.write(line)
fout.close()

