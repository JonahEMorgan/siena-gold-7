#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef int32_t r;

typedef int8_t s;

typedef struct {
    r k;
    r v;
    s f;
    s t;
} m;

typedef struct e {
    r b;
    m* m;
    struct e* p;
} e;

typedef struct q {
    e s;
    struct q* n;
} q;

m ms[] = {
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

bool h(r b, m m) {
    return (b & m.k) == m.v;
}

bool g(r b, s p) {
    return (b >> p) & 1;
}

r w(r b, m m) {
    b &=~ m.k;
    b |= ~m.v & m.k;
    return b;
}

r z(FILE* f) {
    s p;
    r b = 0;
    while(fscanf(f, "%hhd", &p) != EOF) {
        b |= 1 << (p - 1);
    }
    return b;
}

void d(r b) {
    for(int i = 0; i < 11; i++) {
        for(int j = i; j < 10; j++) {
            putchar(' ');
        }
        int j = i / 2;
        if(i % 2 == 0) {
            s p = j * (j + 1) / 2;
            putchar(g(b, p) ? 'O' : ' ');
            for(int k = 1; k <= j; k++) {
                printf("---%c", g(b, p + k) ? 'O' : ' ');
            }
        } else {
            for(int k = 0; k < j + 1; k++) {
                printf("/ \\ ");
            }
        }
        putchar('\n');
    }
}

e* l(r n) {
    q* s = malloc(sizeof(q));
    *s = (q) {
        {
            n,
            NULL,
            NULL
        },
        NULL
    };
    q* b = s;
    q* o = s;
    while(o) {
        q* u = o;
        o = o->n;
        if(!o) {
            b = NULL;
        }
        bool y = false;
        for(int i = 0; i < sizeof(ms) / sizeof(ms[0]); i++) {
            m* m = &ms[i];
            if(h(u->s.b, *m)) {
                y = true;
                q* n = malloc(sizeof(q));
                *n = (q) {
                    {
                        w(u->s.b, *m),
                        m,
                        &u->s
                    },
                    NULL
                };
                if(!o) {
                    o = b = n;
                } else {
                    b->n = n;
                    b = n;
                }
            }
        }
        if(!y) {
            return &u->s;
        }
    }
    return NULL;
}

int p(e* s, int c) {
    if(s->p) {
        c = p(s->p, c + 1);
    }
    if(s->m) {
        printf("%i-%i\n", s->m->f, s->m->t);
    }
    return c;
}

int main(int c, char** g) {
    if(c < 2) {
        puts("Specify input file");
        return -1;
    }
    FILE* f = fopen(g[1], "r");
    r b = z(f);
    puts("Input e:");
    d(b);
    e* s = l(b);
    puts("Final e:");
    d(s->b);
    int u = p(s, 0);
    printf("%i\n", u);
    return 0;
}