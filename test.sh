#!/bin/sh

gcc -O3 -o ./main main.c

./main decode mellohi.mus mellohi.ogg mellohi.mus
./main encode mellohi.ogg mellohi.mus.2 mellohi.mus

sha256sum mellohi.mus mellohi.mus.2

hexdump -C mellohi.mus | head > mellohi.mus.hex
hexdump -C mellohi.mus.2 | head > mellohi.mus.2.hex
git diff --no-index mellohi.mus.hex mellohi.mus.2.hex
