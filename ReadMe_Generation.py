import numpy as np
import re
import os

class Cell:
    def __init__(self, concept, file):
        self.concept = concept
        self.content = []
        self.file = file
        self.kid = []
        self.parent = []


out = open('ReadMe.md', 'w', encoding = 'utf-8')
root = Cell('', '')
CellLib = {}

for file_name in os.listdir('note'):
    file = open('note/' + file_name, 'r', encoding = 'utf-8')
    Str = file.read()
    file.close()
    Str = Str.split('\n')

    stack = [root]
    stack_tab = [-2]
    
    for s in Str:
        tab = 0
        while(tab < len(s) and s[tab] == ' '):
            tab += 1

        if(tab == len(s)):
            continue

        while(tab <= stack_tab[-1]):
            stack.pop()
            stack_tab.pop()

        if(s[tab] == '*'):
            tail = len(s) - 1
            while(tail >= tab and s[tail] == ' '):
                tail -= 1

            title = s[tab+2:tail+1]

            if (title in CellLib):
                newNode = CellLib[title]
                if(tab == 0):
                    newNode.file = file_name

            else:
                newNode = Cell(title, file_name)
                CellLib[title] = newNode

            stack[-1].kid.append(newNode)
            newNode.parent.append(stack[-1])

            stack.append(newNode)
            stack_tab.append(tab)

        else:
            stack[-1].content.append(s)

def walk(a, t):
    if(t == 0):
        for c in a.kid:
            if(len(c.parent) == 1):
                text = '  '*t + '* [' + c.concept + '](note/' + c.file + ')'
                out.write(text + '\n')
                walk(c, t+1)

    else:
        for c in a.kid:
            text = '  '*t + '* [' + c.concept + '](note/' + c.file + ')'
            out.write(text + '\n')
            walk(c, t+1)

walk(root, 0)

out.close()
