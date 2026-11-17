#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

typedef struct Node {
    int v;
    struct Node *next;
} Node;

int n;          
char **G;       
Node **L;       
char *used;     
int *stk, *pos; 


void dfsRec(int v) {
    used[v] = 1;
    printf("%d ", v + 1);
    for (int i = 0; i < n; i++)
        if (G[v][i] && !used[i])
            dfsRec(i);
}

void dfsIter(int s) {
    int top = 0;
    used[s] = 1;
    printf("%d ", s + 1);
    stk[top++] = s;
    while (top) {
        int v = stk[top - 1];
        while (pos[v] < n && !(G[v][pos[v]] && !used[pos[v]]))
            pos[v]++;
        if (pos[v] < n) {
            int i = pos[v];
            used[i] = 1;
            printf("%d ", i + 1);
            stk[top++] = i;
        } else
            top--;
    }
}

void dfsList(int v) {
    used[v] = 1;
    printf("%d ", v + 1);
    for (Node *p = L[v]; p; p = p->next)
        if (!used[p->v])
            dfsList(p->v);
}


void run(void (*dfs)(int)) {
    memset(used, 0, n);
    for (int v = 0; v < n; v++)
        if (!used[v])
            dfs(v);
    printf("\n");
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    srand((unsigned)time(NULL));

    do {
        printf("Введите число вершин: ");
        if (scanf("%d", &n) != 1) {
            while (getchar() != '\n');
            n = 0;
        }
    } while (n < 1);

    G = malloc(n * sizeof(char *));
    L = calloc(n, sizeof(Node *));
    used = malloc(n);
    stk = malloc(n * sizeof(int));
    pos = calloc(n, sizeof(int));
    for (int i = 0; i < n; i++)
        G[i] = calloc(n, 1);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < i; j++)
            G[i][j] = G[j][i] = rand() % 2;

    printf("\nМатрица смежности:\n    ");
    for (int j = 0; j < n; j++)
        printf("%3d", j + 1);
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("%3d ", i + 1);
        for (int j = 0; j < n; j++)
            printf("%3d", G[i][j]);
        printf("\n");
    }

    printf("\nОбход в глубину (рекурсивный, матрица): ");
    run(dfsRec);
    printf("Обход в глубину (нерекурсивный, матрица): ");
    run(dfsIter);

    for (int i = 0; i < n; i++)
        for (int j = n - 1; j >= 0; j--)
            if (G[i][j]) {
                Node *p = malloc(sizeof(Node));
                p->v = j;
                p->next = L[i];
                L[i] = p;
            }

    printf("\nСписки смежности:\n");
    for (int i = 0; i < n; i++) {
        printf("%3d:", i + 1);
        for (Node *p = L[i]; p; p = p->next)
            printf(" %d", p->v + 1);
        printf("\n");
    }

    printf("\nОбход в глубину (списки смежности): ");
    run(dfsList);

    for (int i = 0; i < n; i++) {
        for (Node *p = L[i], *t; p; p = t) {
            t = p->next;
            free(p);
        }
        free(G[i]);
    }
    free(G);
    free(L);
    free(used);
    free(stk);
    free(pos);
    return 0;
}
