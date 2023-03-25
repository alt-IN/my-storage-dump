curl -s 'https://www.spinellis.gr/unix/data/Troilus-Book-I.txt' | tr -c a-zA-Z'\n'' ' '!' | sed 's/!//g' | awk '{print $NF}' | egrep '^.{4,}$' | grep -o '...$' | sort | uniq -c | sort -k 1rn > 3c 

cat 2c 3c 4c | sort -k 1rn | sed -n '/ sse$/,/ le$/p'
