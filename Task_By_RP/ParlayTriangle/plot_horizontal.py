import matplotlib.pyplot as plt

r = int(input("Enter r value: "))

file_name = f"horizontal_data_r_{r}.txt"

horizontal_count = []
num_edges = []

with open(file_name) as f:
    for line in f:
        x, y = map(int, line.split())

        horizontal_count.append(x)
        num_edges.append(y)

plt.figure(figsize=(10,5))

plt.bar(
    horizontal_count,
    num_edges
)

plt.xlabel(
    "Times Edge Appeared Horizontal"
)

plt.ylabel(
    "Number of Edges"
)

plt.title(
    f"Horizontal Edge Distribution (r={r})"
)

plt.grid(True)

plt.show()