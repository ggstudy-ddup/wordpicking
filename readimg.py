import pytesseract, re, os
from PIL import Image

def readImages2List(path, filenames):
    wordlistInOne = []
    for filename in filenames:
        
        wordlist = readList(os.path.join(path, filename).replace("/", "\\"))
        for string in wordlist:
            wordlistInOne.append(string)
    return wordlistInOne

def readList(filename):
    img = Image.open(filename)
    text = pytesseract.image_to_string(img)

    words = []
    strcache = ""
    for c in text:
        if '\n' != c:
            strcache += c
        elif "" != strcache and None == re.match("word list [123456789][1234567890]", strcache, re.I):
            words.append(strcache)
            strcache = ""
        else:
            strcache = ""
    return words
