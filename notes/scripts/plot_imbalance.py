import matplotlib.pyplot as plt
import numpy as np

# Read smart data
smart = []
with open("smart_lb_arrow.txt") as f:
    for line in f:
        x, y = line.split()
        x = int(x) + 1
        y = float(y)
        smart.append((x, y))

# Read naive data
naive = []
with open("naive_lb_arrow.txt") as f:
    for line in f:
        x, y = line.split()
        x = int(x) + 1
        y = float(y)
        naive.append((x, y))

plt.rc("text", usetex=True)
plt.rc("font", family="serif")

# Extract x and y values
smart_x, smart_y = zip(*smart)
naive_x, naive_y = zip(*naive)

# Assuming both lists have the same x values
bar_width = 0.35  # Width of each bar
x_indices = np.arange(len(smart_x))  # Create an array for the bar positions

# Plot the bars side by side
fig, ax = plt.subplots()
rects1 = ax.bar(x_indices - bar_width / 2, smart_y, bar_width, label="Smart", color="Red")
rects2 = ax.bar(x_indices + bar_width / 2, naive_y, bar_width, label="Naive", color="Blue")

# Add labels, title, and legend
ax.set_xlabel("Number of threads")
ax.set_ylabel("Assigned non-zeros [\%]")
ax.set_title("Comparison of Smart and Naive Load balancing strategies")
ax.set_xticks(x_indices)
ax.set_xticklabels(smart_x)
ax.legend()

# Display the plot
plt.show()
