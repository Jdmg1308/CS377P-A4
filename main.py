import networkx as nx
import matplotlib.pyplot as plt
my_list = []
my_nums = []
# Function to read a DIMACS file and create a NetworkX graph
def readDIMACSFile(filename):
    global my_list
    global my_nums
    G = nx.Graph()
    with open(filename, 'r') as file:
        for line in file:
            line = line.strip()
            if line.startswith('c'):
                # Skip comments
                continue
            elif line.startswith('p'):
                _, _, numVertices, numEdges = line.split()
                my_list = [0 for i in range(int(numVertices))] 
                my_nums = [i for i in range(int(numVertices))] 
                # You can process 'numVertices' and 'numEdges' as needed.
            elif line.startswith('a'):
                _, nodeA, nodeB, _ = line.split()
                G.add_edge(int(nodeA), int(nodeB))
                my_list[int(nodeA) - 1] =  my_list[int(nodeA) - 1] + 1
                # For an undirected graph, you may also need to add the reverse edge:
                # G.add_edge(int(nodeB), int(nodeA))
    return G

# Function to read weights from a separate file
def readWeightsFile(filename):
    node_weights = {}
    with open(filename, 'r') as file:
        for line in file:
            node, weight = line.split()
            node_weights[int(node)] = float(weight)
    return node_weights

# Function to visualize the graph with node sizes based on weights
def visualizeGraph(graph, node_weights):
    pos = nx.planar_layout(graph)  # You can use different layouts here

    # Scale the node sizes based on weights
    scaled_sizes = [50000 * node_weights[node] for node in graph.nodes]
    node_colors = [node_weights[node]  for node in graph.nodes]
    # node_cmap = plt.cm.viridis
    node_labels = {}
    with open("output.txt", "r") as file:
        for line in file:
            node_id, label = line.strip().split()
            node_labels[int(node_id)] = "Node: " + node_id + "\nRatio: " + label

    plt.figure(figsize=(10, 6))
    nx.draw(graph, pos, node_size=scaled_sizes, with_labels=False, font_size=10, font_color='black', node_color=node_colors, edge_color='black')
    nx.draw_networkx_labels(G, pos, node_labels, font_size=10, font_color='black')

    plt.title(f'Graph Visualization with Node Sizes Based on Weights')
    plt.show()

# Function to create a histogram of out-degrees
def print_edges_perNode():
    global my_list
    global my_nums
    with open('beans.txt', 'w') as file:
    # Iterate through the indices and values and write them next to each other
        for i, value in enumerate(my_nums):
            line = f"{value + 1}\t{my_list[i]}\n"  # Using a tab character as a separator
            file.write(line)

    # Show the histogram
    plt.show()

def createOutDegreeHistogram():
    global my_list
    global my_nums
    plt.bar(my_nums, my_list, label="data")
    plt.xlabel('Vertex')
    plt.ylabel('Number of Nodes')
    plt.title('Outgoing edges')
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    plt.show()

if __name__ == "__main__":
    filename = "road-NY.dimacs"  # Change this to your DIMACS file
    weights_filename = "output.txt"  # Change this to the separate weights file
    G = readDIMACSFile(filename)
    node_weights = readWeightsFile(weights_filename)

    # Visualize the graph with node sizes based on weights
    # visualizeGraph(G, node_weights)

    # Create a histogram of out-degrees
    print_edges_perNode()

    # only for wiki
    createOutDegreeHistogram()

    print("Graph visualized with node sizes based on weights, and an out-degree histogram using NetworkX and Matplotlib.")
