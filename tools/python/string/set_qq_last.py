def f2():
    f = open("d:\\o.txt", "w")
    for line in open("d:\\i.txt", "r"):
        print >> f, line[: 4] + "***"
    f.close()

