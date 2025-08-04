#include <vector>



template<typename T>
std::vector<T> insertionSort(const std::vector<T>& original) {
    // type T must be a class with a member variable "int order"
    std::vector<T> result = original;

    for (int i = 0; i < original.size(); i++) {
        int max = 0;
        if (original[i].order >= max) {
            max = original[i].order;
        } else {
            int j = 0;
            while (j < i && original[j].order < original[i].order) {
                result[j] = original[i];
            }
        }
    }
    return result;
}