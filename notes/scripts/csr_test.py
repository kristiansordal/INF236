time = 0
threads = 0
flops = 0
with open("csrout.txt") as f:
    for line in f:
        words = line.split()
        if words[0] == "Time:":
            time.append(float(words[1]))
        elif words[0] == "Threads:":
            thread
