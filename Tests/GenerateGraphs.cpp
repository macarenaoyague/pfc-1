#include "Util.hpp"
#include "fstream"

vector<vector<bool>> generateMatrixAdjacencyBoolean(int n,
                                                    bool completed = false,
                                                    bool directed = false) {
  vector<vector<bool>> matrixAdjacencyBoolean(n, vector<bool>(n, 0));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; ++j) {
      if (i != j) {
        if (completed)
          matrixAdjacencyBoolean[i][j] = 1;
        else if (!directed)
          matrixAdjacencyBoolean[i][j] = matrixAdjacencyBoolean[j][i] =
              (int)rand() % 2;
        else
          matrixAdjacencyBoolean[i][j] = (int)rand() % 2;
      }
    }
  }
  return matrixAdjacencyBoolean;
}

vector<vector<int>> generateMatrixAdjacency(int n, bool completed = false,
                                            bool directed = false) {
  vector<vector<bool>> matrixAdjacencyBoolean =
      generateMatrixAdjacencyBoolean(n, completed, directed);
  vector<vector<int>> matrixAdjacency(n, vector<int>(n, 0));
  bool boolean;
  float seedFunction;
  for (int i = 0; i < n; i++) {
    seedFunction = float(rand() % 100 + 1) / 100;
    for (int j = 0; j < n; j++) {
      boolean = matrixAdjacencyBoolean[i][j];
      if (boolean) {
        if (!directed)
          matrixAdjacency[i][j] = matrixAdjacency[j][i] =
              (rand() % 1000) * seedFunction;
        else
          matrixAdjacency[i][j] = (rand() % 1000) * seedFunction;
      } else
        matrixAdjacency[i][j] = 0;
    }
  }
  return matrixAdjacency;
}

void generateGraphs(int limit, string folder = "") {
  for (int i = 2; i < limit; ++i) {
    string fileName = folder + "graph" + to_string(i) + ".txt";
    ofstream file(fileName);
    auto matrix = generateMatrixAdjacency(i);
    file << i << "\n";
    printMatrix(matrix, file);
    file.close();
  }
}

int main() {
  srand(time(NULL));
  ofstream file("graph.txt");
  auto matrix = generateMatrixAdjacency(16);
  printMatrix(matrix, file);
  // generateGraphs(100, "../Graphs/");
}
