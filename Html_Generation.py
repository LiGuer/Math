import svgwrite
import markdown
import numpy as np
import re
import os
from collections import deque

class Cell:
    def __init__(self, concept, file):
        self.concept = concept
        self.content = []
        self.file = file
        self.kid = []
        self.parent = []


def bfs (root, position):
    queue = deque([(root, 0)])
    element = 0
    level_cur = 0

    while queue:
        node, level = queue.popleft()

        if(level != level_cur):
            level_cur = level
            element = 0

        position[node] = [level, element]
        element += 1

        for kid in node.kid:
            if kid not in position:
                queue.append((kid, level + 1))
                position[kid] = [-1, -1] 

def draw_tree(root, filename='tree.svg'):
    position = {}
    bfs(root, position)

    elements_num = [0 for _ in range(100)]
    for key, value in position.items():
        elements_num[value[0]] = max(elements_num[value[0]], value[1] + 1)
    elements_num_max = max(elements_num)

    for key, value in position.items():
        value[1] = (elements_num_max * 40) / elements_num[value[0]] * (value[1] + 0.5)
        value[0] = (value[0] + 1) * 300
        
    dwg = svgwrite.Drawing(filename, profile='full')
    dwg.defs.add(dwg.style('a:hover { cursor: pointer; }'))

    visited = set([])
    draw_node(dwg, root, position, visited, 10)
    svg_string = dwg.tostring()

    # 生成 HTML 文件
    with open("html.html", "w", encoding="utf-8") as f:
        f.write("<!DOCTYPE html>\n")
        f.write("<html>\n")
        f.write("<head>\n")
        f.write("  <meta charset=\"UTF-8\">\n")
        f.write("  <title>SVG Example</title>\n")
        f.write("  <style>\n")
        f.write("    #svg-container {\n")
        f.write("      width: 100%;\n")
        f.write("      height: 100%;\n")
        f.write("      overflow: auto;\n")
        f.write("    }\n")
        f.write("  </style>\n")
        f.write("</head>\n")
        f.write("<body>\n")
        f.write("  <div id=\"svg-container\">\n")
        f.write(f"    <svg width=\"10000\" height=\"10000\">\n")
        f.write(f"      {svg_string}\n")
        f.write("    </svg>\n")
        f.write("  </div>\n")
        f.write("</body>\n")
        f.write("</html>\n")

def draw_node(dwg, node, position, visited, radius):
    # Draw the node
    dwg.add(dwg.circle(center=position[node], r=radius, stroke='red', fill='white'))
    link_element = dwg.add(dwg.a(href=("note/" + node.file if node.file != "" else "ReadMe.md")))
    link_element.add(dwg.text(node.concept if node.file != "" else "Math", insert=(position[node][0] - radius / 2 - 4, position[node][1] + radius / 4 + 4)))

    # Draw children
    for i, child in enumerate(node.kid):
        if (node, child) not in visited:
            visited.add((node, child))
            draw_edge(dwg, 
                    position[node][0] + radius, position[node][1], 
                    position[child][0] - radius, position[child][1], radius)
            draw_node(dwg, child, position, visited, radius)

def draw_edge(dwg, x1, y1, x2, y2, radius):
    # Draw a Bezier curve between nodes
    dwg.add(dwg.path(d=("M", x1, y1, "C", (x1 + x2) / 2, y1, (x1 + x2) / 2, y2, x2, y2), stroke="blue", fill="none"))
    #dwg.add(dwg.line((x1, y1), (x2, y2), stroke="black", fill="none"))


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


draw_tree(root)
