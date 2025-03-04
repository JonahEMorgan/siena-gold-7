#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define SPEED_TEST 10000

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

position positions[] = {
    3,  5,
    6,  8,
    7,  9,
    10, 12,
    11, 13,
    12, 14,
    15, 17,
    16, 18,
    17, 19,
    18, 20,
    6,  17,
    3,  12,
    7,  18,
    1,  8,
    4,  13,
    8,  19,
    0,  5,
    2,  9,
    5,  14,
    9,  20,
    9,  18,
    5,  12,
    8,  17,
    2,  7,
    4,  11,
    7,  16,
    0,  3,
    1,  6,
    3,  10,
    6,  1
};

const int totalMoves = 60;

move moves[totalMoves];

move makeMove(position from, position to) {
    position over = (from + to) / 2;
    return (move) {
        1 << from | 1 << over | 1 << to,
        1 << from | 1 << over,
        from + 1,
        to + 1
    };
}

void constructMoves() {
    for(int i = 0; i < totalMoves / 2; i++) {
        moves[i * 2] = makeMove(positions[i * 2], positions[i * 2 + 1]);
        moves[i * 2 + 1] = makeMove(positions[i * 2 + 1], positions[i * 2]);
    }
    for(int i = 0; i < totalMoves; i++) {
        for(int j = 0; j < totalMoves; j++) {
            move a = moves[i];
            move b = moves[j];
            if(a.from < b.from) {
                moves[i] = b;
                moves[j] = a;
            }
        }
    }
}

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
        for(int i = 0; i < totalMoves; i++) {
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
    constructMoves();
    FILE* file = fopen(argv[1], "r");
    board b = readBoard(file);
    puts("Input state:");
    printBoard(b);
    #ifdef SPEED_TEST
        clock_t start = clock();
        state* s;
        for(int i = 0; i < SPEED_TEST; i++) {
            s = findSolution(b);
        }
        float seconds = (float) (clock() - start) / CLOCKS_PER_SEC;
        printf("Solution found in %.3f milliseconds!\n", seconds * 1000 / SPEED_TEST);
    #else
        state* s = findSolution(b);
    #endif
    puts("Final state:");
    printBoard(s->b);
    int count = printState(s, 0);
    printf("%i\n", count);
    return 0;
}