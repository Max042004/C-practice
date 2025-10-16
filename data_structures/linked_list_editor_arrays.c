#include <stdio.h>
#include <string.h>


#define MAXN 2000005 

static int L[MAXN], R[MAXN];
static int Val[MAXN];

static int head_idx = 0, tail_idx = 0;
static int max_id = 0;

static inline void detach(int k) {
    int a = L[k], b = R[k];
    if (a) R[a] = b; else head_idx = b;
    if (b) L[b] = a; else tail_idx = a;
    L[k] = R[k] = 0;
}

static inline void push_head(int id) {
    if (!head_idx) {
	    head_idx = tail_idx = id; L[id] = R[id] = 0;
	    return;
    }
    L[id] = 0; R[id] = head_idx; L[head_idx] = id; head_idx = id;
}

static inline void push_tail(int id) {
    if (!tail_idx) { 
	    head_idx = tail_idx = id; L[id] = R[id] = 0; 
	    return; 
    }
    R[tail_idx] = id; L[id] = tail_idx; R[id] = 0; tail_idx = id;
}

static inline void insert_after(int k, int id) {
    if (!head_idx) { head_idx = tail_idx = id; 
	    L[id] = R[id] = 0; return; 
    }
    if (k == tail_idx) { push_tail(id); return; }
    int b = R[k];
    R[k] = id; L[id] = k;
    R[id] = b; L[b] = id;
}

static inline void insert_before(int k, int id) {
    if (!head_idx) { head_idx = tail_idx = id; 
	    L[id] = R[id] = 0; return; 
    }
    if (k == head_idx) { push_head(id); return; }
    int a = L[k];
    R[a] = id; L[id] = a;
    R[id] = k; L[k] = id;
}

int main(void) {
    int N, Q;
    if (scanf("%d %d", &N, &Q) != 2) return 0;

    head_idx = (N ? 1 : 0);
    tail_idx = N;
    max_id = N;

    for (int i = 1; i <= N; ++i) {
        int x; scanf("%d", &x);
        Val[i] = x;
        L[i] = (i == 1 ? 0 : i - 1);
        R[i] = (i == N ? 0 : i + 1);
    }

    char op[8];
    for (int qi = 0; qi < Q; ++qi) {
        if (scanf("%7s", op) != 1) return 0;
        if (op[0] == 'H' && op[1] == '\0') {
            int x; scanf("%d", &x);
            int id = ++max_id; Val[id] = x;
            push_head(id);
        } else if (op[0] == 'T' && op[1] == '\0') {
            int x; scanf("%d", &x);
            int id = ++max_id; Val[id] = x;
            push_tail(id);
        } else if (op[0] == 'A' && op[1] == '\0') {
            int k, x; scanf("%d %d", &k, &x);
            int id = ++max_id; Val[id] = x;
            insert_after(k, id);
        } else if (op[0] == 'B' && op[1] == '\0') {
            int k, x; scanf("%d %d", &k, &x);
            int id = ++max_id; Val[id] = x;
            insert_before(k, id);
        } else if (op[0] == 'D' && op[1] == '\0') {
            int k; scanf("%d", &k);
            if (k) detach(k);
        } else if (op[0] == 'M' && op[1] == 'H') { // move to head
            int k; scanf("%d", &k);
            if (k != head_idx) { detach(k); push_head(k); }
        } else if (op[0] == 'M' && op[1] == 'T') { // move to tail
            int k; scanf("%d", &k);
            if (k != tail_idx) { detach(k); push_tail(k); }
        } else {
        }
    }

    if (!head_idx) {
        printf("Empty\nEmpty");
        return 0;
    }

    // forward
    for (int i = head_idx; i; i = R[i]) {
        printf("%d ", Val[i]);
    }
    printf("\n");
    // backward
    for (int i = tail_idx; i; i = L[i]) {
        printf("%d ", Val[i]);
    }
    return 0;
}
