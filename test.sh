#!/bin/sh
set -e
./mustransc decode 'test/test1.mus' 'test/test1.out.txt'
./mustransc decode 'test/test2.mus' 'test/test2.out.bin'
./mustransc encode 'test/test3.txt' 'test/test3.out.mus'

{ cd 'test' && cksum -a sha256 --strict -c 'hashes.sha256'; }
