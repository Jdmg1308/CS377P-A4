#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include "CSR.h"

// Consts for pagerank algorithm
const double threshold = 1.0e-4;
const double damp = 0.85;

int pg_compute_initialize(CSR* g){
  int n_of_nodes= g->node_size();
  for (int n = 1; n <= n_of_nodes; n++) {
    g->set_label(n, THE_CURRENT, 1.0 / n_of_nodes);
  }
  return n_of_nodes;
}

void reset_next_label(CSR* g) { /////////////////////////////
  int num_nodes = g->node_size();
  for (int n = 1; n <= num_nodes; n++) {
    g->set_label(n, THE_NEXT, (1.0 - damp) / (double) num_nodes);
  }
}

void apply_curr_to_others(CSR* g, int n_of_nodes){ /////////////////////////////
  for (int n = 1; n <= n_of_nodes; n++) {
    double my_contribution = damp * g->get_label(n, THE_CURRENT) / (double)g->get_out_degree(n);
    for (int e = g->edge_begin(n); e < g->edge_end(n); e++) {
      int dst = g->get_edge_destination(e);
      g->set_label(dst, THE_NEXT, g->get_label(dst, THE_NEXT) + my_contribution);
    }
  }
}

bool converged(CSR* g) {
  // terminate the page-rank iterations when no node changes its
  // page-rank value by more than 10-4 (threshold) between successive iterations
  for (int n = 1; n <= g->node_size(); n++) {
    if (fabs(g->get_label(n, THE_NEXT) - g->get_label(n,THE_CURRENT)) > threshold) {
      return false;
    }
  }  
  return true;
}

void update_node_label(CSR* g) {
  for (int n = 1; n <= g->node_size(); n++) {
    g->set_label(n, THE_CURRENT, g->get_label(n, THE_NEXT));
  }
}

void scale_node_labels(CSR* g) {
  double sum = 0.0;
  for (int n = 1; n <= g->node_size(); n++) {
    sum += g->get_label(n, THE_CURRENT);
  }
  for (int n = 1; n <= g->node_size(); n++) {
    g->set_label(n, THE_CURRENT, g->get_label(n, THE_CURRENT) / sum);
  }
}

void pg_compute_push(CSR* g) {
  // initialize
  int n_of_nodes = pg_compute_initialize(g);
  bool convergence = false;
  while(!convergence) {
    // reset next labels
    reset_next_label(g);
    // apply current node contribution to others
    apply_curr_to_others(g, n_of_nodes);
    // check the change across successive iterations to determine convergence
    convergence = converged(g);
    // update current labels
    update_node_label(g);
  }
  scale_node_labels(g); // scale so the sum is 1
}

bool compare_pairs(pair<int, double>& p1, pair<int, double>& p2) {
  return (p1.second > p2.second) ? true : (p1.second == p2.second) ? (p1.first < p2.first) : false;
}

void pg_print(CSR* g, string out_file) {
  // create pairs to print
  vector<pair<int, double>> pairs;
  for (int n = 1; n <= g->node_size(); n++) {
    pairs.push_back(make_pair(n, g->get_label(n, THE_CURRENT)));
  }
  // sorts pairs
  sort(pairs.begin(), pairs.end(), compare_pairs);
  // print output
  ofstream out_stream(out_file);
  for (const pair<int, double>& pair: pairs) {
    out_stream << pair.first << " " << fixed << setprecision(6) << pair.second << endl;
  }
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    cerr << "ERROR: Use this format instead: " << argv[0] << " <input.dimacs> <output.txt>\n";
    return 0;
  }
  ifstream f_dimacs(argv[1]);
  if (!f_dimacs) {
    cerr << "ERROR: Could not open file " << argv[1] << endl;
    return 0;
  }
  // CREATE CSR GRAPH
  CSR* g = new CSR(f_dimacs);
  // compute the page rank algorithm
  pg_compute_push(g);
  // prints results to output file
  pg_print(g, argv[2]);
  // delete the allocated space to the graph avoid memory leak
  delete g;
  return 0;
}