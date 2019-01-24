#include "../lib/CNFGenerator.h"
#include "../lib/MiniSatRunner.h"

struct NonogramData{
  int H, W;
  std::vector<std::vector<int> > rowHint, columnHint;

  NonogramData() {}

  void scan(FILE *file) {
    fscanf(file, "%d%d", &H, &W);

    rowHint.assign(H, std::vector<int>());
    columnHint.assign(W, std::vector<int>());

    for(int i=0; i<H; i++){
      int k, v;
      fscanf(file, "%d", &k);
      for(int j=0; j<k; j++){
        fscanf(file, "%d", &v);
        rowHint[i].push_back(v);
      }
    }

    for(int i=0; i<W; i++){
      int k, v;
      fscanf(file, "%d", &k);
      for(int j=0; j<k; j++){
        fscanf(file, "%d", &v);
        columnHint[i].push_back(v);
      }
    }
  }
};


CNFGenerator<int> createCNF(NonogramData data) {
  CNFGenerator<int> gen;

  const auto getPosId = [&](int y, int x, bool isRotate) {
    if (!isRotate) return y * data.W + x + 1;
    return x * data.W + y + 1;
  };

  const auto getHintId = [&](int y, int x, int k, bool isRotate) {
    int base = data.W * data.H + 1;
    if (!isRotate) return base + (y * data.W + x) * data.W + k;
    return base * (data.W + 1) + (x * data.W + y) * data.H + k;
  };

  const auto L = [](int v) { return CNFLiteral<int>(abs(v), v<0); };

  for (int rotate=0; rotate<2; rotate++) {
    int line = rotate == 0 ? data.H : data.W;
    int len = rotate == 0 ? data.W : data.H;

    for (int y=0; y<line; y++) {
      const auto &hint = rotate == 0 ? data.rowHint[y] : data.columnHint[y];

      if (hint.size() == 0) continue;

      // hint[0].x <= xp && xp < hint[0].x => (xp, y) is black
      for (int k=0; k<hint.size(); k++)
        for (int x=0; x<=len-hint[k]; x++)
          for (int xp=x; xp<x+hint[k]; xp++)
            gen.addAtoB(L(getHintId(y, x, k, rotate)), L(getPosId(y, xp, rotate)));

      // hint[k].x < hint[k+1].x
      for (int k=0; k+1<hint.size(); k++)
        for (int x=0; x<len; x++)
          for (int xp=0; xp<=x+hint[k] && xp<len; xp++)
            gen.addAtoB(L(getHintId(y, x, k, rotate)), L(-getHintId(y, xp, k+1, rotate)));

      // hint[0].x > xp -> (xp, y) is white
      for (int x=0; x<=len-hint[0]; x++)
        for (int xp=0; xp<x; xp++)
          gen.addAtoB(L(getHintId(y, x, 0, rotate)), L(-getPosId(y, xp, rotate)));

      // hint[-1].x + hint[-1] <= xp -> (xp, y) is white
      for (int x=0; x<=len-hint[hint.size()-1]; x++)
        for (int xp=x+hint[hint.size()-1]; xp<len; xp++)
          gen.addAtoB(L(getHintId(y, x, hint.size()-1, rotate)), L(-getPosId(y, xp, rotate)));

      // hint[i].x + hint[i] <= xp && xp < hint[i+1].x -> (xp, y) is white
      for (int k=0; k+1<hint.size(); k++)
        for(int x1=0; x1<=len-hint[k]; x1++)
          for(int x2=x1+hint[k]+1; x2<=len-hint[k+1]; x2++)
            for(int xp=x1+hint[k]; xp<x2; xp++){
              std::vector<CNFLiteral<int>> arr = {L(getHintId(y, x1, k, rotate)), L(getHintId(y, x2, k+1, rotate))};
              gen.addArrayAndtoB(arr, L(-getPosId(y, xp, rotate)));
            }

      // use hint
      for (int k=0; k<hint.size(); k++) {
        std::vector<CNFLiteral<int>> arr;
        for (int xp=0; xp<=len-hint[k]; xp++)
          arr.push_back(L(getHintId(y, xp, k, rotate)));
        gen.addArrayOr(arr);
      }
    }
  }

  return gen;
}

void showResult(NonogramData &data, std::map<int,bool> &result) {
  for (int y=0; y<data.H; y++) {
    for (int x=0; x<data.W; x++) {
      if (result[y*data.W+x+1])
        printf("#");
      else
        printf(" ");
    }
    printf("\n");
  }
}

int main(int argc, char** argv) {

  if (argc != 4) {
    fprintf(stderr, "Usage: %s nonogram_data_file input_cnf output_cnf\n", argv[0]);
    exit(1);
  }

  FILE *fp = fopen(argv[1], "r");

  if (fp == NULL) {
    fprintf(stderr, "Error: Can't open file. (%s)", argv[1]);
    exit(1);
  }

  NonogramData data;
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
