#! /usr/bin/env python3

import os
import requests

DEFAULT_SOURCE_DIR = ("data", "feedback")
DEFAULT_URL = 'http://34.170.83.86/feedback/'

def sendFeedback(feedback):
    for fb_num  in feedback:
        #print(fb_num)
        response = requests.post(DEFAULT_URL, json=fb_num)
        #print(response.request.body)
        if not response.ok:
            raise Exception("Status: {} \n Comment: {}".format(response.status_code, response.reason))
        else:
            print ("Status: {} Feedback send".format(response.status_code))

def processFeedbackFiles(path_to_source):
    fb_dict = []
    for file in os.listdir(path_to_source):
        try:
            with open(os.path.join(path_to_source, file), "r") as workfile:        
                fb_dict.append({"title":workfile.readline().strip(),"name":workfile.readline().strip(),"date":workfile.readline().strip(),"feedback":workfile.readline().strip()})
        except:
            print("Cannot open file: {}".format(file))
            continue
    sendFeedback(fb_dict)
    # print(fb_dict)


def main():
    #source_dir = os.path.join(os.path.dirname(__file__), DEFAULT_SOURCE_DIR[0], DEFAULT_SOURCE_DIR[1])
    source_dir = os.path.join(os.sep,DEFAULT_SOURCE_DIR[0], DEFAULT_SOURCE_DIR[1])
    processFeedbackFiles(source_dir)
    
if __name__ == '__main__':
    main()
