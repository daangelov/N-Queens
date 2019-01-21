#include <iostream>
#include <cstdlib>
#include "Board.h"

using namespace std;

// Main
int main(int argc, char* argv[])
{
    if (argc > 2)
    {
        cout << "Invalid argument n!";
        return 0;
    }

    int n; // = atoi(argv[1]);
    cin >> n;
    if (n < 4)
    {
        cout << "No solution exists for n < 4" << endl;
        return 0;
    }
    // Execution starts
    clock_t start = clock();

    srand(time(NULL));
    Board nQueens(n);
    nQueens.run();

    // Execution ends
    clock_t end = clock();

    nQueens.printValues();
    if (n < 101) {
        nQueens.printBoard();
    }
    cout << "Time: " << (end - start) / (double) CLOCKS_PER_SEC << endl;

    return 0;
}
