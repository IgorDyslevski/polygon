import numpy as np


def max_same_len(array):
    diffs = np.diff(array)
    edges_of_changes = np.concatenate(([0], np.where(diffs != 0)[0] + 1, [len(array)]))
    diffs_of_edges = np.diff(edges_of_changes)
    return max(diffs_of_edges)

