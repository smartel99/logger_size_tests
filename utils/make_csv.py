import csv
from pathlib import Path
import argparse


def main():
    parser = argparse.ArgumentParser(prog="make_csv", description="Makes a csv file containing the size of all the bin files found")
    parser.add_argument("dir")
    parser.add_argument("out_path")
    args = parser.parse_args()

    print(f"Searching in {str(args.dir)}")

    tests = ['iterations']
    results = {}

    files = Path(str(args.dir)).glob('**/*.bin')
    for file in files:
        size = file.stat().st_size
        file = file.relative_to(args.dir)
        test_name, file = file.parts
        if test_name not in tests:
            tests.append(test_name)
        bin = file.split('-')[1].split('.')[0]
        if bin not in results:
            results[bin] = {}
        results[bin][test_name] = size


    print(f"Found {len(tests)} tests: {tests}")

    rows = []
    for k, v in results.items():
        rows.append([k] + [v[x] for x in tests if x != "iterations"])

    rows.sort(key=lambda value: int(value[0]))
    for row in rows:
        print(row)

    with open(args.out_path, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(tests)
        writer.writerows(rows)

if __name__ == "__main__":
    main()
