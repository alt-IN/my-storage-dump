git log --pretty=format:%ad | cut -d' ' -f2,5 | sort -k2nr | while read dat; do if test "$dat" = "Aug 2019" ; then echo $dat; fi; done | wc -l
