#include <fstream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <iostream>
using namespace std;

using weightType = short;

vector<int> readNVertices(const string& filename){
    ifstream file(filename);
    if(file.is_open()) {
        vector<int> r;
        short n;
        while (!file.eof()) {
            file.read((char*)&n, sizeof(short));
            r.emplace_back(n);
        }
        return r;
    }
    else assert(false);
}

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

void generateGraphs(string folder = "", int start = 10, int end = 500, int step = 1){
    assert(start <= end);
    ofstream file(folder + "nvertices.bin", ios::binary | ios::app);
    for(short n = start; n <= end; n+=step){
        file.write((char*)&n, sizeof(short));
        string filename = folder + "graph" + to_string(n) + ".bin";
        auto matrix = generateMatrixAdjacency(n);
        saveMatrixAdjacency(matrix, filename);
    }
    file.close();
}

int main(){
    srand(time(NULL));
    generateGraphs("../Graphs/", 10, 990, 10);
    generateGraphs("../Graphs/", 1000, 5000, 200);
    auto v = readNVertices("../Graphs/nvertices.bin");
    for(auto& item : v){
        cout << item << ", ";
    }
}
