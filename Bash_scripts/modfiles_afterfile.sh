#!/bin/sh
file=$HOME/Documents/kernel/events/core.c
find $HOME/Documents/kernel -type f |
while read f; do
	if [ $f -nt $file ] ; then
	ls -la $f
	fi
done |
wc -l
ls -la $file
