#!/bin/sh

for i in `ldd src/setup | cut -d ">" -f 2 | cut -d "(" -f 1` ; do
  PAKET=`rpm -qf $i`
  echo "$PAKET"
done

