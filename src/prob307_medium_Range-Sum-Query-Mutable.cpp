// prob307： Range Sum Query - Mutable

/*
 * Given an integer array nums, find the sum of the elements between indices i and j (i ≤ j), inclusive.
 * The update(i, val) function modifies nums by updating the element at index i to val.
 */

/*
 * Example:
 * Given nums = [1, 3, 5]
 * sumRange(0, 2) -> 9
 * update(1, 2)
 * sumRange(0, 2) -> 8
 */

/*
 * Note:
 * The array is only modifiable by the update function.
 * You may assume the number of calls to update and sumRange function is distributed evenly.
 */

/**
 * Your NumArray object will be instantiated and called as such:
 * NumArray* obj = new NumArray(nums);
 * obj->update(i,val);
 * int param_2 = obj->sumRange(i,j);
 */

#include <vector>

using namespace std;

// 线段树模板写法
struct STNode {
    int start;
    int end;
    int sum;
    STNode *left;
    STNode *right;
    STNode(){}
    STNode(int start, int end, int sum)
        :start(start),end(end),sum(sum),left(nullptr),right(nullptr){}
    STNode(int start, int end, int sum, STNode *left, STNode *right)
        :start(start),end(end),sum(sum),left(left),right(right){}
    ~STNode()
    {
        if(left)
        {
            delete left;
            left = nullptr;
        }
        if(right)
        {
            delete right;
            right = nullptr;
        }
    }
};

class SegmentTree {
public:
    SegmentTree(){
        root = nullptr;
    }

    ~SegmentTree()
    {
        if(root)
        {
            delete root;
            root = nullptr;
        }
    }

    void build(int start, int end, const vector<int>& vals)
    {
        if(end >= start)
            root = _build(start, end, vals);
    }

    void point_update(int index, int val)
    {
        _point_update(root, index, val);
    }

    int range_query(int i, int j)
    {
        return _range_query(root, i, j);
    }

private:
    STNode *root;

    STNode* _build(int start, int end, const vector<int>& vals)
    {
        if(start == end)
            return new STNode(start, end, vals[start]);
        int mid = start + (end - start) / 2;
        STNode *left = _build(start, mid, vals);
        STNode *right = _build(mid + 1, end, vals);
        return new STNode(start, end, left -> sum + right -> sum, left, right);
    }

    void _point_update(STNode *root, int index, int val)
    {
        if(root -> start == root -> end && root -> end == index)
        {
            root -> sum = val;
            return;
        }
        int mid = root -> start + (root -> end - root -> start) / 2;
        if(index <= mid) // 更新位置在左子树
        {
            _point_update(root -> left, index, val);
        }
        else
        {
            _point_update(root -> right, index, val);
        }
        root -> sum = root -> left -> sum + root -> right -> sum;
    }

    int _range_query(STNode *root, int i, int j)
    {
        if(root -> start == i && root -> end == j)
            return root -> sum;
        int mid = root -> start + (root -> end - root -> start) / 2;
        if(j <= mid) // 查询的区间在左子树
            return _range_query(root -> left, i, j);
        else if(i > mid) // 查询的区间在右子树
            return _range_query(root -> right, i, j);
        else
            return _range_query(root -> left, i, mid) + _range_query(root -> right, mid + 1, j);
    }
};

class NumArray {
public:
    NumArray(vector<int>& nums) {
        int n = nums.size();
        int start = 0, end = n - 1;
        sttree = SegmentTree();
        sttree.build(start, end, nums);
    }

    void update(int i, int val) {
        sttree.point_update(i, val);
    }

    int sumRange(int i, int j) {
        return sttree.range_query(i, j);
    }

private:
    SegmentTree sttree;
};


// 树状数组实现, 36ms
class BIT {
public:
    BIT():sums(1, 0){}
    BIT(int n):sums(n + 1, 0){}

    void update(int index, int delta)
    {
        int n = sums.size();
        while(index < n)
        {
            sums[index] += delta;
            index += _lowbit(index);
        }
    }

    int query(int i)
    {
        int sum = 0;
        while(i > 0)
        {
            sum += sums[i];
            i -= _lowbit(i);
        }
        return sum;
    }

private:
    vector<int> sums;

    int _lowbit(int x)
    {
        return x & (-x);
    }
};

class NumArray_2 {
public:
    // vec初始化完守护 nums.size() 就变为0了
    NumArray_2(vector<int> nums):vec(move(nums)),bit(vec.size()) {
        int n = vec.size();
        for(int i = 0; i < n; ++i)
            bit.update(i + 1, vec[i]);
    }

    void update(int i, int val) {
        bit.update(i + 1, val - vec[i]);
        vec[i] = val;
    }

    int sumRange(int i, int j) {
        return bit.query(j + 1) - bit.query(i);
    }

private:
    vector<int> vec;
    BIT bit;
};
