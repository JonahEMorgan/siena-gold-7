#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef int32_t board;

typedef int8_t position;

typedef struct {
    board mask;
    board move;
    position from;
    position to;
} move;

typedef struct state {
    board b;
    move* m;
    struct state* previous;
} state;

typedef struct queue {
    state s;
    struct queue* next;
} queue;

move moves[] = {
    {0xb, 0x3, 1, 4},
    {0xb, 0xa, 4, 1},
    {0x25, 0x5, 1, 6},
    {0x4a, 0xa, 2, 7},
    {0x25, 0x24, 6, 1},
    {0x38, 0x18, 4, 6},
    {0x38, 0x30, 6, 4},
    {0x4a, 0x48, 7, 2},
    {0x94, 0x14, 3, 8},
    {0x94, 0x90, 8, 3},
    {0x112, 0x12, 2, 9},
    {0x1c0, 0xc0, 7, 9},
    {0x112, 0x110, 9, 2},
    {0x1c0, 0x180, 9, 7},
    {0x224, 0x24, 3, 10},
    {0x448, 0x48, 4, 11},
    {0x890, 0x90, 5, 12},
    {0x224, 0x220, 10, 3},
    {0x380, 0x180, 8, 10},
    {0x380, 0x300, 10, 8},
    {0x448, 0x440, 11, 4},
    {0x890, 0x880, 12, 5},
    {0x1088, 0x88, 4, 13},
    {0x1120, 0x120, 6, 13},
    {0x2110, 0x110, 5, 14},
    {0x4220, 0x220, 6, 15},
    {0x8440, 0x440, 7, 16},
    {0x1088, 0x1080, 13, 4},
    {0x1120, 0x1100, 13, 6},
    {0x1c00, 0xc00, 11, 13},
    {0x2110, 0x2100, 14, 5},
    {0x4220, 0x4200, 15, 6},
    {0x8440, 0x8400, 16, 7},
    {0x10880, 0x880, 8, 17},
    {0x20840, 0x840, 7, 18},
    {0x1c00, 0x1800, 13, 11},
    {0x3800, 0x1800, 12, 14},
    {0x3800, 0x3000, 14, 12},
    {0x7000, 0x3000, 13, 15},
    {0x7000, 0x6000, 15, 13},
    {0x21100, 0x1100, 9, 18},
    {0x41080, 0x1080, 8, 19},
    {0x82100, 0x2100, 9, 20},
    {0x10880, 0x10800, 17, 8},
    {0x20840, 0x20800, 18, 7},
    {0x21100, 0x21000, 18, 9},
    {0x41080, 0x41000, 19, 8},
    {0x42200, 0x2200, 10, 19},
    {0x82100, 0x82000, 20, 9},
    {0x38000, 0x18000, 16, 18},
    {0x38000, 0x30000, 18, 16},
    {0x42200, 0x42000, 19, 10},
    {0x70000, 0x30000, 17, 19},
    {0x70000, 0x60000, 19, 17},
    {0xe0000, 0x60000, 18, 20},
    {0xe0000, 0xc0000, 20, 18},
    {0x104200, 0x4200, 10, 21},
    {0x1c0000, 0xc0000, 19, 21},
    {0x104200, 0x104000, 21, 10},
    {0x1c0000, 0x180000, 21, 19},
};

bool hasMove(board b, move m) {
    return (b & m.mask) == m.move;
}

bool get(board b, position p) {
    return (b >> p) & 1;
}

board playMove(board b, move m) {
    b &=~ m.mask;
    b |= ~m.move & m.mask;
    return b;
}

board readBoard(FILE* file) {
    position p;
    board b = 0;
    while(fscanf(file, "%hhd", &p) != EOF) {
        b |= 1 << (p - 1);
    }
    return b;
}

void printBoard(board b) {
    for(int i = 0; i < 11; i++) {
        for(int j = i; j < 10; j++) {
            putchar(' ');
        }
        int j = i / 2;
        if(i % 2 == 0) {
            position p = j * (j + 1) / 2;
            putchar(get(b, p) ? 'O' : ' ');
            for(int k = 1; k <= j; k++) {
                printf("---%c", get(b, p + k) ? 'O' : ' ');
            }
        } else {
            for(int k = 0; k < j + 1; k++) {
                printf("/ \\ ");
            }
        }
        putchar('\n');
    }
}

state* findSolution(board b) {
    queue* start = malloc(sizeof(queue));
    *start = (queue) {
        {
            b,
            NULL,
            NULL
        },
        NULL
    };
    queue* back = start;
    queue* front = start;
    while(front) {
        queue* q = front;
        front = front->next;
        if(!front) {
            back = NULL;
        }
        bool anyMove = false;
        for(int i = 0; i < sizeof(moves) / sizeof(moves[0]); i++) {
            move* m = &moves[i];
            if(hasMove(q->s.b, *m)) {
                anyMove = true;
                queue* new = malloc(sizeof(queue));
                *new = (queue) {
                    {
                        playMove(q->s.b, *m),
                        m,
                        &q->s
                    },
                    NULL
                };
                if(!front) {
                    front = back = new;
                } else {
                    back->next = new;
                    back = new;
                }
            }
        }
        if(!anyMove) {
            return &q->s;
        }
    }
    return NULL;
}

int printState(state* s, int count) {
    if(s->previous) {
        count = printState(s->previous, count + 1);
    }
    if(s->m) {
        printf("%i-%i\n", s->m->from, s->m->to);
    }
    return count;
}

int main(int argc, char** argv) {
    if(argc < 2) {
        puts("Specify input file");
        return -1;
    }
    FILE* file = fopen(argv[1], "r");
    board b = readBoard(file);
    puts("Input state:");
    printBoard(b);
    state* s = findSolution(b);
    puts("Final state:");
    printBoard(s->b);
    int count = printState(s, 0);
    printf("%i\n", count);
    return 0;
}