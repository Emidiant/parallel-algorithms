#include <iostream>
#include <sstream>

/*
 Write a program for counting words in a line.
 Any sequence of characters without separators is considered a word.
 Separators are spaces, tabs, newlines.
 The input string is passed to the program through the terminal as the argv [1] parameter.
 The program should display the number of words on the screen.
 */
using namespace std;

string ReplaceStringInPlace(string subject, const string& search, const string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}

int CountWords(char *argv[]) {
    string clean_s;
    clean_s = ReplaceStringInPlace(argv[1], "\\n", " ");
    clean_s = ReplaceStringInPlace(clean_s, "\\t", " ");
    const int N = 256;
    char word[N] = {};
    stringstream x;
    x << clean_s;
    int count_words = 0;
    while (x >> word) {
        count_words++;
    }
    return count_words;
}

int main(int argc, char *argv[]) {
    cout << "Total words: " << CountWords(argv) << endl;
    return 0;
}
