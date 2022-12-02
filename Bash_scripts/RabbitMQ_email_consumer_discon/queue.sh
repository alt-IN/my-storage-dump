#!/bin/bash

#FILE WITH PARAMETERS

source usr/bin/parameters.settings

#LOOP FOR RESTARTING REQUEST

while :
do

#GET LIST OF BINDS IN EXCHANGE (LIST OF QUEUES)

BindEx=$(curl -s -u $USER:$PASS -H "content-type:application/json"     -XGET "http://$URL/api/exchanges/$VHOST/InR001/bindings/source" | python -m json.tool |  grep -o '"destination": ".*"' | sed -e 's/"destination": "\(.*\)"/\1/');

#GET LIST OF ACTIVE RABBIT CONSUMERS AND THEIR  QUEUES 

BindQu=$(curl -s -u $USER:$PASS -H "content-type:application/json"     -XGET "http://$URL/api/consumers" | python -m json.tool | grep -o '"name": ".*"' |  sed -e 's/"name": "\(.*\)"/\1/');

DATE=`date '+%Y-%m-%d %H:%M:%S'`

#COMPARING LIST OF QUEUES FROM ACTIVE CONSUMERS AND LIST OF QUEUES FROM EXCHANGE

if [ $(comm -12 <(echo "$BindEx" | sort) <(echo "$BindQu" | sort) | wc -l) = $(echo "$BindEx" | wc -l) ]; then
echo "$DATE Consumers match" >> /var/log/sxrcmonitor.log
else
echo -e "$DATE Consumers doesnt match for queue:\n$(comm -23 <(echo "$BindEx" | sort) <(echo "$BindQu" | sort))" >> /var/log/sxrcmonitor.log

#SEND MAIL IF CONSUMERS DOESNT MATCH (require MAILX packet or other to be installed)

mail -s 'LOST CONNET FOR R001' $MAILRECIEVER <<EOF 2> /dev/null;
LOST CONNECT TO QUEUES: $(comm -23 <(echo "$BindEx" | sort) <(echo "$BindQu" | sort))
EOF

fi;

unset BindEx BindQu;

sleep $RESTARTTIME;

done
