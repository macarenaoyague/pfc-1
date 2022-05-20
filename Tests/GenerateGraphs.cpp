#include <fstream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <iostream>
using namespace std;

using weightType = short;

vector<vector<bool>> generateMatrixAdjacencyBoolean(int n, bool completed = true, bool directed = true){
    if(completed){
        vector<vector<bool>> matrixAdjacencyBoolean(n, vector<bool>(n, 1));
        for(int i = 0; i < n; ++i) matrixAdjacencyBoolean[i][i] = 0;
        return matrixAdjacencyBoolean;
    }
    else{
        vector<vector<bool>> matrixAdjacencyBoolean(n, vector<bool>(n, 0));
        for(int i = 0; i < n; i++){
            for(int j = directed ? 0 : i + 1; j < n; ++j){
                if(i != j){
                    if(!directed) matrixAdjacencyBoolean[i][j] = matrixAdjacencyBoolean[j][i] = (int) rand() % 2;
                    else matrixAdjacencyBoolean[i][j] = (int) rand() % 2;
                }
            }
        }
        return matrixAdjacencyBoolean;
    }
}

vector<vector<weightType>> generateMatrixAdjacency(int n, bool completed = true, bool directed = false){
    vector<vector<bool>> matrixAdjacencyBoolean = generateMatrixAdjacencyBoolean(n, completed, directed);
    vector<vector<weightType>> matrixAdjacency(n, vector<weightType>(n, 0));
    bool boolean;
    float seedFunction;
    for (int i = 0; i < n; i++){
        seedFunction = float(rand()%100+1)/100;
        for(int j = directed ? 0 : i + 1; j < n; j++){
            boolean = matrixAdjacencyBoolean[i][j];
            if (boolean) {
                if(!directed) {
                    matrixAdjacency[i][j] = ceil((rand()%1000 + 1)*seedFunction);
                    matrixAdjacency[j][i] = matrixAdjacency[i][j];
                }
                else matrixAdjacency[i][j] = (rand()%1000 + 1)*seedFunction;
            }
            else matrixAdjacency[i][j] = 0;
        }
    }
    return matrixAdjacency;
}

void saveMatrixAdjacency(vector<vector<weightType>>& matrix, string filename, bool directed = false){
    ofstream file(filename, ios::binary);
    if(file.is_open()){
        int n = matrix.size();
        file.write((char *)&n, sizeof(weightType));
        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++) file.write((char *)&matrix[i][j], sizeof(weightType));
        file.close();
    }else{
        assert(false);
    }

}

void generateGraphs(string folder = "", int start = 10, int end = 500, float rate = 1.5, int iterations = 10){
    ofstream file(folder + "nvertices.bin", ios::binary);
    for(int i = 0, n = start; i < iterations; i++, n*=rate){
        if(i == iterations - 1) n = end;
        file << n;
        string filename = folder + "graph" + to_string(n) + ".bin";
        auto matrix = generateMatrixAdjacency(n);
        saveMatrixAdjacency(matrix, filename);
    }
    file.close();
}

int main(){
    srand(time(NULL));
    //generateGraphs("../Graphs/");
    ifstream file("../cmake-build-debug/Graphs/graph10.bin", ios::binary);
    if(file.is_open()){
        int n;
        file.read((char *)& n, sizeof(short));
        cout << n;
    }else cout << "fail";
}
