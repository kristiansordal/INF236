import matplotlib.pyplot as plt
import numpy as np


# Function to read data from a file
def read_data_file(filename):
    return np.loadtxt(filename, delimiter=" ", usecols=(0, 1)).T


# Read Triad data specifically
def read_triad_data(filename):
    x, values = [], []
    with open(filename) as f:
        for line in f:
            words = line.split()
            x.append(int(words[1]))
            values.append((float(words[2]) * 1.33) / 1024 / 12)
    return np.array(x), np.array(values)


def main():
    plt.rc("text", usetex=True)
    plt.rc("font", family="serif")
    # Load Triad data
    triad_x, triad_values = read_triad_data("stream_res.txt")

    # Fit a polynomial (degree 4 for simplicity)
    degree = 4
    coefficients = np.polyfit(triad_x, triad_values, degree)
    polynomial_fit = np.poly1d(coefficients)

    # Generate the fitted curve values
    x_fit = np.linspace(min(triad_x), max(triad_x), 100)
    y_fit = polynomial_fit(x_fit)

    # Load data from other files
    datasets = {
        # "mycielskian [Smart]": read_data_file("smart_res_mycielskian.txt"),
        # "mycielskian [Naive]": read_data_file("naive_res_mycielskian.txt"),
        # "mycielskian [Shared]": read_data_file("shared_res_mycielskian.txt"),
        # "circuit5M [Smart]": read_data_file("smart_res_5m.txt"),
        # "circuit5M [Naive]": read_data_file("naive_res_5m.txt"),
        # "circuit5M [Shared]": read_data_file("shared_res_5m.txt"),
        "arrow [Smart]": read_data_file("smart_res_arrow.txt"),
        "arrow [Naive]": read_data_file("naive_res_arrow.txt"),
        "arrow [Shared]": read_data_file("shared_res_arrow.txt"),
    }

    # Prepare the plot
    fig, ax = plt.subplots(figsize=(10, 6))

    # Plot Triad data and its line of best fit
    ax.plot(triad_x, triad_values, marker="*", linestyle="-", label="Triad", color="blue")
    ax.plot(x_fit, y_fit, "--", color="orange", label="Best Fit (Triad)")

    # Plot other datasets
    # markers = ["o", "v", "^"]
    colors = ["red", "green", "yellow", "orange", "pink", "purple"]
    for (label, (x_data, y_data)), color in zip(datasets.items(), colors):
        ax.plot(x_data, y_data, linestyle="-", label=label, color=color)

    # Customize the plot
    ax.set_xlabel("Number of Threads")
    ax.set_ylabel("GFLOPS")
    ax.set_title("Performance Analysis")
    ax.legend(loc="upper left")
    ax.grid(visible=True, linestyle="--", alpha=0.7)

    # Final adjustments
    ax.set_xlim(min(triad_x), max(triad_x))
    ax.set_ylim(0, max(triad_values.max(), max(max(y) for _, y in datasets.values())))

    plt.show()


main()
