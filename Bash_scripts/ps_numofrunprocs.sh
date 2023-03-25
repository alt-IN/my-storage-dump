curl -s 'https://www.spinellis.gr/unix/data/ps.out'| awk -F' ' '{print $6}' | grep 'R' | wc -l
