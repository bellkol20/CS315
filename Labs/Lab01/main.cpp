#include <iostream>
#include <string>
using namespace std;

string ToHex(unsigned int value);

int main() {
    cout << ToHex(35) << endl;
    cout << ToHex(26) << endl;
}

string ToHex(unsigned int value) {
    if (value == 0) return "0";

    const string digits = "0123456789ABCDEF";
    string hex = "";

    while (value > 0) {
        hex = digits[value % 16] + hex;
        value /= 16;
    }

    return hex;
}
