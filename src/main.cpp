#include <array>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

const auto BUFSIZE = 1024;

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
    s.erase(s.size()-2);
    
    result.push_back(s);
  }
  return result;
}

template <typename T, typename R> struct CSV {
  vector<T> headers;
  vector<vector<R>> rows;
  static CSV<T, R> readCSV(string path) {
    char buf[BUFSIZE] = {};
    ifstream input_file{path};

    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << path << "'" << endl;
      exit(EXIT_FAILURE);
    }
    vector<char> input;
    while ((!input_file.eof()) && input_file.read(buf, BUFSIZE)) {
      for (auto x : buf) {
        input.push_back(x);
      }
    }

    // close stream
    input_file.close();

    for (auto x : buf) {
      // meet end
      if (x == '\0') {
        break;
      }
      input.push_back(x);
    }

    auto input_len = 0;
    for (auto x : input) {
      if (x == '\n') {
        break;
      }
      input_len++;
    }

    auto header = slicing(input, 0, input_len);
    vector<vector<char>> rows = {};
    while (input_len < input.size()) {
      rows.push_back(slicing_by(input, input_len, '\n'));
      input_len += rows[rows.size() - 1].size();
    }

    rows.erase(rows.begin() + 0);

    CSV<string, string> csv = {};

    for (auto x : rows) {
      csv.rows.push_back(slice_to_strings(x, ','));
    }

    csv.headers = slice_to_strings(header, ',');

    return csv;
  }
  string toJson() {
    string result = "[";
    for (auto row : rows) {
      result.append("{");
      for (auto i = 0; i < headers.size(); i++) {
        result.append("\"" + headers[i] + "\":");
        result.append(row[i] + ",");
      }
      // remove ,
      result.erase(result.size() - 1);
      result.append("},");
    }
    result.erase(result.size() - 1);
    result.append("]");
    return result;
  }
};

/**
for test
*/
int main() {

  auto path = "simple.csv";
  auto jpath = "simple.json";

  CSV<string, string> _csv = CSV<string, string>::readCSV(path);
  for (auto x : _csv.headers) {
    cout << x << " ";
  }
   cout<< " \n";
  for (auto x : _csv.rows) {
    for (auto y : x) {
      cout << y << " ";
    }
    cout << "\n";
  }

  auto _json = _csv.toJson();
  cout << _json;

  ofstream output_file{jpath};

  output_file << _json;

  output_file.close();
  return 0;
}
