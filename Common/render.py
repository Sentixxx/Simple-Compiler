import matplotlib.pyplot as plt
import networkx as nx

node_dic = {}

def read_nodes():
    with open('../Data/nodes.out', 'r') as file:
        lines = file.readlines()
    for line in lines:
        node_id = line.split(':')[0]
        node_name = line.split(':')[1].strip()
        node_dic[node_id] = node_name
        print(node_id, node_name)


def tree_parser():
    with open('../Data/tree.out', 'r') as file:
        lines = file.readlines()
    tree_dict = {}
    for line in lines:
        if '——' in line:
            parent, children = line.split('——')
            children_nodes = children.strip().split(' ')
            tree_dict[parent.strip()] = children_nodes

    return tree_dict

def draw_tree(tree_dict):
    G = nx.DiGraph()  # 创建一个有向图
    for parent, children in tree_dict.items():
        for child in children:
            G.add_edge(parent, child)
    
    pos = nx.spring_layout(G)  # 布置图形
    nx.draw(G, pos, with_labels=True, node_size=5000, node_color="skyblue", node_shape="s", alpha=0.5, linewidths=40)
    nx.draw_networkx_labels(G, pos, labels=node_dic, font_size=12)
    plt.show()  # 显示图形


read_nodes()
tree_dict = tree_parser()
draw_tree(tree_dict)