#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "utils.hpp"

using namespace std;

const auto BUFSIZE = 1024;


template <typename T, typename R> struct CSV {
  vector<T> headers;
  vector<vector<R>> rows;
  CSV<T, R>(string _input) {

    auto input_len = _input.size();
    auto input = std::vector<char>(_input.begin(), _input.end());

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

  }
  CSV<T,R>(){}
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
  cout << " \n";
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
