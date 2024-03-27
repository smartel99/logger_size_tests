import argparse
import matplotlib.pyplot as plt
import pandas as pd


def main():
    parser = argparse.ArgumentParser(prog="graph_from_csv", description="")
    parser.add_argument("csv")
    parser.add_argument("out_path")
    args = parser.parse_args()

    data = pd.read_csv(args.csv)

    iterations = [x for x in data['iterations']]
    sets = {k: [e for e in v] for k, v in data.items() if k != 'iterations'}

    fig, ax = plt.subplots()
    ax.set_title(args.csv.split('/')[-1])
    ax.set_xlabel("Instantiations")
    ax.set_ylabel("Size of the Binary (bytes)")

    for name, test in sets.items():
        ax.plot(iterations, test, label=name)

    ax.legend()
    plt.savefig(args.out_path)


if __name__ == "__main__":
    main()
