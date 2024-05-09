from graphviz import Digraph

node_dic = {}

def read_nodes():
    with open('../Data/nodes.txt', 'r') as file:
        lines = file.readlines()
    for line in lines:
        
        node_id = line.split(' ')[0]
        node_name = line.split(' ')[1].strip()
        node_dic[node_id] = node_name
        #print(node_id, node_name)
        dot.node(node_id, label = node_name)


def tree_parser():
    with open('../Data/tree.txt', 'r') as file:
        lines = file.readlines()
    tree_dict = {}
    for line in lines:
        if '——' in line:
            parent, children = line.split('——')
            children_nodes = children.strip().split(' ')
            tree_dict[parent.strip()] = children_nodes

    return tree_dict

def draw_tree(tree_dict):
    edge = []
    for parent, children in tree_dict.items():
        for child in children:
            #print((parent, child))
            edge.append((parent, child))
    dot.edges(edge)
    dot.render('SyntaxTree.gv', view=True)


dot = Digraph(comment='The Syntax Tree')
read_nodes()
tree_dict = tree_parser()
draw_tree(tree_dict)