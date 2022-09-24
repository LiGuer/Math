* Tree
  - Define  
    A class of Connected undirected graphs without loops.

    - root node: A node without forward nodes, and a tree has only one root node.
    - leaf node: A node that has no children.
    - depth: Number of edges in the simple path from node to root node. The depth of the tree is the maximum node depth in the tree.

  - Property
    - There exists and only exists one single simple path between any two points of the tree.
    - $number(E) = number(V) - 1$
    - Delete an edge, the Tree will become disconnected;  
      Add an edge, the Tree will have a loop.

  - Include
    * Binary tree
      - Define  
        A tree in which each node has at most two children, which are referred to as the left child and the right child.

      - Property
        * Traversal of Tree
          - Preorder traversal
          - Inorder traversal
          - Postorder traversal

        * Minimum Spanning Tree
          - Purpose
            Find a Tree on the Graph with the smallest sum of edge weights connected all nodes.

          - Algorithm
            * Prim Algorithm
              - Greed by vertices, put the shortest edge $(u,v)$ of $u$ from the searched vertices into the result edge sequence every time, and $v$ does not belong to the searched vertices.
              - Time complexity $O(E·logV)$

            * Kruskal Algorithm
              - Greed by edges
              - Time complexity $O(E·logV)$

      - Include
        * Complete Binary Tree
          - Define  
            A binary tree in which all leaf nodes have the same height.

          - Property
            - Number of nodes with depth $h$: $2^h$  
              Number of nodes in a complete binary tree with depth $h$: $2^{h+1} - 1$  
              Number of non leaf nodes: $2^h - 1$  
              Number of leaf nodes: $2^h$  

              - Proof
                $$\sum_{i=0}^h 2^i = \frac{1 - 2^h}{1 - 2} = 2^h - 1  \tag{geometric sequence summation}$$
