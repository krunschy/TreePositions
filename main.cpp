#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>

using namespace std;

class Tree {
public:
    string id;
    double x, y;
    string name;

    Tree(string id, double x, double y, string name) : id(id), x(x), y(y), name(name) {}
};

class Distance {
public:
    string tree1, tree2;
    double distance;

    Distance(string tree1, string tree2, double distance) : tree1(tree1), tree2(tree2), distance(distance) {}
};

vector<Tree> parseTreeList(const vector<string>& lines) {
    vector<Tree> trees;
    for (const string& line : lines) {
        istringstream iss(line);
        string id, name;
        double x, y;

        // Read the id, and check if it's a single digit, if so, add a leading zero
        iss >> id >> x;
        if (id.length() == 1) {
            id = "0" + id;  // Add leading zero to single-digit id
        }

        iss.ignore(1, ','); // Ignore comma
        iss >> y;
        iss.ignore(1, '\t'); // Ignore tab
        getline(iss, name);

        trees.emplace_back(id, x, y, name);
    }
    return trees;
}

vector<Distance> parseDistanceList(const vector<string>& lines) {
    vector<Distance> distances;
    for (const string& line : lines) {
        string trimmed_line = line;
        // Trim leading and trailing spaces
        size_t start = trimmed_line.find_first_not_of(" \t");
        trimmed_line = trimmed_line.substr(start);

        // Find the position of the '#' which separates tree1 and tree2
        size_t hashPos = trimmed_line.find('#');
        size_t spacePos = trimmed_line.find_first_of(" \t", hashPos);  // Find the first space after #

        if (hashPos != string::npos && spacePos != string::npos) {
            string tree1 = trimmed_line.substr(0, hashPos);  // Extract tree1
            string tree2 = trimmed_line.substr(hashPos + 1, spacePos - hashPos - 1);  // Extract tree2
            double dist = stod(trimmed_line.substr(spacePos));  // Extract the distance value

            distances.emplace_back(tree1, tree2, dist);
        } else {
            cerr << "Error parsing line: " << line << endl;
        }
    }
    return distances;
}

void printTrees(const vector<Tree>& trees) {
    for (const auto& tree : trees) {
        cout << "ID: " << tree.id << " | X: " << tree.x << " | Y: " << tree.y << " | Name: " << tree.name << endl;
    }
}

void printDistances(const vector<Distance>& distances) {
    for (const auto& dist : distances) {
        cout << "Tree1: " << dist.tree1 << " | Tree2: " << dist.tree2
             << " | Distance: " << fixed << setprecision(2) << dist.distance << endl;
    }
}


void processTreesAndDistances(vector<Tree>& knownTrees, vector<Tree>& unknownTrees, const vector<Distance>& distances, int failcount) {
    // If the unknown trees list is empty, return success message
    if (unknownTrees.empty()) {
        cout << "Success: All trees have been processed!" << endl;
        return;
    }

       // Sort the unknown trees by their IDs (lexicographical sorting)
    sort(unknownTrees.begin(), unknownTrees.end(), [](const Tree& a, const Tree& b) {
        return a.id < b.id;  // Sort by ID in ascending order
    });

    // Find the tree with the lowest number, considering failcount
    string selectedTreeId = "";
    int index = failcount;  // Start from the failcount index, meaning skip 'failcount' number of trees

    if (index >= unknownTrees.size()) {
        cout << "Error: Failcount exceeds the number of unknown trees!" << endl;
        return;
    }

    newTree = unknownTrees[index];  // Select the tree at the 'index' position in the sorted list

    //lets find all distance that involve newKnownTree
    vector<Distance> newTreeDistances;
    for (const auto& dist : distances) {
    if (newTree.id == dist.tree1 || newTree.id == dist.tree2) {
        newTreeDistances.push_back(dist);
    }
}
vector<string> involvedTrees;

    // Check if either tree1 or tree2 matches any known tree
    for (const auto& knownTree : knownTrees) {
        if (dist.tree1 == knownTree.id || dist.tree2 == knownTree.id) {
            matchesKnownTree = true;
            // If it's a match, push the tree IDs into the vector, but check whether we don't have it already
            if (find(involvedTrees.begin(), involvedTrees.end(), dist.tree1) == involvedTrees.end()) {
                involvedTrees.push_back(dist.tree1);
            }
            if (find(involvedTrees.begin(), involvedTrees.end(), dist.tree2) == involvedTrees.end()) {
                involvedTrees.push_back(dist.tree2);
            }
            break;
        }
    }

    if(newTreeDistances.size<2){
        processTreesAndDistances(knownTrees, unknownTrees, distances, failcount + 1);
    }
    else{
            //find the trees in known trees
            // Process the relevant distances and use the math to find the tree locations
            // For now, just add the tree and continue
            // TODO: Add the math for calculating the coordinates using distances

            // Reset failcount
            failcount = 0;

            // Continue the recursion with the updated lists
            processTreesAndDistances(knownTrees, unknownTrees, distances, failcount);
        }
    } else {
        cout << "Error: Unable to find tree " << lowestTree << " in the unknown list!" << endl;
    }
}

vector<string> readInput() {
    vector<string> lines;
    string line;
    while (true) {
        getline(cin, line);
        if (line.empty()) break;
        lines.push_back(line);
    }
    return lines;
}

int main() {
    cout << "Enter tree data (Press Enter on an empty line to finish):\n";
    vector<string> treeData = readInput();

    cout << "\nEnter distance data (Press Enter on an empty line to finish):\n";
    vector<string> distanceData = readInput();

    vector<Tree> trees = parseTreeList(treeData);
    vector<Distance> distances = parseDistanceList(distanceData);

    cout << "\n--- Trees ---\n";
    printTrees(trees);

    cout << "\n--- Distances ---\n";
    printDistances(distances);
    /*
    //split the tree array
    vector<Tree> knownTrees(trees.begin(), trees.begin() + 14);
    vector<Tree> unknownTrees(trees.begin() + 14, trees.end());
    processTreesAndDistances(knownTrees, unknownTrees, distances, 0);
    printTrees(knownTrees);
    */
    return 0;
}
