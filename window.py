from tkinter import *
from tkinter import filedialog
from tkinter import StringVar
import os, random
import readimg, writetxt

desktop = os.path.join(os.path.join(os.environ['USERPROFILE']), 'Desktop')
# file names 
fnames = []
# output dirctory
outputDir = desktop + '\\'

### Slots ###
#
# browse images
def browseImages():
    global inputList
    global fnames
    global mainwindow
    new = filedialog.askopenfilenames(initialdir = desktop, \
        title = "Select Image", \
        filetypes = (("PNG images", "*.png"), ("JPEG images", "*.jpg *.jpeg")))
    for name in new:
        if name not in fnames:
            fnames.append(name)
            inputList.insert(END, os.path.basename(name))
    return

def browseOutputDir():
    global outputDirEntry
    dir = filedialog.askdirectory(initialdir = desktop, \
        title = "Select Output Directory")
    outputDirEntry.delete(0, END)
    for c in dir:
        outputDirEntry.insert(END, c)
    pass

### Slots ###

def getOutputPath():
    return outputDirEntry.get()

# @return 
#  [
#   numeric list -> list,
#   extend name list -> list,
#   directory name list -> list
#  ]
def sortWithExtDir(numericlist, extendnames, dirnames):
    if 1 >= len(numericlist):
        return [numericlist, extendnames, dirnames]
    compare = numericlist[0]
    numericlist.pop(0)
    fstExt = extendnames.pop(0)
    fstDir = dirnames.pop(0)
    small = []
    large = []
    extSmall = []; dirSmall = []
    extLarge = []; dirLarge = []
    for i in range(len(numericlist)):
        if compare > numericlist[i]:
            small += [numericlist[i]]
            extSmall += [extendnames[i]]
            dirSmall += [dirnames[i]]
        else:
            large += [numericlist[i]]
            extLarge += [extendnames[i]]
            dirLarge += [dirnames[i]]
    return [small + [compare] + large,\
            extSmall + [fstExt] + extLarge,\
            dirSmall + [fstDir] + dirLarge]
        

### Process ###
#

# separate the Lists
def separateListWithRatio(denominator, wordlists):
    lists = []
    while 1 <= len(wordlists) / denominator:
        lists.append(wordlists[0:denominator])
        wordlists = wordlists[denominator:]
    else: 
        if 0 != len(wordlists) % denominator:
            lists.append(wordlists)
    return lists

# random pick from lists
def randomPickLists(numerator, wordlists):
    lists = []
    print(len(wordlists[-1]))
    for singleList in wordlists:
        outputList = []
        lenth = numerator
        if len(singleList) < numerator:
            lenth = len(singleList)
        for i in range(lenth):
            chosenIndext = random.choice(range(len(singleList)))
            outputList.append(singleList[chosenIndext])
            singleList.pop(chosenIndext)
        lists.append(outputList)
    return lists

# use tesseract to ocr the image
def ocrImages(filenames):
    # read images
    lists = readimg.readImages2List(desktop, filenames)
    denominator = int(denominator_wordChoseRatioSpinbox.get())
    numerator = int(numerator_wordChoseRatioSpinbox.get())
    # random picking 
    separatedList = separateListWithRatio(denominator, lists)
    return randomPickLists(numerator, separatedList)
    
# main export
# Slot
def main_export():
    ## set file names in numeric sequence
    ## also make the full path
    absoluteFullPath = []
    sequencialFileNames = list(fnames)
    extendnames = []
    path = []
    # delete all extend name
    for i in range(len(sequencialFileNames)):
        extendnames.append("")
        while '.' != sequencialFileNames[i][-1]:
            extendnames[i] += sequencialFileNames[i][-1]
            sequencialFileNames[i] = sequencialFileNames[i][0:-1]   # delete the last character
        sequencialFileNames[i] = sequencialFileNames[i][0:-1]   # remove the dot
        extendnames[i] += '.'                                      # append the extend name dot
        extendnames[i] = extendnames[i][::-1]                         # reverse the string
        path.append(os.path.dirname(sequencialFileNames[i]))    # get the dir path
        sequencialFileNames[i] = os.path.basename(sequencialFileNames[i])   # base name only
        sequencialFileNames[i] = int(sequencialFileNames[i])                # change to integer
    sortedlist = sortWithExtDir(sequencialFileNames, extendnames, path)
    sequencialFileNames = sortedlist[0]
    extendnames = sortedlist[1]
    path = sortedlist[2]
    del sortedlist
    for i in range(len(sequencialFileNames)):
        # int to str
        sequencialFileNames[i] = str(sequencialFileNames[i])
        absoluteFullPath.append(sequencialFileNames[i])
        # append the extend name
        absoluteFullPath[i] += extendnames[i]
        # make the full path
        absoluteFullPath[i] = os.path.join(path[i], absoluteFullPath[i]).replace('\\', '/')
    
    ## ocr
    # read img and get the lists
    lists = ocrImages(absoluteFullPath)

    ## output
    # write the list
    writetxt.writeLists(lists, getOutputPath())

try:
    # init #
    mainwindow = Tk()                                                                       # main window 
    mainwindow.title("Word Picker - Version 0.1.2")                                         # main window
    mainwindow.geometry("600x450")                                                          # main window
    mainwindow.resizable(width=False, height=False)                                         # main window

    inputLabel = Label(mainwindow, text = "Chose Images:")                                  # Input
    inputButton = Button(mainwindow, text = "Browse...", command = browseImages)            # Input
    inputList = Listbox(mainwindow)                                                         # Input

    outputDirLabel = Label(mainwindow, text = "Output Directory:")                          # Output Dir
    outputDirEntry = Entry(mainwindow, textvariable = outputDir)                            # Output Dir
    for c in outputDir:                                                                     # Output Dir
        # set default dir - ~/Desktop                                                       # Output Dir
        outputDirEntry.insert(END, c)                                                       # Output Dir
    outputDirBrowse = Button(mainwindow, text = "Browse...", command = browseOutputDir)     # Output Dir

    wordChoseRatioLabel = Label(mainwindow, text = "Word Chose Ratio: ")                    # word chose ratio
    wordChoseRatioFractionBar = Label(mainwindow, text = '/')                               # word chose ratio
    numerator_wordChoseRatioSpinbox = Spinbox(mainwindow, from_ = 0, value = 40)            # word chose ratio
    denominator_wordChoseRatioSpinbox = Spinbox(mainwindow, from_ = 0, value = 60)          # word chose ratio

    exportButton = Button(mainwindow, text = "Export", command = main_export)
    
    # show #
    # Input
    inputLabel.place(x = 10, y = 15 + 50, width = 100)
    inputButton.place(x = 10 + 110, y = 10 + 50, width = 100)
    inputList.place(x = 10 + 110 + 100 + 30, y = 10 + 0, width = 325)

    # Output Dir
    outputDirLabel.place(x = 10, y = 215, width = 100)
    outputDirEntry.place(x = 10 + 110, y = 215 + 2, width = 350)
    outputDirBrowse.place(x = 10 + 110 + 350 + 10, y = 215 - 2, width = 100)

    # word chose ratio
    wordChoseRatioLabel.place(x = 10, y = 275, width = 125)
    numerator_wordChoseRatioSpinbox.place(x = 10 + 135, y = 275, width = 55)
    wordChoseRatioFractionBar.place(x = 10 + 135 + 65, y = 275, width = 40)
    denominator_wordChoseRatioSpinbox.place(x = 10 + 135 + 65 + 50, y = 275, width = 60)

    # export
    exportButton.place(x = 250, y = 350, width = 100)
    
    # start mainloop #
    mainwindow.mainloop()

except BaseException:
    pass
