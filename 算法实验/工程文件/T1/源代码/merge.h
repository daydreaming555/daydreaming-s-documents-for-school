#pragma once
#include <vector>

// 合并两个有序数组
std::vector<int> mergeTwo(const std::vector<int>& a, const std::vector<int>& b);

// 分治法合并K个有序数组
std::vector<int> mergeKDivide(const std::vector<std::vector<int>>& arrays, int left, int right);

// 堆方法合并K个有序数组
std::vector<int> mergeKHeap(const std::vector<std::vector<int>>& arrays);