import os
def writeLists(wordlists, dir):
    for i in range(len(wordlists)):
        writeList(os.path.join(dir, str(i + 1) + ".txt"), wordlists[i])
    return 0

def writeList(filename, wordlist):
    file = open(filename, "w")
    for word in wordlist:
        file.write(word + '\n')
    return 0
