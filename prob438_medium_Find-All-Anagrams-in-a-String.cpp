// prob438: Find All Anagrams in a String

/*
 * Given a string s and a non-empty string p, find all the start indices of p's anagrams in s.
 * Strings consists of lowercase English letters only and the length of both strings s and p will not be larger than 20,100.
 * The order of output does not matter.
 */

/*
 * Example 1:
 *
 * Input:
 * s: "cbaebabacd" p: "abc"
 *
 * Output:
 * [0, 6]
 */

/*
 * Example 2:
 * Input:
 * s: "abab" p: "ab"
 * Output:
 * [0, 1, 2]
 */

#include <string>
#include <vector>

using namespace std;

// 字符计数
class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        if(s.empty()) return vector<int>();
        int n = s.size(), m = p.size();
        if(n < m) return vector<int>();
        // n >= m
        vector<int> result;
        vector<int> pattern_key = _build_key(p, 0, m - 1); // 传入闭区间
        vector<int> key = _build_key(s, 0, m - 1); // 传入闭区间
        if(_compare_key(pattern_key, key))
            result.push_back(0);
        for(int i = 1; i <= n - m; ++i)
        {
            //
            _modify_key(s, key, i, m);
            if(_compare_key(pattern_key, key))
                result.push_back(i);
        }
        return result;
    }

private:
    vector<int> _build_key(const string& s, int left, int right)
    {
        // 调用方保证 left 和 right 的合法性
        vector<int> result(26, 0);
        for(int i = left; i <= right; ++i)
            ++result[s[i] - 'a'];
        return result;
    }

    void _modify_key(const string& s, vector<int>& key, int idx, int m)
    {
        int to_be_removed_idx = idx - 1;
        int new_idx = idx + m - 1;
        --key[s[to_be_removed_idx] - 'a'];
        ++key[s[new_idx] - 'a'];
    }

    bool _compare_key(const vector<int>& key1, const vector<int>& key2)
    {
        int n = key1.size(), m = key2.size();
        if(n != m) return false;
        for(int i = 0; i < n; ++i)
        {
            if(key1[i] != key2[i])
                return false;
        }
        return true;
    }
};

// 字符串哈希, 本题不太适用
// 因为要输出所有答案，冲突太多，会重建很多次滑窗的 key
class Solution_2 {
public:
    vector<int> findAnagrams(string s, string p) {
        if(s.empty()) return vector<int>();
        int n = s.size(), m = p.size();
        if(n < m) return vector<int>();
        // n >= m
        vector<int> result;
        vector<int> pattern_key = _build_key(p, 0, m - 1); // 传入闭区间
        int pattern_hashcode = _build_hashcode(p, 0, m - 1); // 传入闭区间
        int hashcode = _build_hashcode(s, 0, m - 1); // 传入闭区间
        if(pattern_hashcode == hashcode && _compare(pattern_key, s, 0, m - 1)) // 传入闭区间
            result.push_back(0);
        for(int i = 1; i <= n - m; ++i)
        {
            _modify_hashcode(s, hashcode, i, m);
            if(pattern_hashcode == hashcode && _compare(pattern_key, s, i, m - 1 + i)) // 传入闭区间
                result.push_back(i);
        }
        return result;
    }

private:
    int _build_hashcode(const string& s, int left, int right)
    {
        // 调用方保证 left 和 right 的合法性
        int result = 0;
        for(int i = left; i <= right; ++i)
            result += s[i] - 'a';
        return result;
    }

    void _modify_hashcode(const string& s, int& hashcode, int idx, int m)
    {
        int to_be_removed_idx = idx - 1;
        int new_idx = idx + m - 1;
        hashcode -= s[to_be_removed_idx];
        hashcode += s[new_idx];
    }

    bool _compare(const vector<int>& pattern_key, const string& s, int left, int right)
    {
        // pattern_key -- 模式串的字符计数，s -- 滑窗所在串，[left, right] -- 滑窗在s中的区间
        vector<int> key = _build_key(s, left, right);
        return _compare_key(pattern_key, key);
    }

    bool _compare_key(const vector<int>& key1, const vector<int>& key2)
    {
        int n = key1.size(), m = key2.size();
        if(n != m) return false;
        for(int i = 0; i < n; ++i)
        {
            if(key1[i] != key2[i])
                return false;
        }
        return true;
    }

    vector<int> _build_key(const string& s, int left, int right)
    {
        // 调用方保证 left 和 right 的合法性
        vector<int> result(26, 0);
        for(int i = left; i <= right; ++i)
            ++result[s[i] - 'a'];
        return result;
    }
};
