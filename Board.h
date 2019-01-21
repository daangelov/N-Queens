#ifndef BOARD_H
#define BOARD_H

#include <ctime>

using namespace std;

class Board
{
private:

    int* values;
    int* positiveD;
    int* negativeD;
    int* rows;
    int size;

public:

    Board();    // Default constructor
    Board(int); // Constructor
    ~Board();   // Destructor

public:

    void reset();          // Initiate Board
    void initValues();
    void initConflicts();

    // void shuffle();       // Shuffle the values
    void calcConflicts(); // Calculate rows, positive and negative diagonal conflicts

    void setValue(int, int);

    int diff(int, int);
    int sum(int, int, int);

    bool hasConflicts() const;
    int getColWithMaxConflicts();
    int getValueWithMinConflicts(int);
    int getConflictsAtColForValue(int, int);

    int* getConflicts();
    int getConflictsAt(int);
public:

    void run(); // Run algorithm to find solution

    void printValues() const;    // Print values as array
    void printBoard() const;     // Print values as board
    void printConflicts() const; // PRint conflicts
};

Board::Board()
{
    this->size = 0;
    this->values = nullptr;
    this->positiveD = nullptr;
    this->negativeD = nullptr;
    this->rows = nullptr;
}

Board::Board(int size)
{
    this->size = size;
    this->values = new int[size];
    this->positiveD = new int[2 * size - 1]();
    this->negativeD = new int[2 * size - 1]();
    this->rows = new int[size]();

    this->initValues();
    this->calcConflicts();
}

Board::~Board()
{
    if (this->values) delete[] values;
    if (this->positiveD) delete[] positiveD;
    if (this->negativeD) delete[] negativeD;
    if (this->rows) delete[] rows;
}

void Board::reset()
{
    this->initValues();
    this->initConflicts();
    this->calcConflicts();
}

void Board::initValues()
{
    for (int i = 0; i < this->size; ++i)
    {
        this->values[i] = rand() % this->size;
    }
}

void Board::initConflicts()
{
    for (int i = 0; i < 2 * this->size - 1; ++i)
    {
        if (i < this->size)
            this->rows[i] = 0;

        this->positiveD[i] = 0;
        this->negativeD[i] = 0;
    }
}

/*
void Board::shuffle()
{
    this->initValues();
    for (int i = this->size - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        swap(this->values[j], this->values[i]);
    }
}
*/

void Board::calcConflicts()
{
    for (int j = 0; j < this->size; ++j)
    {
        int i = this->values[j];

        // Calculate positive diagonal
        this->positiveD[this->diff(i, j)]++;
        // Calculate negative diagonal
        this->negativeD[i + j]++;
        // Calculate rows
        this->rows[i]++;
    }
}

void Board::setValue(int col, int value)
{
    int oldValue = this->values[col];
    this->rows[oldValue]--;
    this->positiveD[this->diff(oldValue, col)]--;
    this->negativeD[oldValue + col]--;

    this->values[col] = value;
    this->rows[value]++;
    this->positiveD[this->diff(value, col)]++;
    this->negativeD[value + col]++;
}

void Board::run()
{
    if (this->hasConflicts())
    {
        int numberOfMoves = 2 * this->size;

        while (true)
        {
            int col = this->getColWithMaxConflicts();
            int value = this->getValueWithMinConflicts(col);
            this->setValue(col, value);

            if (!this->hasConflicts())
            {
                std::cout << "number of moves: " << numberOfMoves << std::endl << std::endl;
                break;
            }
            numberOfMoves--;
            if (numberOfMoves == 0)
            {
                this->reset();
                numberOfMoves = 2 * this->size;
                std::cout << "Restarted" << std::endl;
            }
        }
    }
}

bool Board::hasConflicts() const
{
    for (int i = 0; i < this->size * 2 - 1; ++i)
    {
        if (i < this->size)
        {
            if (this->rows[i] > 1) return true;
        }
        if (this->positiveD[i] > 1) return true;
        if (this->negativeD[i] > 1) return true;
    }
    return false;
}

int Board::diff(int i, int j)
{
    int diff = i - j;
    if (diff < 0)
        return this->size * 2 - 1 + diff;

    return diff;
}

int Board::sum(int a, int b, int c)
{
    if (a > 0) a--;
    if (b > 0) b--;
    if (c > 0) c--;
    return a + b + c;
}

int* Board::getConflicts()
{
    int* conflicts = new int[this->size];
    for(int j = 0; j < this->size; ++j)
    {
        conflicts[j] = this->getConflictsAt(j);
    }
    return conflicts;
}

int Board::getConflictsAt(int col)
{
    return this->sum(
               this->rows[this->values[col]],
               this->positiveD[this->diff(this->values[col], col)],
               this->negativeD[this->values[col] + col]
           );
}

int Board::getColWithMaxConflicts()
{
    int col = 0;
    int maxConflicts = this->getConflictsAt(col);

    for (int j = 1; j < this->size; ++j)
    {
        int currConflicts = this->getConflictsAt(j);
        if (currConflicts > maxConflicts)
        {
            col = j;
            maxConflicts = currConflicts;
        }
        else if (currConflicts == maxConflicts)
        {
            if (rand() % 2 == 0)
            {
                col = j;
                maxConflicts = currConflicts;
            }
        }
    }
    return col;
}

int Board::getConflictsAtColForValue(int col, int value)
{
    int currValue = this->values[col];
    this->rows[currValue]--;
    this->positiveD[this->diff(currValue, col)]--;
    this->negativeD[currValue + col]--;

    this->rows[value]++;
    this->positiveD[this->diff(value, col)]++;
    this->negativeD[value + col]++;

    int conflicts = this->sum(
                        this->rows[value],
                        this->positiveD[this->diff(value, col)],
                        this->negativeD[value + col]
                    );

    this->rows[value]--;
    this->positiveD[this->diff(value, col)]--;
    this->negativeD[value + col]--;

    this->rows[currValue]++;
    this->positiveD[this->diff(currValue, col)]++;
    this->negativeD[currValue + col]++;

    return conflicts;
}

int Board::getValueWithMinConflicts(int col)
{
    int valueWithMinConflicts = this->values[col];
    int conflictsForCol = this->getConflictsAt(col);

    for (int i = 0; i < this->size; ++i)
    {
        if (i == valueWithMinConflicts) continue;

        int conflictsAtI = this->getConflictsAtColForValue(col, i);
        if (conflictsAtI < conflictsForCol)
        {
            conflictsForCol = conflictsAtI;
            valueWithMinConflicts = i;
        }
        else if (conflictsAtI == conflictsForCol)
        {
            if (rand() % 2 == 0)
            {
                conflictsForCol = conflictsAtI;
                valueWithMinConflicts = i;
            }
        }
    }

    return valueWithMinConflicts;
}

void Board::printBoard() const
{
    char** board = new char*[this->size];
    for(int i = 0; i < this->size; ++i)
    {
        board[i] = new char[this->size];
    }

    for (int i = 0; i < this->size; ++i)
    {
        for (int j = 0; j < this->size; ++j)
        {
            board[i][j] = (this->values[j] == i) ? '*' : '_';
        }
    }

    for (int i = 0; i < this->size; ++i)
    {
        for (int j = 0; j < this->size; ++j)
        {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Board::printValues() const
{
    for (int i = 0; i < this->size; ++i)
    {
        std::cout << this->values[i] << " ";
    }
    std::cout << std::endl;
}

void Board::printConflicts() const
{
    std::cout << "positiveD: ";
    for (int i = 0; i < this->size * 2 - 1; ++i)
    {
        std::cout << this->positiveD[i] << " ";
    }
    std::cout << endl;

    std::cout << "negativeD: ";
    for (int i = 0; i < this->size * 2 - 1; ++i)
    {
        std::cout << this->negativeD[i] << " ";
    }
    std::cout << endl;

    std::cout << "rows: ";
    for (int i = 0; i < this->size; ++i)
    {
        std::cout << this->rows[i] << " ";
    }
    std::cout << endl;

}

#endif // BOARD_H

