typedef struct listnode {
  int data;
  struct listnode *next;
} ListNode;

ListNode * genListNode(int data, ListNode *next);
ListNode * insertLinkedList(ListNode *head, int data);
void printLinkedList(ListNode *node);
void freeLinkedList(ListNode *node);
