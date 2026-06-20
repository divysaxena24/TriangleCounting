import matplotlib.pyplot as plt

r = []
cover = []

with open("graph_data.txt") as f:
    for line in f:
        x, y = map(int, line.split())
        r.append(x)
        cover.append(y)

plt.figure(figsize=(8,5))

plt.plot(
    r,
    cover,
    marker='o'
)

plt.xlabel("Number of BFS Runs (r)")
plt.ylabel("Cover Set Size")
plt.title("Cover Set Size vs BFS Runs")

plt.grid(True)

plt.show()