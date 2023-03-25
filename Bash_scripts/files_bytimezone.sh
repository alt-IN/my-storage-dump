#!/bin/sh
cd /home/pc/Documents/the-art-of-command-line;
ls README-??.md |
	while read file; do
		tz=$(git blame --line-porcelain $file | grep '^committer-tz' | sort | uniq -c | sort -rn | head -1 | cut -d' ' -f 7)
		if [ "$tz" = "+0100" ]; then
			echo $tz
		fi
done |
	wc -l
