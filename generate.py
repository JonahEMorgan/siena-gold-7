positions = [
    (3,  5),
    (6,  8),
    (7,  9),
    (10, 12),
    (11, 13),
    (12, 14),
    (15, 17),
    (16, 18),
    (17, 19),
    (18, 20),

    (6, 17),
    (3, 12),
    (7, 18),
    (1, 8),
    (4, 13),
    (8, 19),
    (0, 5),
    (2, 9),
    (5, 14),
    (9, 20),

    (9, 18),
    (5, 12),
    (8, 17),
    (2, 7),
    (4, 11),
    (7, 16),
    (0, 3),
    (1, 6),
    (3, 10),
    (6, 15)
]


def move(begin, end):
    over = (begin + end) // 2
    return (
        1 << begin | 1 << over | 1 << end,
        1 << begin | 1 << over
    )


print("move moves[] = {")

moves = []
triangle = True

for p in positions:
    a = move(p[0], p[1])
    b = move(p[1], p[0])
    if triangle:
        moves.append(f"    {{{a[0]},{a[1]},{p[0] + 1},{p[1] + 1}}},")
        moves.append(f"    {{{b[0]},{b[1]},{p[1] + 1},{p[0] + 1}}},")
    else:
        moves.append(f"    {{{hex(a[0])}, {hex(a[1])}, {p[0] + 1}, {p[1] + 1}}},")
        moves.append(f"    {{{hex(b[0])}, {hex(b[1])}, {p[1] + 1}, {p[0] + 1}}},")

moves.sort(key=lambda m: (len(m), m.index(","), m))

for move in moves:
    print(move)

print("};")
