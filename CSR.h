#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <string>
#include <sstream>
// #include <boost/algorithm/string.hpp> --

using namespace std;

#define THE_CURRENT 0
#define THE_NEXT 1

class CSR {
// You shouldn't need to modify this class except for adding essential locks and relevant methods,
// but if you find it useful for your code you can add some minor helper functions.
// Make sure not to store too much additional information here.
// The whole point of using CSR is that it utilizes the space efficiently.

private:

public:
  // use an array of struct for labels
  struct Node {
    // labels[0] is the current label, labels[1] is the next label
    double node_labels[2];
  };

  // helper struct for COO
  struct Edge {
    int src;
    int dst; 
    int weight;
    Edge(int s, int d, int w): src(s), dst(d), weight(w) { }
  };

  // number of nodes and edges in the graph
  int num_nodes;
  int num_edges;

  // csr representation of the graph (same as in the slides)
  int* rp;
  int* ci;
  int* ai;

  // pagerank labels
  Node* node;

  bool is_allocated;

  CSR();
  CSR(ifstream& f_dimacs);
  ~CSR();
  vector<Edge> parse_dimacs(ifstream& f_dimacs);
  void gen_CSR(vector<Edge>& edges);
  int edge_begin(int n);
  int edge_end(int n);
  int node_size();
  int size_edges();
  double get_label(int n, int which);
  void set_label(int n, int which, double label);
  int get_out_degree(int n);
  int get_edge_destination(int e);
};

