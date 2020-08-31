from graphviz import Digraph

def dfs(x, fa, lable, G, graph):
    graph.node(str(x), lable[x])
    if fa != -1:
        graph.edge(str(fa), str(x))
    for y in G[x]:
        if y == "":
            break
        dfs(int(y), x, lable, G, graph)


def data_acquire(path):
    # read file
    fo = open(path, "r+")
    data = fo.read().split("\n")

    # acquire data
    n = int(data[0])
    lable = data[1].split(" ")
    for i in range(0, len(lable)):
        if lable[i] == "<>":
            lable[i] = "\<\>"
        
    G = []
    for row in range(2, len(data)):
        G.append(data[row].split(" "))
    return n, lable, G


if __name__ == "__main__":
    # path list
    path = "data/output/PL0_graph.txt"
    out_path1 = "data/output/PL0_graph_structure.txt"
    out_path2 = "data/output/PL0_graph"

    # init data acquire
    n, lable, G = data_acquire(path)
    # graph = gz.Graph()
    graph = Digraph('G', filename=out_path2)
    graph.attr(rankdir="LR", style="filled", bgcolor="white")
    graph.node_attr.update(style='filled', color='white')
    graph.edge_attr.update(style='filled', color='grey')

    # dfs construct graph
    dfs(int(0), int(-1), lable, G, graph)

    # output graph structure
    fo = open(out_path1, "w")
    fo.write(graph.source)
    graph.render(out_path2, view=True)
