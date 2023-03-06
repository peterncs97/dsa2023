#include <stdio.h>
#include "listnode.h"

int main(void){
  int num = 0;

  ListNode *head = NULL;

  while (scanf("%d", &num) != EOF){
    head = insertLinkedList(head, num);
  }
  printLinkedList(head);
  freeLinkedList(head);

  return 0;
}
