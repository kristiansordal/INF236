import sys

threads = int(sys.argv[1])
imb_s = 0
imb_n = 0
with open("imb_smart.txt") as f:
    for l in f:
        words = l.split()
        if words[0] == "Diff":
            imb_s = float(words[1])

with open("imb_naive.txt") as f:
    for l in f:
        words = l.split()
        if words[0] == "Diff":
            imb_n = float(words[1])

with open("imb_res.txt", "a") as f:
    f.write(f"{threads} {imb_s} {imb_n}\n")
