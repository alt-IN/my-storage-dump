#!/usr/bin/env python3

import re
import sys
import csv
import operator

error_dict = {}
usercount_dict= {}

with open(sys.argv[1], 'r') as file:
    for row in file:
        if "ERROR" in row:
            result=re.search(r"\s(\w+): [A-Z]+ ([A-Za-z0-9 ']+) \(([\w.0-9]+)\)", row)
            
            error_dict[result.group(2)]=error_dict.get(result.group(2),0) + 1
                        
            usercount_dict[result.group(3)]=usercount_dict.get(result.group(3),[0,0])
            usercount_dict[result.group(3)][1] += 1
            

        elif "INFO" in row:
            result=re.search(r"\(([\w.0-9]+)\)",row)
            
            usercount_dict[result.group(1)]=usercount_dict.get(result.group(1),[0,0])
            usercount_dict[result.group(1)][0] += 1


    file.close()

#sorted(usercount_dict.items(), key=operator.itemgetter(0))
#sorted(error_dict.items(), key=operator.itemgetter(1), reverse=True)


with open('error_message.csv', 'w', encoding='UTF-8', newline='') as file:
    fieldname = ['Error', 'Count']
    writer=csv.writer(file)
    writer.writerow(fieldname)
    writer.writerows(sorted(error_dict.items(), key=operator.itemgetter(1), reverse=True))
    file.close()

with open('user_statistics.csv', 'w', encoding='UTF-8', newline='') as file:
    fieldname = ['Username', 'INFO', 'ERROR']
    writer=csv.DictWriter(file, fieldnames=fieldname)
    writer.writeheader()
    s_user_dict= sorted(usercount_dict.items(), key=operator.itemgetter(0))
    w_dict = []
    for key in s_user_dict:
        w_dict.append({'Username':key[0], 'INFO':key[1][0], 'ERROR':key[1][1]})
    writer.writerows(w_dict)
    file.close() 