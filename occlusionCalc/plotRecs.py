import matplotlib.pyplot as plt

def read_rectangles(filename):
    rects = []
    with open(filename, "r") as f:
        for line in f:
            if line.startswith("Rectangle corners:"):
                parts = line.split(":")[1].strip().split(") ")
                corners = []
                for p in parts:
                    if "(" in p:
                        x, y = p.strip("() ").split(",")
                        corners.append((float(x), float(y)))
                rects.append(corners)
    return rects

rects = read_rectangles("rects.txt")

# plot
plt.figure(figsize=(6,6))
for corners in rects:
    xs = [p[0] for p in corners] + [corners[0][0]]
    ys = [p[1] for p in corners] + [corners[0][1]]
    plt.plot(xs, ys, marker="o")

# plot the origin (observer point)
plt.plot(0, 0, "r*", markersize=15)

plt.axhline(0, color="black", linewidth=0.5)
plt.axvline(0, color="black", linewidth=0.5)
plt.gca().set_aspect("equal", adjustable="box")
plt.title("Random Rectangles and Origin")
plt.show()
