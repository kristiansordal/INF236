ti_sm, ti_sq, ti_na, ti_sh = 0, 0, 0, 0
tr_sm, tr_sq, tr_na, tr_sh = 0, 0, 0, 0
gf_sm, gf_sq, gf_na, gf_sh = 0, 0, 0, 0
with open("shared_out.txt") as f:
    for line in f:
        words = line.split()

        if words[0] == "ti_sh":
            ti_sh = float(words[1])
        elif words[0] == "tr_sh":
            tr_sh = float(words[1])
        elif words[0] == "gf_sh":
            gf_sh = float(words[1])
f.close()

with open("seq_out.txt") as f:
    for line in f:
        words = line.split()

        if words[0] == "ti_sq":
            ti_sq = float(words[1])
        elif words[0] == "tr_sq":
            tr_sq = float(words[1])
        elif words[0] == "gf_sq":
            gf_sq = float(words[1])
f.close()

with open("smart_out.txt") as f:
    for line in f:
        words = line.split()

        if words[0] == "ti_sm":
            ti_sm = float(words[1])
        elif words[0] == "tr_sm":
            tr_sm = float(words[1])
        elif words[0] == "gf_sm":
            gf_sm = float(words[1])
f.close()

with open("naive_out.txt") as f:
    for line in f:
        words = line.split()

        if words[0] == "ti_na":
            ti_na = float(words[1])
        elif words[0] == "tr_na":
            tr_na = float(words[1])
        elif words[0] == "gf_na":
            gf_na = float(words[1])
f.close()

with open("shared_res.txt", "a") as f:
    f.write(f"{tr_sh} {gf_sh} {ti_sh}")
f.close()
with open("seq_res.txt", "a") as f:
    f.write(f"{tr_sq} {gf_sq} {ti_sq}")
f.close()
with open("smart_res.txt", "a") as f:
    f.write(f"{tr_sm} {gf_sm} {ti_sm}")
f.close()
with open("naive_res.txt", "a") as f:
    f.write(f"{tr_na} {gf_na} {ti_na}")
f.close()
