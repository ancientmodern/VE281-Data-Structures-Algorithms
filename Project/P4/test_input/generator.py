import random

SIZE = 8192
FACTOR = 0.5
SOURCE_NODE = 342
DEST_NODE = 4982

with open('test.txt', 'w') as f:
    f.write(str(SIZE) + '\n' + str(SOURCE_NODE) + '\n' + str(DEST_NODE) + '\n')
    for i in range(int(SIZE * FACTOR)):
        f.write(str(random.randint(0, SIZE - 1)) + ' ' +
                str(random.randint(0, SIZE - 1)) + ' ' +
                str(random.randint(0, int(SIZE / FACTOR))))
        if i < int(SIZE * FACTOR) - 1:
            f.write('\n')
