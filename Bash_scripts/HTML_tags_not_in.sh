comm -23 <(curl -s 'https://www.spinellis.gr/unix/data/unix.html.gz' | gzip -dc | egrep -o '<[^ >]*' | sed 's/<//g
s/\///g
s/^[^a-zA-Z].*$//g' | sort -u) <(curl -s 'https://www.spinellis.gr/unix/data/allowed-tags'| sort)
