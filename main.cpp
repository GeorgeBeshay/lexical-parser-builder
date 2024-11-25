#include <iostream>
#include <bits/stdc++.h>

using namespace std;

template <typename S>
ostream& operator<<(ostream& os,
                    const vector<S>& vector)
{
    for (auto element : vector) {
        os << element << ",";
    }
    os << endl;
    return os;
}

int main() {
    cout << "Hey" << endl;
    return 0;
}
