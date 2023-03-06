#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#define MAXN 25
#define WHITE 0
#define BLACK 1
#define TEMP_BLACK 2
#define SPACE 3

typedef struct clue {
  int N;
  int M;
  int row_seg[MAXN][MAXN];
  int col_seg[MAXN][MAXN];
  int row_seg_cnt[MAXN];
  int col_seg_cnt[MAXN];
} Clue;

void init(Clue * clue){
  assert(scanf("%d%d", &clue->N, &clue->M) == 2);
  for (int i = 0; i < clue->N; i++){
    assert(scanf("%d", &clue->row_seg_cnt[i]) == 1);
    for (int j = 0; j < clue->row_seg_cnt[i]; j++)
      assert(scanf("%d", &clue->row_seg[i][j]) == 1);
  }
  for (int i = 0; i < clue->M; i++){
    assert(scanf("%d", &clue->col_seg_cnt[i]) == 1);
    for (int j = 0; j < clue->col_seg_cnt[i]; j++)
      assert(scanf("%d", &clue->col_seg[i][j]) == 1);
  }
}

void printBoard(int board[MAXN][MAXN], int N, int M){
  for (int i = 0; i < N; i++){
    for (int j = 0; j < M; j++)
      switch (board[i][j]){
        case BLACK: printf("o"); break;
        case WHITE: printf("_"); break;
      }
    printf("\n");
  }
}

bool checkCol(int board[MAXN][MAXN], int clue_seg[][MAXN], int clue_seg_cnt[MAXN], int row, int col){
  for (int c = 0; c < col; c++){
    int seg_cnt = clue_seg_cnt[c];
    int curr_seg = 0;
    int consec_cnt = 0;

    for (int r = 0; r < row; r++){
      if (board[r][c] == BLACK)
        consec_cnt++;
      if (consec_cnt > 0 && (board[r][c] == WHITE || r == row-1)){
        if (clue_seg[c][curr_seg] != consec_cnt || curr_seg >= seg_cnt)
          return false;
        curr_seg++;
        consec_cnt = 0;
      }
    }
    if (curr_seg != seg_cnt)
      return false;
  }
  return true;
}

bool checkRow(int board[MAXN][MAXN], int clue_seg[][MAXN], int clue_seg_cnt[MAXN], int row, int col){
  for (int r = 0; r < row; r++){
    int seg_cnt = clue_seg_cnt[r];
    int curr_seg = 0;
    int consec_cnt = 0;

    for (int c = 0; c < col; c++){
      if (board[r][c] == BLACK)
        consec_cnt++;
      if (consec_cnt > 0 && (board[r][c] == WHITE || c == col-1)){
        if (clue_seg[r][curr_seg] != consec_cnt || curr_seg >= seg_cnt)
          return false;
        curr_seg++;
        consec_cnt = 0;
      }
    }
    if (curr_seg != seg_cnt)
      return false;
  }
  return true;
}

bool recur(int board[MAXN][MAXN], Clue * clue, int r, int c){  
  if (r == clue->N){
    if (checkCol(board, clue->col_seg, clue->col_seg_cnt, clue->N, clue->M) 
        && checkRow(board, clue->row_seg, clue->row_seg_cnt, clue->N, clue->M)){
      printBoard(board, clue->N, clue->M);
      return true;
    }
    return false;
  }

  bool isRowEnd = (c == clue->M-1);
 
  board[r][c] = BLACK;
  bool succeed = recur(board, clue, (isRowEnd) ? r+1:r, (isRowEnd) ? 0:c+1);
  if (succeed) return true;

  board[r][c] = WHITE;
  succeed = recur(board, clue, (isRowEnd) ? r+1:r, (isRowEnd) ? 0:c+1);
  if (succeed) return true;

  return false;
}

int main(void){
  Clue clue;
  init(&clue);

  int board[MAXN][MAXN] = {WHITE};
  recur(board, &clue, 0, 0);  
}
