#include "Tests/Testing.hpp"

int main() {
  for (int i = 0; i < 100; i++) {
      auto result = Testing(i, "Graphs/graph.txt");
      if (!result){
          cout << "Fallando i = " << i + 1 << endl;
      }
  }
}