#ifndef VE281P1_SORT_HPP
#define VE281P1_SORT_HPP

#include <vector>

template <typename T>
void my_swap(T &lhs, T &rhs)
{
    T temp = lhs;
    lhs = rhs;
    rhs = temp;
}

template <typename T, typename Compare = std::less<T>>
void bubble_sort(std::vector<T> &vector, Compare comp = Compare())
{
    // TODO: implement
    int n = (int)vector.size();
    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (comp(vector[j + 1], vector[j]))
                my_swap(vector[j], vector[j + 1]);
        }
    }
}

template <typename T, typename Compare = std::less<T>>
void insertion_sort(std::vector<T> &vector, Compare comp = Compare())
{
    // TODO: implement
    int n = (int)vector.size();
    T tmp;
    for (int i = 1; i < n; ++i)
    {
        tmp = vector[i];
        for (int j = i - 1; j >= 0; --j)
        {
            if (comp(tmp, vector[j]))
            {
                my_swap(vector[j], vector[j + 1]);
            }
            else
                break;
        }
    }
}

template <typename T, typename Compare = std::less<T>>
void selection_sort(std::vector<T> &vector, Compare comp = Compare())
{
    // TODO: implement
    int n = (int)vector.size();
    for (int i = 0; i < n - 1; ++i)
    {
        T minn = vector[i];
        int mini = i;
        for (int j = i; j < n; ++j)
        {
            if (comp(vector[j], minn))
            {
                minn = vector[j];
                mini = j;
            }
        }
        my_swap(vector[i], vector[mini]);
    }
}

template <typename T, typename Compare = std::less<T>>
void merge(std::vector<T> &vector, int left, int mid, int right, Compare comp = Compare())
{
    // Debugger
    // std::cout << left << ' ' << mid << ' ' << right << std::endl;

    std::vector<T> tmp(right - left + 1);
    int i = 0, l = left, r = mid + 1;
    while (l <= mid && r <= right)
    {
        tmp[i++] = !comp(vector[r], vector[l]) ? vector[l++] : vector[r++];
    }
    while (l <= mid)
    {
        tmp[i++] = vector[l++];
    }
    while (r <= right)
    {
        tmp[i++] = vector[r++];
    }
    for (int j = 0; j < (int)tmp.size(); ++j)
    {
        vector[left + j] = tmp[j];
    }

    // Debugger
    // for (auto &x : vector)
    // {
    //     std::cout << x << ' ';
    // }
    // std::cout << std::endl;
}

template <typename T, typename Compare = std::less<T>>
void merge_sort_helper(std::vector<T> &vector, int left, int right, Compare comp = Compare())
{
    // TODO: implement
    if (left >= right)
        return;
    int mid = (left + right) / 2;
    merge_sort_helper(vector, left, mid, comp);
    merge_sort_helper(vector, mid + 1, right, comp);
    merge(vector, left, mid, right, comp);
}

template <typename T, typename Compare = std::less<T>>
void merge_sort(std::vector<T> &vector, Compare comp = Compare())
{
    // TODO: implement
    merge_sort_helper(vector, 0, (int)vector.size() - 1, comp);
}

template <typename T, typename Compare = std::less<T>>
void quick_sort_extra_helper(std::vector<T> &vector, int left, int right, Compare comp = Compare())
{
    // TODO: implement
    int pivotat; // index of the pivot
    if (left >= right)
        return;
    std::vector<T> extra(right - left + 1);
    int rand_index = rand() % (right - left + 1) + left;
    T pivot = vector[rand_index];
    my_swap(vector[rand_index], vector[left]);
    int l = 0, r = right - left;
    for (int i = left + 1; i <= right; ++i)
    {
        if (comp(vector[i], pivot))
            extra[l++] = vector[i];
        else
            extra[r--] = vector[i];
    }
    extra[l] = pivot;
    for (int i = left; i <= right; ++i)
    {
        vector[i] = extra[i - left];
    }
    pivotat = l + left;
    quick_sort_extra_helper(vector, left, pivotat - 1, comp);
    quick_sort_extra_helper(vector, pivotat + 1, right, comp);
}

template <typename T, typename Compare = std::less<T>>
void quick_sort_extra(std::vector<T> &vector, Compare comp = Compare())
{
    // TODO: implement
    srand((int)time(0));
    quick_sort_extra_helper(vector, 0, (int)vector.size() - 1, comp);
}

template <typename T, typename Compare = std::less<T>>
void quick_sort_inplace_helper(std::vector<T> &vector, int left, int right, Compare comp = Compare())
{
    // TODO: implement
    int pivotat; // index of the pivot
    if (left >= right)
        return;

    int i = left + 1, j = right;
    int rand_index = rand() % (right - left + 1) + left;
    T pivot = vector[rand_index];
    my_swap(vector[rand_index], vector[left]);
    while (true)
    {
        while (comp(vector[i], pivot) && i <= right)
            i++;
        while (!comp(vector[j], pivot) && j > left)
            j--;
        if (i < j)
        {
            my_swap(vector[i], vector[j]);
        }
        else
        {
            vector[left] = vector[j];
            vector[j] = pivot;
            break;
        }
    }

    // Debugger
    // std::cout << j << std::endl;

    pivotat = j;
    quick_sort_inplace_helper(vector, left, pivotat - 1, comp);
    quick_sort_inplace_helper(vector, pivotat + 1, right, comp);
}

template <typename T, typename Compare = std::less<T>>
void quick_sort_inplace(std::vector<T> &vector, Compare comp = Compare())
{
    // TODO: implement
    srand((int)time(0));
    quick_sort_inplace_helper(vector, 0, (int)vector.size() - 1, comp);
}

#endif //VE281P1_SORT_HPP
