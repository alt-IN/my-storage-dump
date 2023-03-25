#!/bin/sh
if test -d ~/Documents/Desktop ; then 
	rm -rf ~/Documents/Desktop
fi ;
 
mkdir ~/Documents/Desktop && cd ~/Documents/Desktop ;
curl https://www.spinellis.gr/unix/data/hier.txt > 10.txt ;
cat 10.txt |
while read action ; do
	command=$(echo $action | cut -d' ' -f1)  
	file=$(echo $action | cut -d' ' -f2) 
#	echo $file $command
	if test "$command" = "create-file" ; then 
#		echo $command $file
		touch $file
	elif test "$command" = "delete-file" ; then 
		rm $file
	elif test "$command" = "create-directory" ; then
		mkdir $file
	elif test "$command" = "delete-directory" ; then
		rm -rf $file
	fi

#	echo $command+3

# if [[ $action != "$str" ]]


#	if []; then 
#		cut --delimeter=' ' -f2 $action 
#	fi
done ;
rm 10.txt;
ls | wc -l;

#cut --delimiter=' ' -f1 < 10.txt

