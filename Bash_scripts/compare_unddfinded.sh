wget -q 'https://www.spinellis.gr/unix/data/a.out';
nm a.out | grep ' U ' | sed 's/U _*//;s/@@.*//' > ar.txt;
wget -q https://www.spinellis.gr/unix/data/socketpipe.o.gz;
gzip -dk socketpipe.o.gz;
nm socketpipe.o | grep ' U ' | sed 's/U _*//;s/@@.*//' > sr.txt;
comm -12 ar.txt sr.txt
