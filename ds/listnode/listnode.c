#include <stdio.h>
#include <stdlib.h>
#include "listnode.h"
#include <assert.h>

ListNode * genListNode(int data, ListNode *next){
  ListNode *node = (ListNode *)malloc(sizeof(ListNode));
  assert(node != NULL);
  node->data = data;
  node->next = next;
  return node;
}

ListNode * insertLinkedList(ListNode *head, int data){
  ListNode *new = genListNode(data, NULL);
  if (head == NULL){
    return new;
  }

  ListNode *current = head;
  ListNode *previous = NULL;
  while (current != NULL && current->data < data){
    previous = current;
    current = current->next;
  }

  
  if (previous == NULL){
    new->next = head;
    return new;
  } else {
    new->next = previous->next;
    previous->next = new;
    return head;
  }
}

void printLinkedList(ListNode *node){
  while (node != NULL){
    ListNode *next = node->next;
    printf("%d\n", node->data);
    node = next;
  }
}

void freeLinkedList(ListNode *node){
  while (node != NULL){
    ListNode *next = node->next;
    free(node);
    node = next;
  }
}


