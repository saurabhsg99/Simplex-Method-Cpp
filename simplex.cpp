#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
#define PRINTC(c, f, s) printf("\033[%dm" f "\033[0m", 30 + c, s)

void printTable(vector<vector<double>> table)
{
    cout << endl;
    for (int i = 0; i < table.size(); i++)
    {
        for (int j = 0; j < table[i].size(); j++)
        {
            if (i == 0 && j == 0)
                PRINTC(5, "%12s", "Cj / Xb");

            else if (i > 0 && i < table.size() - 2 && j > 0 && j < table[i].size() - 1)
            {
                PRINTC(1, "%10.3f ", table[i][j]);
            }
            else if (i == table.size() - 2 && j == 0)
            {
                PRINTC(5, "%12s", "Zj   ");
            }
            else if (i == 0 && j == table[0].size() - 1)
            {
                PRINTC(5, "%12s", "minRatio");
            }
            else if (i == table.size() - 1 && j == 0)
            {
                PRINTC(5, "%12s", "Cj-Zj  ");
            }
            else if (i == table.size() - 1 && j == table[i].size() - 2)
            {
                PRINTC(5, "%12s", "      ");
            }
            else if (i == 0 && j == table[i].size() - 2)
            {
                PRINTC(5, "%12s", "      ");
            }
            else if ((i > table.size() - 3 )&& (j == table[i].size() - 1))
                PRINTC(4, "%12s", "      ");
            else if (j == 0)
                printf("%11.3f ", table[i][j]);
            else
                printf("%10.3f ", table[i][j]);
        }
        cout << endl;
    }
    cout << endl;
}

int main()
{
    vector<vector<double>> matrix2;

    // Input the initial matrix2
    int m, n;
    cout << "\nEnter the number of constraints (m): ";
    cin >> m;
    cout << "Enter the number of variables (n): ";
    cin >> n;

    // Initialize the matrix2 with appropriate dimensions
    matrix2.resize(m + 1, vector<double>(n + m + 2));

    // Input the coefficients of the objective function (c)
    cout << "Enter the coefficients of the objective function (c): ";
    for (int j = 1; j <= n; j++)
    {
        cin >> matrix2[0][j];
    }

    // Input the coefficients of the constraints (A) and the right-hand side (b)
    for (int i = 1; i <= m; i++)
    {
        cout << "Enter the coefficients of the constraint " << i << " : ";
        for (int j = 1; j <= n; j++)
        {
            cin >> matrix2[i][j];
        }
        matrix2[i][n + i] = 1;
        cout << "Enter the right side value of constraint " << i << " : ";
        cin >> matrix2[i][n + m + 1];
        cout << endl;
    }

    cout << "Options : " << endl;
    cout << "1: Maximum" << endl;
    cout << "2: Minimum" << endl;
    int choice = 1;
    cout << "Select the Option : ";
    cin >> choice;
    if (choice == 2)
    {
        for (int i = 1; i <= n; i++)
        {
            matrix2[0][i] = -matrix2[0][i];
        }
    }

    int col = matrix2[0].size() + 1;
    int row = matrix2.size() + 2;
    vector<vector<double>> table(row, vector<double>(col, -1));

    // fill up table
    for (int i = 0; i < matrix2.size(); i++)
    {
        for (int j = 0; j < matrix2[i].size(); j++)
        {
            table[i][j] = matrix2[i][j];
        }
    }

    vector<int> incomingV; // for variables

    // getting the max in cofficient
    auto maxC = max_element(table[0].begin() + 1, table[0].end() - 2);
    int incoming = maxC - table[0].begin();

    incomingV.push_back(incoming);

    int minRatio = 1;
    int minRatioCol = col - 1; // exact index hai
    int ZiRow = row - 2;

    bool flagg = true;
    for (int i = 1; i < ZiRow; i++)
    {
        if (table[i][incoming] > 0)
        {
            table[i][minRatioCol] = table[i][minRatioCol - 1] / table[i][incoming];
            if (table[minRatio][minRatioCol] > table[i][minRatioCol])
            {
                minRatio = i;
            }
            flagg = false;
        }
    }
    if (flagg)
    {
        printTable(table);
        cout << "\n\tIts unbounded" << endl;
        return 3;
    }
    // zi
    for (int i = 1; i < minRatioCol; i++)
    {
        table[ZiRow][i] = table[0][i];
    }
    cout << "Iteration 1 : " << endl;
    printTable(table);

    // cout
    //     << "Minimum minRatio is " << table[minRatio][minRatioCol - 1] << endl;

    // simplex;
    vector<vector<double>> newTable = table;
    int it = 1;
    while (1)
    {
        flagg = true;

        // cout << "minration is " << minRatio << endl;
        double pivot = newTable[minRatio][incoming];
        // cout << "Pivot is : " << pivot << endl;

        newTable[minRatio][0] = newTable[0][incoming];
        incomingV.push_back(incoming);

        for (int j = 1; j < row; j++)
        {
            if (j == minRatio)
            {
                for (int i = 1; i <= minRatioCol - 1; i++)
                    newTable[j][i] = table[j][i] / pivot;
            }
            else if (j > 0 && j < ZiRow)
            {
                for (int i = 1; i <= minRatioCol - 1; i++)
                {
                    newTable[j][i] = table[j][i] - (table[j][incoming] * table[minRatio][i]) / pivot;
                }
            }
            if (j == ZiRow)
            {
                // calculating zj
                for (int i = 1; i <= minRatioCol - 1; i++)
                {
                    double temp = 0;
                    for (int k = 1; k < ZiRow; k++)
                    {
                        temp = temp + newTable[k][0] * newTable[k][i];
                    }
                    newTable[j][i] = temp;
                }
            }
            // cj-zj
            if (j == row - 1)
            {
                int greatest = 1;
                bool flag = true;
                for (int i = 1; i < minRatioCol - 1; i++)
                {
                    newTable[j][i] = newTable[0][i] - newTable[ZiRow][i];
                    if (newTable[j][i] > 0)
                    {
                        flag = 0;
                    }
                    if (newTable[j][greatest] < newTable[j][i])
                        greatest = i;
                }
                incoming = greatest;
                // cout << "Incoming is " << incoming << endl;
                if (flag)
                {
                    ++it;
                    cout << "Iteration " << it << " is :" << endl;
                    printTable(newTable);

                    // cout << newTable[3][1] << endl;
                    cout << "Optimal Solution:" << endl;
                    for (int r = 1; r <= n; r++)
                    {
                        int basic_row = -1;
                        for (int i = 1; i <= m; i++)
                        {
                            if ((double)newTable[i][r] == 1.0 && basic_row == -1)
                            {
                                basic_row = i;
                            }
                            else if ((double)newTable[i][r] != 0.0)
                            {
                                basic_row = -1;
                                break;
                            }
                        }
                        if (basic_row != -1)
                        {
                            cout << "\tx" << r << " = " << newTable[basic_row][n + m + 1] << endl;
                        }
                        else
                        {
                            cout << "\tx" << r << " = 0" << endl;
                        }
                    }
                    float z = newTable[ZiRow][minRatioCol - 1];
                    if(choice ==2)
                        z = -z;
                    cout << "\tz = " <<z  << endl;

                    return 1;
                }
            }
        }
        ++it;
        cout << "Iteration " << it << " is :" << endl;
        printTable(newTable);
        minRatio = 1;
        for (int i = 1; i < ZiRow; i++)
        {
            if (newTable[i][incoming] > 0)
            {
                newTable[i][minRatioCol] = newTable[i][minRatioCol - 1] / newTable[i][incoming];
                if (newTable[i][minRatioCol] < newTable[minRatio][minRatioCol])
                {
                    minRatio = i;
                }
                flagg = false;
            }
        }
        if (flagg)
        {
            printTable(newTable);
            cout << "\n\t Its unbounded" << endl;
            return 3;
        }
        table = newTable;
    }

    return 0;
}
