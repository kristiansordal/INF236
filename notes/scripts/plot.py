import matplotlib.pyplot as plt
import numpy as np
from sys import argv


# Function to read data from a file
def read_data_file(filename):
    return np.loadtxt(filename, delimiter=" ", usecols=(0, 1)).T


# Read Triad data specifically
def read_triad_data(filename):
    x, values, gfs = [], [], []
    with open(filename) as f:
        a = True
        for line in f:
            words = line.split()
            if a:
                x.append(int(words[1]))
                values.append((float(words[2])) / 1024)  # Convert to GFLOPS
            else:
                gfs.append((float(words[2])))
            a = not a
    return np.array(x), np.array(values), np.array(gfs)


def main():
    plt.rc("text", usetex=True)
    plt.rc("font", family="serif")
    triad_x, triad_values, triad_gf = read_triad_data("stream_res.txt")
    triad_gf = [2 * 10000000 / (x * 10**9) for x in triad_gf]

    degree = 4
    coefficients_gf = np.polyfit(triad_x, triad_gf, degree)
    coefficients_gbs = np.polyfit(triad_x, triad_values, degree)
    # polynomial_fit = np.poly1d(coefficients_gbs)
    polynomial_fit = np.poly1d(coefficients_gf)

    # Generate the fitted curve values
    x_fit_gf = np.linspace(min(triad_x), max(triad_x), 100)
    y_fit = polynomial_fit(x_fit_gf)

    # Load data from other files and uncomment datasets
    datasets = {
        # "mycielskian [Smart]": read_data_file("smart_res_mycielskian.txt"),
        # "mycielskian [Naive]": read_data_file("naive_res_mycielskian.txt"),
        # "mycielskian [Shared]": read_data_file("shared_res_mycielskian.txt"),
        # "circuit5M [Smart]": read_data_file("smart_res_5m.txt"),
        # "circuit5M [Naive]": read_data_file("naive_res_5m.txt"),
        # "circuit5M [Shared]": read_data_file("shared_res_5m.txt"),
        "arrow [Smart]": read_data_file("smart_res_arrow2.txt"),
        "arrow [Naive]": read_data_file("naive_res_arrow.txt"),
        "arrow [Shared]": read_data_file("shared_res_arrow.txt"),
    }

    # Prepare the plot
    fig, ax = plt.subplots(figsize=(10, 6))

    # Plot Triad data and its line of best fit
    # ax.plot(triad_x, triad_values, linestyle="-", label="Triad", color="blue")
    ax.plot(triad_x, triad_gf, linestyle="-", label="Triad", color="blue")
    ax.plot(x_fit_gf, y_fit, "--", color="orange", label="Line of best fit")

    # Plot additional datasets
    for label, (x, values) in datasets.items():
        ax.plot(x, values, linestyle="-", label=label)

    # Customize the plot
    ax.set_xlabel("Number of Threads")
    ax.set_ylabel("GFLOPS")
    ax.legend(loc="best")
    ax.grid(visible=True, linestyle="--", alpha=0.7)

    # Final adjustments
    ax.set_xlim(min(triad_x), max(triad_x))
    y_lim = max(triad_gf) + 0.1
    ax.set_ylim(0, y_lim)

    # Save the plot to a file
    plt.savefig(argv[1], dpi=1200)

    # Show the plot
    plt.show()


if __name__ == "__main__":
    main()