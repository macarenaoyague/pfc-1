#ifndef PFC_PROJECT_UTIL_H
#define PFC_PROJECT_UTIL_H

#include "../Classes/Global.h"

void printVector(vector<int>& vector, ostream& out = cout){
    for(auto item : vector)
        out << item << " ";
    out << "\n";
}

void printMatrix(vector<vector<int>>& matrix, ostream& out = cout){
    for(auto & row : matrix){
        printVector(row, out);
    }
    out << endl;
}

#endif //PFC_PROJECT_UTIL_H
