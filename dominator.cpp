#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Alias for readability
using Node = int;
using Graph = vector<vector<Node>>;

// Function to compute dominators
unordered_map<Node, set<Node>> computeDominators(const Graph& cfg, Node entry) {
    size_t n = cfg.size();
    unordered_map<Node, set<Node>> dom;

    // Initialize dominator sets
    for (Node i = 0; i < n; ++i) {
        if (i == entry) {
            dom[i] = {i}; // Entry node only dominates itself
        } else {
            dom[i] = set<Node>(cfg.begin(), cfg.end()); // Initially, all nodes dominate
        }
    }

    // Iterative computation
    bool changed = true;
    while (changed) {
        changed = false;
        for (Node node = 0; node < n; ++node) {
            if (node == entry) continue;

            // New dominator set: intersection of predecessors' dominator sets
            set<Node> newDom = {node};
            for (Node pred : cfg[node]) {
                set<Node> intersect;
                set_intersection(dom[pred].begin(), dom[pred].end(),
                                 newDom.begin(), newDom.end(),
                                 inserter(intersect, intersect.begin()));
                newDom = intersect;
            }

            // Check for changes
            if (newDom != dom[node]) {
                dom[node] = newDom;
                changed = true;
            }
        }
    }

    return dom;
}

// Function to compute immediate dominators (IDOM)
unordered_map<Node, Node> computeImmediateDominators(const Graph& cfg, Node entry) {
    auto dom = computeDominators(cfg, entry);
    unordered_map<Node, Node> idom;

    for (Node node = 0; node < cfg.size(); ++node) {
        if (node == entry) continue;

        // Immediate dominator: the unique dominator closest to the node
        set<Node> dominators = dom[node];
        dominators.erase(node);

        Node immediateDominator = -1;
        for (Node d : dominators) {
            bool isClosest = true;
            for (Node other : dominators) {
                if (other != d && dom[other].count(d)) {
                    isClosest = false;
                    break;
                }
            }
            if (isClosest) {
                immediateDominator = d;
                break;
            }
        }
        idom[node] = immediateDominator;
    }

    return idom;
}

// Function to compute dominance frontier (DF)
unordered_map<Node, set<Node>> computeDominanceFrontier(const Graph& cfg, const unordered_map<Node, Node>& idom) {
    unordered_map<Node, set<Node>> df;
    for (Node node = 0; node < cfg.size(); ++node) {
        if (cfg[node].size() < 2) continue; // Skip nodes without multiple predecessors

        for (Node succ : cfg[node]) {
            Node runner = succ;
            while (runner != idom.at(node)) {
                df[runner].insert(node);
                runner = idom.at(runner);
            }
        }
    }
    return df;
}

// Example usage
int main() {
    // Example CFG (adjacency list representation)
    Graph cfg = {
        {1},          // Node 0 -> 1
        {2, 3},       // Node 1 -> 2, 3
        {4},          // Node 2 -> 4
        {4},          // Node 3 -> 4
        {}            // Node 4 (exit node)
    };

    Node entry = 0;

    // Compute immediate dominators
    auto idom = computeImmediateDominators(cfg, entry);

    // Output immediate dominators
    cout << "Immediate Dominators (IDOM):" << endl;
    for (const auto& [node, dom] : idom) {
        cout << "Node " << node << " is immediately dominated by Node " << dom << endl;
    }

    // Compute dominance frontier
    auto df = computeDominanceFrontier(cfg, idom);

    // Output dominance frontier
    cout << "\nDominance Frontier (DF):" << endl;
    for (const auto& [node, frontier] : df) {
        cout << "Node " << node << ": { ";
        for (Node f : frontier) {
            cout << f << " ";
        }
        cout << "}" << endl;
    }

    return 0;
}
