#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

int is_valid(Node *n) {
  int seen[10];
  // Verificar filas
  for (int i = 0; i < 9; i++) {
      for (int k = 0; k < 10; k++) seen[k] = 0;
      for (int j = 0; j < 9; j++) {
          int num = n->sudo[i][j];
          if (num != 0) {
              if (seen[num]) return 0;
              seen[num] = 1;
          }
      }
  }
  // Verificar columnas
  for (int j = 0; j < 9; j++) {
      for (int k = 0; k < 10; k++) seen[k] = 0;
      for (int i = 0; i < 9; i++) {
          int num = n->sudo[i][j];
          if (num != 0) {
              if (seen[num]) return 0;
              seen[num] = 1;
          }
      }
  }
  // Verificar submatrices 3x3
  for (int k = 0; k < 9; k++) {
      for (int m = 0; m < 10; m++) seen[m] = 0;
      for (int p = 0; p < 9; p++) {
          int i = 3 * (k / 3) + (p / 3);
          int j = 3 * (k % 3) + (p % 3);
          int num = n->sudo[i][j];
          if (num != 0) {
              if (seen[num]) return 0;
              seen[num] = 1;
          }
      }
  }
  return 1;
}

List* get_adj_nodes(Node* n) {
  List* list = createList();
  int found = 0;
  for (int i = 0; i < 9 && !found; i++) {
      for (int j = 0; j < 9 && !found; j++) {
          if (n->sudo[i][j] == 0) {
              found = 1;
              for (int num = 1; num <= 9; num++) {
                  Node* new_node = copy(n);
                  new_node->sudo[i][j] = num;
                  if (is_valid(new_node)) {
                      pushBack(list, new_node);
                  } else {
                      free(new_node);
                  }
              }
          }
      }
  }
  return list;
}


int is_final(Node *n) {
  for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
          if (n->sudo[i][j] == 0) return 0;
      }
  }
  return 1;
}

Node* DFS(Node* initial, int* cont) {
  Stack* s = (Stack*) createStack();
  push(s, initial);
  *cont = 0;

  while (top(s) != NULL) {
      Node* current = top(s);
      pop(s);
      (*cont)++;

      if (is_final(current)) {
          // Liberar nodos restantes en la pila
          while (top(s) != NULL) {
              Node* temp = top(s);
              pop(s);
              free(temp);
          }
          free(s);
          return current;
      }

      List* adj = get_adj_nodes(current);
      Node* adj_node = NULL;
      while ((adj_node = ((Node*) popFront(adj))) != NULL) {
          push(s, adj_node);
      }

      free(current);
      free(adj);
  }

  free(s);
  return NULL;
}


/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}*/