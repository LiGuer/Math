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
    #Str = re.sub(r"\t", "  ", Str)
    Str = Str.split('\n')

    stack = [root]
    stack_tab = [-2]
    
    for s in Str:
        # Tab length
        tab = 0
        while(tab < len(s) and s[tab] == ' '):
            tab += 1

        if(tab == len(s)):
            continue

        while(tab <= stack_tab[-1]):
            stack.pop()
            stack_tab.pop()

        if(s[tab:tab+2] == "* "):
            tail = len(s) - 1
            while(tail >= tab and s[tail] == ' '):
                tail -= 1
            title = s[tab+2:tail+1]

            if (title in CellLib):
                newNode = CellLib[title]

                if (tab == 0):
                    newNode.file = file_name
            else:
                newNode = Cell(title, file_name)
                CellLib[title] = newNode

            # add kid into tree
            stack[-1].kid.append(newNode)
            newNode.parent.append(stack[-1])
            # stack push
            stack.append(newNode)
            stack_tab.append(tab)
        else:
            stack[-1].content.append(s)

for e in CellLib.values():
    if (e in root.kid and len(e.parent) != 1):
        root.kid.remove(e)
        e.parent.remove(root)

def walk(a, t):
    for c in a.kid:
        out.write('  ' * t + '* [' + c.concept + '](note/' + c.file + ')\n')
        walk(c, t+1)

walk(root, 0)

out.close()
