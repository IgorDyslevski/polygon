import matplotlib.pyplot as plt
from math import log2


with open('time6') as f:
    data = list(map(lambda x: (int(x.split()[1]), float(x.split()[3])), f.read().strip().split('\n')[:-1]))
x, y = zip(*data)
# ty = list(map(lambda i: y[-1] * ((i * log2(i)) / (x[-1] * log2(x[-1]))), x))
ty = list(map(lambda i: y[-1] * ((i * i) / (x[-1] * x[-1])), x))
print(list(filter(lambda i: i[0] in [10, 20, 100, 200], zip(x, y, ty))))
# Plot the graph
plt.figure(figsize=(10, 6))
plt.plot(x, y, label='Real Time')
plt.plot(x, ty, label='Calculated Time')
plt.title('Execution Time vs. Array Length', fontsize=14)
plt.xlabel('Array Length', fontsize=12)
# plt.xlabel('Array Length | Array Diff: 5000000', fontsize=12)
# plt.ylabel('Time (seconds)', fontsize=12)
plt.grid(True)
plt.legend()
# plt.show()
plt.savefig('plot6.png', dpi=300)
