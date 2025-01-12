import numpy as np
import datetime


def max_same_len_numpy(array):
    diffs = np.diff(array)
    edges_of_changes = np.concatenate(([0], np.where(diffs != 0)[0] + 1, [len(array)]))
    diffs_of_edges = np.diff(edges_of_changes)
    return max(diffs_of_edges)

def max_same_len_python(array):
    mx = 0
    curr = 1
    for i in range(1, len(array)):
        if array[i] == array[i - 1]:
            curr += 1
        else:
            mx = max(mx, curr)
            curr = 1
    return max(mx, curr)


def test(length):
    array = np.random.randint(0, 10, length)
    time1 = datetime.datetime.now()
    res1 = max_same_len_numpy(array)
    time2 = datetime.datetime.now()
    res2 = max_same_len_python(array)
    time3 = datetime.datetime.now()
    print(time2 - time1, time3 - time2)
    assert res1 == res2


test(100000)
test(1000000)
test(10000000)

# numpy быстрее стандартного Python

