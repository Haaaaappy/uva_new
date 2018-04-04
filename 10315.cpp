#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <sstream>
#include <map>
#include <queue>
#include <bitset>

using namespace std;

int64_t getValue(vector<string> tokens);

int main() {
    string str;
    while (getline(cin, str)){
        istringstream buf(str);
        istream_iterator<string> beg(buf), end;

        vector<string> tokens(beg, end);
        vector<string> black (tokens.begin(), tokens.begin()+5);
        vector<string> white (tokens.begin()+5, tokens.end());

        int64_t bvalue = getValue(black);
        int64_t wvalue = getValue(white);

        if (bvalue > wvalue)
            cout << "Black wins." << endl;
        else if (bvalue < wvalue)
            cout << "White wins." << endl;
        else
            cout << "Tie." << endl;

//        cout << "black is " << bitset<64>(bvalue) << endl;
//        cout << "white is " << bitset<64>(wvalue) << endl;

    }

    return 0;
}

int64_t getValue(vector<string> tokens) {
    int64_t value = 0;
    int64_t unit = 1;
    map<char, int> shift;

    shift['2'] = -11;
    shift['3'] = -10;
    shift['4'] = -9;
    shift['5'] = -8;
    shift['6'] = -7;
    shift['7'] = -6;
    shift['8'] = -5;
    shift['9'] = -4;
    shift['T'] = -3;
    shift['J'] = -2;
    shift['Q'] = -1;
    shift['K'] = 0;
    shift['A'] = 1;

    // construct digits part
    for (auto &s : tokens){
        shift[s.front()] += 13;
    }
    for (auto &s : tokens){
        value |= (unit << shift[s.front()]);
    }

    // detect if is flush
    int64_t is_flush = 0;
    if (tokens[0].back() == tokens[1].back() && tokens[0].back() == tokens[2].back() &&
            tokens[0].back() == tokens[3].back() && tokens[0].back() == tokens[4].back())
    {
        is_flush = 1;
    }
    value |= (is_flush << 59);

    // detect if is straight
    int64_t is_straight = 1;
    priority_queue<int> straight;
    for (auto &s : tokens){
        straight.push(shift[s.front()]);
    }
    while (!straight.empty()){
        int top = straight.top();
        straight.pop();

        if (!straight.empty() && top - straight.top() != 1)
            is_straight = 0;
    }
    value |= (is_straight << 58);

    // detect if is straight flush
    value |= (is_straight & is_flush) << 62;

    // detect if is four of a kind
    int64_t val = value >> 41;
    int count = 0;
    int64_t is_four = 0;
    for (int i = 0; i < 13; i++){
        if ((val & unit) > 0)
            count++;
        val >>= 1;
    }
    if (count > 0)
        is_four = 1;
    value |= (is_four << 61);

    // detect if is three of a hand
    val = value >> 28;
    count = 0;
    int64_t is_three = 0;
    for (int i = 0; i < 13; i++){
        if ((val & unit) > 0)
            count++;
        val >>= 1;
    }
    if (count > 0)
        is_three = 1;
    value |= (is_three << 57);

    // detect if is pair
    val = value >> 15;
    count = 0;
    int64_t is_pair = 0;
    for (int i = 0; i < 13; i++){
        if ((val & unit) > 0)
            count++;
        val >>= 1;
    }
    if (count > 0)
        is_pair = 1;
    value |= (is_pair << 55);

    // detect if is two pairs
    int64_t is_two = 0;
    if (count >= 2)
        is_two = 1;
    value |= (is_two << 56);

    // detect if is full house
    value |= (is_three & is_pair) << 60;

    return value;

}
