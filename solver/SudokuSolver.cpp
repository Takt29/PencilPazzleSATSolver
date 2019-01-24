#include <cmath>
#include "../lib/CNFGenerator.h"
#include "../lib/MiniSatRunner.h"

struct SudokuData{
  int N;
  std::vector<std::vector<int>> hint;

  SudokuData(int N = 9):
    N(N), hint(N, std::vector<int>(N)) {}

  void scan(FILE *file) {
    int num;

    for (int i=0; i<N; i++) {
      for (int j=0; j<N; j++) {
        fscanf(file, "%1d", &num);
        hint[i][j] = num;
        if (num < 0 || 9 < num) {
          fprintf(stderr, "Error: Invalid data.\n");
          exit(1);
        }
      }
    }
  }
};

CNFGenerator<int> createCNF(const SudokuData data) {
  CNFGenerator<int> gen;
  const int N = data.N;
  const int sqrN = (int)sqrt(N);

  const auto getId = [&](int y, int x, int num) {
    return (y * N + x) * N + num;
  };

  //hint
  for (int y=0; y<N; y++) {
    for (int x=0; x<N; x++) {
      if (data.hint[y][x])
        gen.addA(CNFLiteral<int>(getId(y, x, data.hint[y][x]-1)));
    }
  }

  //row
  for (int y=0; y<N; y++) {
    for (int num=0; num<N; num++) {
      std::vector<CNFLiteral<int>> arr;
      for (int x=0; x<N; x++) {
        arr.push_back(CNFLiteral<int>(getId(y, x, num)));
      }
      gen.addArrayOr(arr);
    }
  }

  //column
  for (int x=0; x<N; x++) {
    for (int num=0; num<N; num++) {
      std::vector<CNFLiteral<int>> arr;
      for (int y=0; y<N; y++) {
        arr.push_back(CNFLiteral<int>(getId(y, x, num)));
      }
      gen.addArrayOr(arr);
    }
  }

  //block
  for (int block=0; block<N; block++) {
    for (int num=0; num<N; num++) {
      std::vector<CNFLiteral<int>> arr;
      for (int pos=0; pos<N; pos++) {
        int y = block / sqrN * sqrN + pos / sqrN;
        int x = block % sqrN * sqrN + pos % sqrN;
        arr.push_back(CNFLiteral<int>(getId(y, x, num)));
      }
      gen.addArrayOr(arr);
    }
  }


  //number
  for (int y=0; y<N; y++) {
    for (int x=0; x<N; x++) {
      std::vector<CNFLiteral<int>> arr;
      for (int num=0; num<N; num++) {
        arr.push_back(CNFLiteral<int>(getId(y, x, num)));
      }
      gen.addEqualX(arr, 1);
    }
  }

  return gen;
}

void showResult(const SudokuData &data, const std::map<int,bool> &result) {
  const int N = data.N;

  for (int y=0; y<N; y++) {
    for (int x=0; x<N; x++) {
      for (int num=0; num<N; num++) {
        auto it = result.find((y * N + x) * N + num);
        if (it->second) {
          printf("%d ", num+1);
        }
      }
    }
    printf("\n");
  }
}

int main(int argc, char** argv) {

  if (argc != 4) {
    fprintf(stderr, "Usage: %s sudoku_data_file input_cnf output_cnf\n", argv[0]);
    exit(1);
  }

  FILE *fp = fopen(argv[1], "r");

  if (fp == NULL) {
    fprintf(stderr, "Error: Can't open file. (%s)", argv[1]);
    exit(1);
  }

  SudokuData data;
  data.scan(fp);
  fclose(fp);

  auto gen = createCNF(data);
  auto runner = MiniSatRunner();

  auto res = runner.run(gen, argv[2], argv[3]);

  if (res.size() == 0) {
    fprintf(stderr, "This data has no solution.\n");
    exit(1);
  }

  showResult(data, res);

  return 0;
}
