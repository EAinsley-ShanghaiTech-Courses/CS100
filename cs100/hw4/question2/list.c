#include <stdio.h>
#include <stdlib.h>
struct Mynode {
  int value;
  struct Mynode *next;
};
typedef struct Mynode Node;
struct Mylist {
  Node *head, *tail;
  size_t size;
};
typedef struct Mylist LinkedList;

void list_init(LinkedList *l) {
  if (!l) {
    l = (LinkedList *)malloc(sizeof(LinkedList));
  }
  l->size = 0;
  l->head = l->tail = NULL;
}

void list_insert(LinkedList *l, int value) {
  Node *newnode = (Node *)malloc(sizeof(Node));
  newnode->value = value;
  newnode->next = NULL;
  // when before the list is empty
  if (!l->tail)
    l->head = l->tail = newnode;
  else {
    l->tail->next = newnode;
    l->tail = newnode;
  }
  l->size++;
}

void list_clear(LinkedList *l) {
  Node *currentnode = l->head;
  while (!currentnode) {
    l->head = l->head->next;
    free(currentnode);
    currentnode = l->head;
  }
  l->size = 0;
  l->head = l->tail = NULL;
}

Node *list_remove(LinkedList *l, Node *target) {
  // remove head
  if (!l->size)
    return NULL;
  Node *prenode = l->head;
  if (target == l->head) {
    if (target == l->tail)
      l->tail = NULL;
    l->head = prenode->next;
    free(prenode);
    l->size--;
    return l->head;
  }
  // otherwise
  Node *currentnode = l->head->next;
  while (currentnode && currentnode != target) {
    prenode = currentnode;
    currentnode = currentnode->next;
  }
  // does not exist
  if (!currentnode)
    return NULL;
  // remove tail
  if (currentnode->next == NULL)
    l->tail = prenode;
  // remove middle
  prenode->next = currentnode->next;
  free(currentnode);
  l->size--;
  return prenode->next;
}
