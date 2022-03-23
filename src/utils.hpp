#include <array>
#include <string>
#include <vector>

using namespace std;

// slice a vector by a range
template <typename T> vector<T> slicing(vector<T> &arr, int from, int to) {

  auto start = arr.begin() + from;
  auto end = arr.begin() + to + 1;

  vector<T> result(to - from + 1);

  copy(start, end, result.begin());

  return result;
}

// slice a vector by a begin-index and end-value
template <typename T> vector<T> slicing_by(vector<T> &arr, int from, T x) {

  auto count = from;

  for (; count < arr.size(); count++) {
    if (arr[count] == x) {
      break;
    }
  }

  auto start = arr.begin() + from;
  auto end = arr.begin() + count;

  vector<T> result(count - from + 1);

  copy(start, end, result.begin());

  return result;
}

// slice a vector to serials sub-vectors
template <typename T> vector<vector<T>> slice(vector<T> &arr, T slice_point) {

  vector<int> marks = {};
  vector<vector<T>> result = {};
  auto mark = 0;
  marks.push_back(0);
  for (auto i : arr) {
    if (i == slice_point) {
      marks.push_back(mark + 1);
    }
    mark++;
  }
  marks.push_back(arr.size());
  for (auto i = 0; i < marks.size() - 1; i++) {

    auto start = arr.begin() + marks[i];
    auto end = arr.begin() + marks[i + 1] - 1;

    vector<T> ret(marks[i + 1] - marks[i] + 1);

    copy(start, end, ret.begin());
    result.push_back(ret);
  }
  return result;
}

// convert a char vector to string vector
template <typename T>
vector<string> slice_to_strings(vector<T> &arr, T slice_point) {

  vector<int> marks = {};
  vector<string> result = {};
  auto mark = 0;
  marks.push_back(0);
  for (auto i : arr) {
    if (i == slice_point) {
      marks.push_back(mark + 1);
    }
    mark++;
  }
  marks.push_back(arr.size());
  for (auto i = 0; i < marks.size() - 1; i++) {

    auto start = arr.begin() + marks[i];
    auto end = arr.begin() + marks[i + 1] - 1;

    vector<T> ret(marks[i + 1] - marks[i] + 1);

    copy(start, end, ret.begin());
    string s = string(ret.begin(), ret.end());

    // ?
    s.erase(s.size() - 2);

    result.push_back(s);
  }
  return result;
}