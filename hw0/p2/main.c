#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

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
  int row_seg_completed[MAXN][MAXN];
  int col_seg_completed[MAXN][MAXN];
  int row_seg_completed_cnt[MAXN];
  int col_seg_completed_cnt[MAXN];
} Clue;

void init(Clue * clue){
  assert(scanf("%d%d", &clue->N, &clue->M) == 2);
  for (int i = 0; i < clue->N; i++){
    assert(scanf("%d", &clue->row_seg_cnt[i]) == 1);
    for (int j = 0; j < clue->row_seg_cnt[i]; j++){
      assert(scanf("%d", &clue->row_seg[i][j]) == 1);
      clue->row_seg_completed[i][j] = -1;
    }
  }
  for (int i = 0; i < clue->M; i++){
    assert(scanf("%d", &clue->col_seg_cnt[i]) == 1);
    for (int j = 0; j < clue->col_seg_cnt[i]; j++){
      assert(scanf("%d", &clue->col_seg[i][j]) == 1);
      clue->col_seg_completed[i][j] = -1;
    }
  }
  for (int i = 0; i < clue->N; i++)
    clue->row_seg_completed_cnt[i] = 0;
  for (int i = 0; i < clue->M; i++)
    clue->col_seg_completed_cnt[i] = 0;
}

void printBoard(int board[MAXN][MAXN], int N, int M){
  for (int i = 0; i < N; i++){
    for (int j = 0; j < M; j++)
      switch (board[i][j]){
        case BLACK: printf("o"); break;
        case TEMP_BLACK: printf("1"); break;
        case WHITE: printf("_"); break;
        case SPACE: printf("x"); break;
      }
    printf("\n");
  }
}

bool checkRowEnd(int board[MAXN][MAXN], Clue * clue, int r){
  int sum = 0;
  for (int c = 0; c < clue->M; c++)
    sum += board[r][c];
  for (int i = 0; i < clue->row_seg_cnt[r]; i++)
    sum -= clue->row_seg[r][i];
  return sum == 0;
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

bool checkCell(int board[MAXN][MAXN], int i, int j, bool isRow, int value){
  int r = (isRow) ? i : j;
  int c = (isRow) ? j : i;
  return board[r][c] == value;
}

void paintCell(int board[MAXN][MAXN], int i, int j, bool isRow, int value){
  int r = (isRow) ? i : j;
  int c = (isRow) ? j : i;
  board[r][c] = value;
}

int fillLineBox(int line[MAXN], int seg_cnt, int seg[MAXN], int bound, int curr){
  for (int i = 0; i < seg_cnt; i++){
    for (int j = 0; j < seg[i]; j++)
      line[curr++] = i+MAXN;
    if (i != seg_cnt-1){
      line[curr++] = WHITE;
    }
  }
  return bound - curr;
}

void simpleBoxs(int completed[MAXN][MAXN], int seg_cnt[MAXN], int seg[MAXN][MAXN], int I, int J, bool isRow){
  for (int i = 0; i < I; i++){
    int right[MAXN] = {WHITE};
    int left[MAXN] = {WHITE};
    int spaceLeft = fillLineBox(left, seg_cnt[i], seg[i], J, 0);
    fillLineBox(right, seg_cnt[i], seg[i], J, spaceLeft);

    for (int j = 0; j < J; j++)
      if (left[j] != WHITE && right[j] != WHITE && left[j] == right[j])
        paintCell(completed, i, j, isRow, TEMP_BLACK);
  }
}

int isUniqueSegment(int seg[MAXN], int seg_cnt, int seg_len, int seg_completed){
  int count = 0;
  int index = -1;
  for (int i = 0; i < seg_cnt; i++)
    if (seg[i] == seg_len){
      count++;
      index = i;
    }
  return (count > 1) ? -1 : index;
}

void fillCompletedLine(int completed[MAXN][MAXN], int i, int I, int J, bool isRow){
  for (int j = 0; j < J; j++){
    if (!checkCell(completed, i, j, isRow, BLACK) && !checkCell(completed, i, j, isRow, TEMP_BLACK))
      paintCell(completed, i, j, isRow, SPACE);
  }
}

int completeSegment(int completed[MAXN][MAXN], int seg_cnt[MAXN], int seg_completed[MAXN][MAXN], int seg_completed_cnt[MAXN], int seg[MAXN][MAXN], int I, int J, bool isRow){
  int completed_cnt = 0;
  for (int i = 0; i < I; i++){
    if (seg_completed_cnt[i] == seg_cnt[i])
      continue; 

    int curr_seg = 0;
    int consec_cnt = 0;

    for (int j = 0; j < J; j++){
      if (checkCell(completed, i, j, isRow, TEMP_BLACK) || checkCell(completed, i, j, isRow, BLACK))
        consec_cnt++;
      if (consec_cnt > 0 && ((checkCell(completed, i, j+1, isRow, WHITE) || checkCell(completed, i, j+1, isRow, SPACE)) || j == J-1)){
        int uni_seg = isUniqueSegment(seg[i], seg_cnt[i], consec_cnt, seg_completed_cnt[i]);
        if (seg[i][curr_seg] == consec_cnt && uni_seg == -1){
          for (int k = j+1-consec_cnt; k <= j; k++)
            paintCell(completed, i, k, isRow, BLACK);
          seg_completed[i][curr_seg] = j+1-consec_cnt;
          seg_completed_cnt[i]++;
          completed_cnt++;
        }
        curr_seg++;
        consec_cnt = 0;
      }
    }
    if (seg_completed_cnt[i] == seg_cnt[i])
      fillCompletedLine(completed, i, I, J, isRow);     
  }
  return completed_cnt;
}
/*
void simpleSpaces(int completed[MAXN][MAXN],int seg_completed[MAXN][MAXN], int seg_completed_cnt[MAXN], int seg_cnt[MAXN], int seg[MAXN][MAXN], int I, int J, bool isRow){
  for (int i = 0; i < I; i++){
    if (seg_completed_cnt[i] > 0 && (seg_cnt[i] - seg_completed_cnt[i] == 1)){
      int uncompleted_seg = -1;
      for (int k = 0; k < seg_cnt[i]; k++){
        if (seg_completed[i][k] == -1) uncompleted_seg = k;
      }
      assert(uncompleted_seg != -1);

      for (int j = 0; j < J; j++){
        int uncompleted_cell = -1;
        if (checkCell(completed, i, j, isRow, TEMP_BLACK))
      
      }


      int prev_seg = uncompletd_seg-1;
      if (prev_seg >= 0){

      }

      for (int j = 0; j < J; j++){
        int uncompleted_cell = -1;
        if (checkCell(completed, i, j, isRow, TEMP_BLACK))
      
      }


      for (int j = 0; j < J; j++)
        if (!checkCell(completed, i, j, isRow, TEMP_BLACK) && !checkCell(completed, i, j, isRow, BLACK))
          paintCell(completed, i, j, isRow, SPACE);
    }
  }
}
*/


void preprocess(int completed[MAXN][MAXN], Clue * clue){
  simpleBoxs(completed, clue->row_seg_cnt, clue->row_seg, clue->N, clue->M, true);
  simpleBoxs(completed, clue->col_seg_cnt, clue->col_seg, clue->M, clue->N, false);
  completeSegment(completed, clue->row_seg_cnt, clue->row_seg_completed, clue->row_seg_completed_cnt, clue->row_seg, clue->N, clue->M, true);
  completeSegment(completed, clue->col_seg_cnt, clue->col_seg_completed, clue->row_seg_completed_cnt, clue->col_seg, clue->M, clue->N, false);
}

bool paint(int board[MAXN][MAXN], int completed[MAXN][MAXN], int r, int c, int value){
  if ((value == WHITE && (completed[r][c] == TEMP_BLACK || completed[r][c] == BLACK))
    || (value == BLACK && completed[r][c] == SPACE))
      return false;
  board[r][c] = value;
  return true;
}

bool recur(int board[MAXN][MAXN], int completed[MAXN][MAXN], Clue * clue, int r, int c, int curr_seg){
  if (r == clue->N){
    if (checkCol(board, clue->col_seg, clue->col_seg_cnt, clue->N, clue->M)){
      printBoard(board, clue->N, clue->M);
      return true;
    } else {
      return false;
    }
  }
  
  int temp_r = r;
  int temp_c = c;
  int temp_curr_seg = curr_seg;
  
  // paint black segment
  bool succeed = true;
  int row_seg_cnt = clue->row_seg_cnt[r];
  if (row_seg_cnt > 0){
    int seg_len = clue->row_seg[r][curr_seg];
    if (c+seg_len > clue->M)
      return false;  

    for (int i = 0; i < seg_len && succeed; i++)
      succeed = paint(board, completed, r, c+i, BLACK);
    c+= seg_len;
   
    if (succeed && c < clue->M){
      succeed = paint(board, completed, r, c, WHITE);
      c++;
    }
    curr_seg++;
  }

  if (succeed && curr_seg == row_seg_cnt){
    for (int i = c; i < clue->M && succeed; i++)
      succeed = paint(board, completed, r, i, WHITE);
    r++;
    c = 0;
    curr_seg = 0;
  }
  if (succeed) succeed = recur(board, completed, clue, r, c, curr_seg);
  if (succeed) return true;

  // paint a white cell 
  if (paint(board, completed, temp_r, temp_c, WHITE)){
    bool rowEnd = (temp_c == clue->M-1);
    succeed = recur(board, completed, clue, (rowEnd) ? temp_r+1:temp_r, (rowEnd) ? 0:temp_c+1, (rowEnd) ? 0:temp_curr_seg);
    if (succeed) return true;
  }
  return false;
}

int main(void){
  Clue clue;
  init(&clue);

  int board[MAXN][MAXN] = {WHITE};
  int completed[MAXN][MAXN] = {WHITE};
  preprocess(completed, &clue);
#ifdef DEBUG
  printBoard(completed, clue.N, clue.M);
  printf("\n");
#endif
  clock_t begin = clock();
  recur(board, completed, &clue, 0, 0, 0);  
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
#ifdef DEBUG
  printf("%f\n", time_spent);
#endif
}
