// HLP2: word-count-ver1.cpp
// 01/26/2022
// We'd like a program similar to the Linux program wc. Just like wc, our
// program should print the line, word, and byte count of a text file.
// The program can be easily implemented using std::string.
// The algorithm's outline is:
// 1) open input text file
// 2) initialize counters line_cnt, word_cnt, byte_cnt to zero
// 3) define variable line of type std::string
// 3) use std::getline to read each line of text into line
//      increment line_cnt [because we've have a new line]
//      add line.size()+1 to byte_cnt [because getline reads the line and discards '\n' character]
//      use a function line_splitter to find the word count in current line
//      add this word count to word_cnt
// 4) write values in line_cnt, word_cnt, and byte_cnt to standard output

#include <string>
#include <iostream>
#include <fstream>

// could have used std::isspace() declared in <cctype>
// however, it is good for student's to continually practice the
// implementation of simple functions to improve their skills ...
bool is_whitespace(char ch) {
  return (ch==' ' || ch=='\t' || ch=='\n' || ch=='\f' || ch=='\v' || ch=='\t') ? true : false;
}

// this is just the logical negation of is_whitespace() ...
bool is_not_whitespace(char ch) {
  return !is_whitespace(ch);
}

// Function that returns the number of "words" in a line of text where
// a "word" is delimited by following ASCII whitespace characters: ' ',
// '\t', '\n', '\r' [carriage return], '\v' [vertical tab], '\f' [formfeed] ...
// The function will define two indices i and j, that will delimit each "word" in turn.
// The idea is that we'll locate a "word" by computing values for i and j such
// that the "word" will be the characters in the range [i, j).
// Once we've these indices, we know that a "word" exists and the word counter
// is incremented.
int line_splitter(std::string const& line) {
  int word_cnt{};
  std::string::size_type i{0};
  while (i < line.size()) {
    // ignore leading whitespace characters ...
    while (i < line.size() && is_whitespace(line[i])) { ++i; }
    std::string::size_type j {i};
    // find end of next word ...
    while (j < line.size() && is_not_whitespace(line[j])) { ++j; }
    // if we found some non-whitespace characters, we've a new "word"
    if (i != j) {
      ++word_cnt;
      i = j; // don't forget this: we want to continue repeating the algorith
             // to find the remaining "words" in this string ...
    }
  }
  return word_cnt;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Usage: ./a.out input-text-file-name\n";
    return 0;
  }

  // associate file to read from with an input file stream ifs
  std::ifstream ifs {argv[1]};
  if (!ifs) {
    std::cout << "Unable to open input file " << argv[1] << "\n";
    return 0;
  }

  /*
  Put this from a more introductory program to reinforce how getline works:s
  The getline function takes an input stream (note that an input file stream
  is also an input stream) and a string. This function reads the given stream
  up to and including the first newline and stores what it read - not including
  the newline - in its string argument. After getline sees a newline, even if
  it is the first character in the input, it stops reading and returns. If the
  first character in the input is a newline, then the resulting string is the
  empty string.
  Like the input operator, getline returns an istream (but remember that ifstream
  is-an istream too). As a result, we can use getline as a condition. Why?
  Because expression getline(ifs, line) evaluates to reference to ifstream.
  Therefore, getline(ifs, line) tests ifs. When we use an ifstream as a condition,
  the effect is to test the state of the stream. If the stream is valid - that is,
  if the stream hasn't encountered an error - then the test succeeds. An ifstream
  becomes invalid when it hits the end-of-file. An ifstream that is in an invalid
  state will cause the condition to yield false.
  */
  int line_cnt{}, word_cnt{}, byte_cnt{};
  std::string line;
  while (getline(ifs, line)) {
    ++line_cnt; // because we've a new line
    word_cnt += line_splitter(line);
    byte_cnt += line.size() + 1; // because getline discards the '\n' character it reads from file
  }
  // imitating the output presentation of linux program wc ...
  std::cout << "  " << line_cnt << "  " << word_cnt << " " << byte_cnt
            << " " << argv[1] << '\n';
  ifs.close();
}
