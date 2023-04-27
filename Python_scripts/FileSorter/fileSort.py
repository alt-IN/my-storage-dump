#!/usr/bin/env python3

import tkinter as tk
import os
#import re
import datetime
from PIL import ImageTk, Image


def get_files() -> dict:
    erLabel['text'] =  ""
    global path
    temp = pathEntry.get()
    if temp == '': 
        path = os.path.abspath(os.path.dirname(__file__))
        # print(path)
    else:
        if not os.path.exists(temp):
            erLabel['text'] = "Incorrect path!"
            return None
        path = temp
        
    fileList = [f for f in os.listdir(path) if os.path.isfile(os.path.join(path, f)) and f != os.path.basename(__file__)]
    fileDict = {}
    for f in fileList: 
        modDate = os.stat(os.path.join(path,f)).st_mtime
        modDate = datetime.datetime.utcfromtimestamp(modDate).strftime('%d_%m_%Y')
        fileDict[modDate] = fileDict.get(modDate, [])
        fileDict[modDate].append(f)
    
    return fileDict


def proceed_func() -> None:
    files = get_files()
    for k in files:
        create_dir(k)
        move_files(k, files[k])


def create_dir(dateVal) -> None:
    global path
    try:
        os.mkdir(os.path.join(path, dateVal))
    except:
        print(f"Cannot create directory:{os.path.join(path, dateVal)}")



def move_files(dir: str, fileList: tuple) -> None: 
    global path
    for name in fileList:
        try:
            os.replace(os.path.join(path,name), os.path.join(path,dir,name))
        except:
            print("Cannot move file {}".format(name))



if __name__ == '__main__':

    path = None
    rootWindow = tk.Tk()
    icon = ImageTk.PhotoImage(Image.open("fdi.jpg"))
    rootWindow.title("File Sorter")
    rootWindow.geometry("500x250")
    rootWindow.resizable(0, 0)
    rootWindow.iconphoto(False, icon)

    lFrame = tk.Frame(rootWindow, width=100)
    cFrame = tk.Frame(rootWindow)
    lFrame.pack(side=tk.LEFT, fill=tk.Y)
    img = ImageTk.PhotoImage(Image.open("sort_cur.jpg").resize((100, 250)))
    imgLabel = tk.Label(lFrame, image=img)
    imgLabel.pack(fill=tk.BOTH, expand=1)

    pathLabel = tk.Label(
        cFrame, text="Enter path to directiory (by default: current directory):")
    pathEntry = tk.Entry(cFrame)
    cFrame.pack(side=tk.LEFT, fill=tk.BOTH, expand=1)
    pathLabel.pack(anchor='n', pady=5)
    pathEntry.pack(anchor='n', fill=tk.X, padx=10)
    selBut = tk.Button(cFrame, text='ChooseDir', width=20, bd=3)
    selBut.pack(pady=5)

    erLabel = tk.Label(cFrame, fg='#FF0000')
    erLabel.pack(pady=15)
    goBut = tk.Button(cFrame, text='Run', width=20, command=proceed_func, bd=3)
    goBut.pack(side=tk.LEFT, anchor='s', padx=10,  pady=10)

    imgCl= ImageTk.PhotoImage(Image.open("close.png").resize((30, 30)))
    closeBut = tk.Button(cFrame, image=imgCl, borderwidth=0, relief='raised',  justify=tk.CENTER, height=30, width=30,  command = rootWindow.destroy)
    closeBut.pack(anchor='s', side = tk.RIGHT, padx=10, pady=10)
    rootWindow.mainloop()


