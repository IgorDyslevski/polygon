import matplotlib.pyplot as plt


with open('box5.1.2') as f:
    data = list(map(lambda x: (int(x.split()[1]), float(x.split()[3])), f.read().strip().split('\n')[:-1:2]))
x, y = zip(*data[50:])
#x, y = zip(*data[len(data) // 2:])
#x, y = zip(*data[:len(data) // 2])
print(x, y)
# Plot the graph
plt.figure(figsize=(6, 6))
plt.boxplot(y)
plt.title('1e5', fontsize=14)
plt.grid(True)
plt.legend()
plt.savefig('boxplot5.1.2.2.png', dpi=300)
