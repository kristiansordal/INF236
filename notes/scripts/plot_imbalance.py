import matplotlib.pyplot as plt
import numpy as np
from sys import argv

# Read data from file
xs, sm, na = [], [], []
with open("imb_res.txt") as file:
    for line in file:
        x, smart, naive = map(float, line.split())
        xs.append(int(x))
        sm.append(smart)
        na.append(naive)

# Settings for using LaTeX and serif font
plt.rc("text", usetex=True)
plt.rc("font", family="serif")

# Set the width of the bars
bar_width = 0.35

# Set positions of the bars
r1 = np.arange(len(sm))
r2 = [x + bar_width for x in r1]

fig, ax = plt.subplots(figsize=(10, 6))

# Create bars
plt.bar(r1, sm, width=bar_width, color="b", label="METIS")
plt.bar(r2, na, width=bar_width, color="r", label="Greedy")

# Add xticks on the middle of the group bars
plt.xlabel("Number of threads")
plt.ylabel("Max \(\Delta\) imabalance [\%]")
plt.xticks([r + bar_width / 2 for r in range(len(sm))], xs)


# Create legend & Show graphic
plt.legend()

plt.savefig(argv[1], dpi=1200)
plt.show()
