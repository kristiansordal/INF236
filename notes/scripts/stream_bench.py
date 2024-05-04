a = 0
req_threads = 0
got_threads = 0
triad = 0
copy = 0
add = 0
scalar = 0

with open("output.txt") as f:
    for line in f:
        words = line.split()
        if words[0] == "Number" and words[3] == "requested":
            req_threads = int(words[5])
        if words[0] == "Number" and words[3] == "counted":
            got_threads = int(words[5])

        if words[0] == "Copy:":
            copy = float(words[1])
        if words[0] == "Triad:":
            triad = float(words[1])
        if words[0] == "Add:":
            add = float(words[1])
        if words[0] == "Scale:":
            scalar = float(words[1])
f.close()
with open("results.txt", "a") as f:
    f.write(f"{req_threads} {got_threads} {triad} {scalar} {add} {copy}\n")
f.close()
