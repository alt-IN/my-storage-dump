curl -s 'https://www.spinellis.gr/unix/data/nasa-web-log.gz' | gzip -dc | cut -f 1,7 | awk -F . '{print $NF}' | awk '{size[$1]+=$2} END{ for (i in size) print i,size[i]}' | sort -k 2rn | sed -n 2p
