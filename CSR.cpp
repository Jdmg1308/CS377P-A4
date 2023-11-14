#include "CSR.h"

  CSR::CSR() {
    // no dynamic allocated space used
    is_allocated = false;
  }

  CSR::CSR(ifstream& f_dimacs) {
    // parse file
    vector<Edge> edges = parse_dimacs(f_dimacs);
    // convert to CSR
    gen_CSR(edges);
    // delete the space
    is_allocated = true;
  }

  CSR::~CSR() {
    if (!is_allocated) {
      return;
    }
    // clean up dynamic arrays
    delete [] node;
    delete [] rp;
    delete [] ci;
    delete [] ai;
  }

  vector<string> split_line(const std::string& str, char delimiter) {
    vector<string> tokens;
    istringstream stream(str);
    string token;
    while (std::getline(stream, token, delimiter)) {
      tokens.push_back(token);
    }
    return tokens;
  }

  vector<CSR::Edge> CSR::parse_dimacs(ifstream& f_dimacs) {
    vector<Edge> edges;
    string line;
    // parse dimacs
    while (getline(f_dimacs, line)) {
      // skip comments
      if (line[0] != 'c') {  
        // split line into tokens
        vector<string> token = split_line(line, ' ');
        // p <type> <num_nodes> <num_edges>
        if (token[0] == "p") {
          num_nodes = stoi(token[2]);
          num_edges = stoi(token[3]);
        }
        // a <src> <dst> <weight>
        if (token[0] == "a") {
          edges.push_back(Edge(stoi(token[1]), stoi(token[2]), stoi(token[3])));
        }
      }
    }

    return edges;
  }

  bool edge_sort(CSR::Edge &e1,CSR::Edge &e2) {
    return (e1.src < e2.src) ? true : (e1.src == e2.src) ? (e1.dst < e2.dst) : false;
  }

  void CSR::gen_CSR(vector<Edge>& edges) {
    // sort the edges
    sort(edges.begin(), edges.end(), edge_sort);
    // handle duplicate in the edges
    vector<Edge> good_edges;
    int edg_size = edges.size();
    int src = edges[0].src;
    int dst = edges[0].dst;
    int max_weight = edges[0].weight;
    for (int i = 1; i < edg_size; i++) {
      if (edges[i].src == edges[i - 1].src && edges[i].dst == edges[i - 1].dst) {
        max_weight = edges[i].weight > max_weight ? edges[i].weight : max_weight;
      } 
      else {
        good_edges.push_back (Edge(edges[i - 1].src, edges[i - 1].dst, max_weight));
        max_weight = edges[i].weight;
      }
    }
    // push last edge
    good_edges.push_back(Edge(edges[edg_size - 1].src, edges[edg_size - 1].dst, max_weight));
    // get number of edges
    num_edges = good_edges.size();
    // allocation
    node = new Node [num_nodes + 2];
    ci = new int [num_edges + 2];
    ai = new int [num_edges + 2];
    rp = new int [num_nodes + 2];
    ci[0] = 0;
    ai[0] = 0;
    rp[0] = 1;
    good_edges.insert(good_edges.begin(), Edge(rp[0], ci[0], ai[0]));
    // loop through each edge and construct CSR representation
    int cur_node = 0;
    int cur_edge = 1;
    while (cur_edge <= num_edges) {
      const Edge& e = good_edges[cur_edge];
      // if current source node is larger than current node update cur_node and its coresponding rp when 
      if (e.src > cur_node) {
        cur_node++;
        rp[cur_node] = cur_edge;
      }
      // copy the destination and weight to ci and ai otherwise
      else {
        ci[cur_edge] = e.dst;
        ai[cur_edge] = e.weight;
        cur_edge++;
      }
    }
    // set the remaining nodes without edges
    for (int i = cur_node + 1; i <= num_nodes + 1; i++) {
      rp[i] = num_edges + 1;
    }
    ai[num_edges + 1] = 0;
    ci[num_edges + 1] = num_nodes + 1;
  }

  
  int CSR::edge_begin(int n) {
    return rp[n];
  }

  int CSR::edge_end(int n) {
    return rp[n + 1]; 
  }

  int CSR::node_size() {
    return num_nodes; 
  }

  int CSR::size_edges() {
    return num_edges; 
  }

  double CSR::get_label(int n, int i) {
    return node[n].node_labels[i]; 
  }

  void CSR::set_label(int n, int i, double label) {
    node[n].node_labels[i] = label;
  }

  int CSR::get_out_degree(int n) {
    return rp[n + 1] - rp[n]; 
  }

  int CSR::get_edge_destination(int e) {
    return ci[e];
  }


