import matplotlib.pyplot as plt


def main():
    x = []
    scale, add, copy, triad = [], [], [], []
    with open("stream_res.txt") as f:
        for l in f:
            words = l.split()
            x.append(int(words[1]))
            # triad.append((float(words[2]) / 1024) / 12)
            # scale.append((float(words[3]) / 1024) / 12)
            # add.append((float(words[4]) / 1024) / 12)
            # copy.append((float(words[5]) / 1024) / 12)
            triad.append(((float(words[2]) * 1.33) / 1024) / 12)
    plt.ylabel("Stream GFLOPS")
    plt.xlabel("Number of threads")
    plt.plot(x, triad, label="Triad")
    # plt.plot(x, scale, label="Scale")
    # plt.plot(x, add, label="Add")
    # plt.plot(x, copy, label="Copy")
    plt.legend(loc="upper left")
    plt.show()


main()
