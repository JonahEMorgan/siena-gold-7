#include <stdio.h>
#include <stdint.h>
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

s x[] = {
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

m j[60];

m h(s f, s t) {
    s o = (f + t) / 2;
    return (m) {
        1 << f | 1 << o | 1 << t,
        1 << f | 1 << o,
        f + 1,
        t + 1
    };
}

bool g(r b, s p) {
    return (b >> p) & 1;
}

void d(r b) {
    for(int i = 0; i < 11; i++) {
        for(int j = i; j < 10; j++)
            putchar(' ');
        int j = i / 2;
        if(i % 2 == 0) {
            s p = j * (j + 1) / 2;
            putchar(g(b, p) ? 'O' : ' ');
            for(int k = 1; k <= j; k++)
                printf("---%c", g(b, p + k) ? 'O' : ' ');
        } else
            for(int k = 0; k < j + 1; k++)
                printf("/ \\ ");
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
        if(!o)
            b = NULL;
        int y = 0;
        for(int i = 0; i < 60; i++) {
            m* m = &j[i];
            if((u->s.b & m->k) == m->v) {
                y = 1;
                q* n = malloc(sizeof(q));
                *n = (q) {
                    {
                        u->s.b & ~m->k | (~m->v & m->k),
                        m,
                        &u->s
                    },
                    NULL
                };
                if(!o)
                    o = b = n;
                else {
                    b->n = n;
                    b = n;
                }
            }
        }
        if(!y)
            return &u->s;
    }
    return NULL;
}

int y(e* s, int c) {
    if(s->p)
        c = y(s->p, c + 1);
    if(s->m)
        printf("%i-%i\n", s->m->f, s->m->t);
    return c;
}

int main(int c, char** g) {
    if(c < 2) {
        puts("Specify input file");
        return -1;
    }
    for(int i = 0; i < 30; i++) {
        j[i * 2] = h(x[i * 2], x[i * 2 + 1]);
        j[i * 2 + 1] = h(x[i * 2 + 1], x[i * 2]);
    }
    for(int i = 0; i < 60; i++)
        for(int k = 0; k < 60; k++) {
            m a = j[i];
            m b = j[k];
            if(a.f < b.f) {
                j[i] = b;
                j[k] = a;
            }
        }
    FILE* f = fopen(g[1], "r");
    s p;
    r b = 0;
    while(fscanf(f, "%hhd", &p) != EOF)
        b |= 1 << (p - 1);
    puts("Input state:");
    d(b);
    e* s = l(b);
    puts("Final state:");
    d(s->b);
    int u = y(s, 0);
    printf("%i\n", u);
    return 0;
}