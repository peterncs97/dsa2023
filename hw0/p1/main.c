#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#define MAXDIGIT 256
#define EQUAL 0
#define LARGER 1
#define SMALLER 2

struct bigInt {
  int digit[MAXDIGIT];
  int digitCount;
};
typedef struct bigInt BigInt;

void printBigInt(BigInt *biptr){
  for (int i = biptr->digitCount-1; i >= 0; i--){
    printf("%d", biptr->digit[i]);
  }

  if (biptr->digitCount <= 0){
    printf("0");
  }
  printf("\n");
}

void scanBigInt(BigInt *biptr){
  int temp[MAXDIGIT] = {0};
  int digitCount = 0;

  char c = 0;
  while (scanf("%c", &c) != EOF && isdigit(c)){
    temp[digitCount] = c-48;  
    digitCount++;
  }

  biptr->digitCount = digitCount;
  for (int i = 0; i < digitCount; i++){
    biptr->digit[i] = temp[digitCount-i-1];
  }
}

int compBigInt(BigInt *biptr1, BigInt *biptr2){
  if (biptr1->digitCount > biptr2->digitCount){
    return LARGER;
  } else if (biptr1->digitCount < biptr2->digitCount){
    return SMALLER;
  } else {
    for (int i = biptr1->digitCount-1; i >= 0; i--){
      if (biptr1->digit[i] > biptr2->digit[i]){
        return LARGER;
      } else if (biptr1->digit[i] < biptr2->digit[i]){
        return SMALLER;
      }
    }
    return EQUAL;
  }
}

// assume bigIntA is larger
void subtractBigInt(BigInt *biptrA, BigInt *biptrB){
  int borrow[MAXDIGIT] = {0};

  for (int i = 0; i < biptrA->digitCount; i++){
    int newDigit = biptrA->digit[i] - biptrB->digit[i] - borrow[i];
    if (newDigit < 0){
      newDigit += 10;
      borrow[i+1]++;
    }
    biptrA->digit[i] = newDigit;
  }

  for (int i = biptrA->digitCount-1; i >= 0; i--){
    if (biptrA->digit[i] == 0){
      biptrA->digitCount--;
    } else {
      break;
    }
  }
}

void divideBigIntByTwo(BigInt *biptrA){
  int prev = 0;

  if (biptrA->digit[biptrA->digitCount-1] == 1){
    biptrA->digit[biptrA->digitCount-1] = 0;
    biptrA->digitCount--;
    prev = 1;
  }

  for (int i = biptrA->digitCount-1; i >= 0; i--){
    int value = 10 * prev + biptrA->digit[i];
    biptrA->digit[i] = value/2;
    prev = value % 2;
  }
}

void multiplyBigIntByPowerOfTwo(BigInt *biptrA, int k){
  for (int i = 0; i < k; i++){
    int carry[MAXDIGIT] = {0};
    for (int j = 0; j < biptrA->digitCount; j++){
      int newDigit = (biptrA->digit[j] * 2) + carry[j];
      if (newDigit > 9){
        biptrA->digit[j] = newDigit - 10;
        carry[j+1] = 1;
      } else {
        biptrA->digit[j] = newDigit;
      }
    }
    if (carry[biptrA->digitCount] == 1){
      biptrA->digitCount++;
      biptrA->digit[biptrA->digitCount-1] = 1;
    }
  }
}

bool isZero(BigInt *biptr){
  return (biptr->digitCount == 0 && biptr->digit[0] == 0);
}

bool isEven(BigInt *biptr){
  return (biptr->digit[0] % 2 == 0);
}
 
int main()
{
  BigInt a = {{0}, 0};
  BigInt b = {{0}, 0};
  scanBigInt(&a);
  scanBigInt(&b);

  BigInt *n = &a;
  BigInt *m = &b;

  if (compBigInt(&a, &b) == LARGER){
    m = &a;
    n = &b;
  }

  int ans = 0;
  while (!isZero(n) && !isZero(m)){
    if (isEven(n) && isEven(m)){
      ans++;
      divideBigIntByTwo(n);
      divideBigIntByTwo(m);
    } else if (isEven(n)){
      divideBigIntByTwo(n); 
    } else if (isEven(m)){
      divideBigIntByTwo(m); 
    }
 
    if (compBigInt(n, m) == LARGER){
      BigInt *temp = n;
      n = m;
      m = temp; 
    }
  
    subtractBigInt(m, n);
 }  
  
  multiplyBigIntByPowerOfTwo(n, ans);

  printBigInt(n);
}
