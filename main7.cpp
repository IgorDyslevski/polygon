#include <iostream>
#include <vector>
#include <random>
#include <ctime>

using namespace std;


void quick_sort(std::vector<long long int>& arr, long long int left, long long int right) {
    if (left >= right) return;

    long long int pivot = arr[(left + right) / 2];
    long long int i = left, j = right;

    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;

        if (i <= j) {
            std::swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }
    quick_sort(arr, left, j);
    quick_sort(arr, i, right);
}


int put_marbles(std::vector<long long int>& weights, long long int k) {
    long long int n = weights.size();
    vector<long long int> sums(n - 1);
    for (long long int i = 0; i < n - 1; i++) {
        sums[i] = weights[i + 1] + weights[i];
    }
    quick_sort(sums, 0, sums.size() - 1);
    if (k == 1) {
        return 0;
    }
    else {
        long long int min_sum = 0, max_sum = 0;
        for (long long int i = 0; i < k - 1; i++) {
            min_sum += sums[i];
        }
        for (long long int i = n - 1 - (k - 1); i < sums.size(); i++) {
            max_sum += sums[i];
        }
        return max_sum - min_sum;
    }
}

void test_time(int len) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<long long int> arr(len, 0);
    for (int i = 0; i < arr.size(); i++) {
        arr[i] = gen();
    }
    long long int k = gen() % len;
    clock_t start = clock();
    put_marbles(arr, k);
    clock_t end = clock();
    std::cout << "Time: " << (long double)(end - start) / (long double)(CLOCKS_PER_SEC) << "\n";
}

void test() {
    vector<long long int> arr = {1,3,5,1};
    long long int result = put_marbles(arr, 2);
    
    if (result == 4) {
        std::cout << "OK\n";
    } else {
        std::cout << "Test Failed\n";
    }

    arr = {1,3};
    result = put_marbles(arr, 2);
    
    if (result == 0) {
        std::cout << "OK\n";
    } else {
        std::cout << "Test Failed\n";
    }
}

int main() {
    test();
    return 0;
}
