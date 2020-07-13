#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RANGE_SIZE 512*512
#define uint64_t UINT64

typedef unsigned long  uint64_t;

typedef struct __pair {
  UINT64 first;
  UINT64 second;
  UINT64 stamp;
} pair;

typedef struct __node {
  struct __pair *data;
  struct __node *prev;
  struct __node *next;
} node;

typedef struct __list {
  struct __node *head;
  struct __node *curr;
  struct __node *tail;
} linkedList;

void createNode(linkedList*, UINT64[]);
void findLocation(linkedList*, UINT64);
void collect(linkedList*, UINT64[]);
void freeNode(node*);
int deleteCurrNode(linkedList*);
void divideByMaxRange(linkedList*);
void printLinkedList(linkedList*);

int main(int argc, char **argv) {

  if(argc < 3) {
    printf("usage: ./parser [FILE_NAME] [I/O_SIZE]\n");
    return 0;
  }
  else if((atoi(argv[2]) % 512) != 0) {
    printf("I/O Size must be a positive integer that is a multiple of 512.\n");
    return 0;
  }

  if(access(".tmp", 0) != 0) {
    printf("Required file not found. First run the trace script and generator.\n");
    exit(1);
  }

  if(access(argv[1], 0) != 0) {
    printf("There are no files to parse. First run the trace script and generator.\n");
    exit(1);
  }

  int tmp = open(".tmp", O_RDONLY);
  char pbuf[16] = { 0x00 };
  read(tmp, pbuf, 16); close(tmp);
  int pnum = atoi(pbuf);
  int offset = (atoi(argv[2]) / 512) - 1;

  char strBuf[256] = { 0 };
  long start = -1; long end = -1;
  UINT64 pcount = 0;

  FILE *f = fopen(argv[1], "r");

  linkedList *L = (linkedList *)malloc(sizeof(linkedList));
  L->curr = NULL; L->head = NULL; L->tail = NULL;
  UINT64 pairBuf[3] = { 0 };

  while(!feof(f)) {

    memset(strBuf, 0, sizeof(strBuf));
    fgets(strBuf, sizeof(strBuf), f);

    char* p = strtok(strBuf, " ");
    char e[4] = { 0 };
    strncpy(e, p, 3);
    if(!strcmp(e, "CPU")) {
      pairBuf[0] = start;
      pairBuf[1] = end + offset;
      pairBuf[2] = pcount;
      findLocation(L, pairBuf[0]);
      collect(L, pairBuf);
      pcount++;
      break;
    }
    if(atoi(p) == pnum) {
      p = strtok(NULL, " ");
      if(strcmp(p, "R") != 0) continue;
      p = strtok(NULL, " ");
      int now = atoi(p);
      if(start == -1) {
        start = now;
      }
      if(end == -1 || now == end + 1) {
        end = now;
      }
      else {
        pairBuf[0] = start;
        pairBuf[1] = end + offset;
        pairBuf[2] = pcount;
        findLocation(L, pairBuf[0]);
        collect(L, pairBuf);
        start = now; end = now;
        pcount++;
      }
    }
  }

  divideByMaxRange(L);
  printLinkedList(L);

  return 0;
}

void createNode(linkedList *L, UINT64 pPair[]) {
  node *newNode = (node *)malloc(sizeof(node));
  newNode->data = (pair *)malloc(sizeof(pair));
  newNode->data->first = pPair[0];
  newNode->data->second = pPair[1];
  newNode->data->stamp = pPair[2];
  newNode->prev = NULL;
  newNode->next = NULL;

  if(L->head == NULL && L->tail == NULL) {
    L->head = L->tail = newNode;
    L->curr = newNode;
  }
  else {
    L->tail->next = newNode;
    L->tail->next->prev = L->tail;
    L->tail = newNode;
  }
}

void findLocation(linkedList *L, UINT64 p) {
  if(L->curr == NULL) return;
  if(L->curr->data->first > p) {
    while(L->curr->data->first > p) {
      if(L->curr->prev == NULL) break;
      L->curr = L->curr->prev;
    }
  }
  else {
    while(L->curr->data->first < p) {
      if(L->curr->next == NULL) break;
      L->curr = L->curr->next;
    }
  }
}

void collect(linkedList *L, UINT64 pPair[]) {

  if(L->curr == NULL) {
    createNode(L, pPair);
    return;
  }

  if(L->curr->data->first > pPair[0]) {
    if(L->curr->data->first - 1 == pPair[1]) {
      L->curr->data->first = pPair[0];
      if(L->curr->prev != NULL && L->curr->prev->data->second + 1 == pPair[0]) {
        L->curr->data->first = L->curr->prev->data->first;
        L->curr = L->curr->prev;
        deleteCurrNode(L);
      }
    }
    else if(L->curr->prev != NULL && L->curr->prev->data->second + 1 == pPair[0]) {
      L->curr->prev->data->second = pPair[1];
    }
    else {
      node *newNode = (node *)malloc(sizeof(node));
      newNode->data = (pair *)malloc(sizeof(pair));
      newNode->data->first = pPair[0];
      newNode->data->second = pPair[1];
      newNode->data->stamp = pPair[2];

      if(L->curr->prev == NULL) {
        L->curr->prev = newNode;
        newNode->next = L->curr;
        newNode->prev = NULL;
        L->head = newNode;
      }
      else {
        L->curr->prev->next = newNode;
        newNode->prev = L->curr->prev;
        newNode->next = L->curr;
        L->curr->prev = newNode;
      }
    }
  }
  else {
    if(L->curr->data->second + 1 == pPair[0]) {
      L->curr->data->second = pPair[1];
      if(L->curr->next != NULL && L->curr->next->data->first - 1 == pPair[1]) {
        L->curr->data->second = L->curr->next->data->second;
        L->curr = L->curr->next;
        deleteCurrNode(L);
      }
    }
    else if(L->curr->next != NULL && L->curr->next->data->first - 1 == pPair[1]) {
      L->curr->next->data->first = pPair[0];
    }
    else {
      node *newNode = (node *)malloc(sizeof(node));
      newNode->data = (pair *)malloc(sizeof(pair));
      newNode->data->first = pPair[0];
      newNode->data->second = pPair[1];
      newNode->data->stamp = pPair[2];

      if(L->curr->next == NULL) {
        L->curr->next = newNode;
        newNode->prev = L->curr;
        newNode->next = NULL;
        L->tail = newNode;
      }
      else {
        L->curr->next->prev = newNode;
        newNode->next = L->curr->next;
        newNode->prev = L->curr;
        L->curr->next = newNode;
      }
    }
  }
}

void freeNode(node *N) {
  free(N->data);
  free(N);
}

int deleteCurrNode(linkedList *L) {
  if(L->head == L->tail) {
    freeNode(L->curr);
    L->head = NULL;
    L->tail = NULL;
    L->curr = NULL;
    return 0;
  }
  else if(L->curr == L->head) {
    L->head = L->curr->next;
    L->head->prev = NULL;
    freeNode(L->curr);
    L->curr = L->head;
  }
  else if(L->curr == L->tail) {
    L->tail = L->curr->prev;
    L->tail->next = NULL;
    freeNode(L->curr);
    L->curr = L->tail;
  }
  else {
    node *p = L->curr->prev;
    p->next = p->next->next;
    p->next->prev = p;
    freeNode(L->curr);
    L->curr = p;
  }
  return 1;
}

void divideByMaxRange(linkedList *L) {
  UINT64 sSize;
  node *p = L->head;

  while(1) {
    sSize = p->data->second - p->data->first;
    if(sSize > MAX_RANGE_SIZE) {
      node *newNode = (node *)malloc(sizeof(node));
      newNode->data = (pair *)malloc(sizeof(pair));
      newNode->data->stamp = p->data->stamp;
      newNode->prev = p;

      if(p->next == NULL) {
        newNode->next = NULL;
        L->tail = newNode;
      }
      else {
        newNode->next = p->next;
        p->next->prev = newNode;
      }

      p->next = newNode;
      newNode->data->second = p->data->second;
      newNode->data->first = p->data->first + MAX_RANGE_SIZE;
      p->data->second = newNode->data->first - 1;
    }
    if(p->next == NULL) break;
    p = p->next;
  }
}

void sortByStamp(linkedList *L) {
  node *k = L->head;
  node *s = NULL;
  node *p = NULL;
  node *n = NULL;
  while(k->next != NULL) {
    UINT64 nextStamp = k->next->data->stamp;
    if(nextStamp < k->data->stamp) {
      s = k->next;

      if(k->prev != NULL) {
        p = k->prev;
        p->next = s;
      }
      else {
        p = NULL;
      }

      if(k->next->next != NULL) {
        n = k->next->next;
        n->prev = k;
      }
      else {
        n = NULL;
        L->tail = k;
      }

      k->prev = s;
      k->next = n;
      s->prev = p;
      s->next = k;

      if(p != NULL) k = p;
      else {
        k = s;
        L->head = k;
      }
    }
    else k = k->next;
  }
}

void printLinkedList(linkedList *L) {
  if(L->curr == NULL) return;
  sortByStamp(L);
  node *p = L->head;
  int index = 0;
  UINT64 sectors = 0;
  UINT64 mapper = 0;
  //printf(" L->curr  node - LBA: %lu ~ %lu\n", L->curr->data->first, L->curr->data->second);
  //printf(" L->head  node - LBA: %lu ~ %lu\n", L->head->data->first, L->head->data->second);
  //printf(" L->tail  node - LBA: %lu ~ %lu\n\n", L->tail->data->first, L->tail->data->second);
  printf("+=======++===========================+=========================+=========+\n");
  printf("|  idx  ||        byte offset        |           LBA           | sectors |\n");
  printf("+=======++===========================+=========================+=========+\n");
  while(1) {
    sectors = p->data->second - p->data->first + 1;
    printf("| %-5d || %-12lu~ %-12lu| %-11lu~ %-11lu| %7lu |\n", ++index, mapper * 512, (mapper + sectors) * 512 - 1, p->data->first, p->data->second, sectors);
    mapper += sectors;
    if(p->next == NULL) break;
    p = p->next;
  }
  printf("+=======++===========================+=========================+=========+\n");
  printf("\n");
}
