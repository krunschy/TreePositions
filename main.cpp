#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;


struct Point {
    double x, y;
};

class Tree {
public:
    string id;
    double x, y;
    string name;

    Tree() : id(""), x(0), y(0), name("") {}
    Tree(string id, double x, double y, string name) : id(id), x(x), y(y), name(name) {}
};

class Distance {
public:
    string tree1, tree2;
    double distance;

    Distance() : tree1(""), tree2(""), distance(0.0) {}
    Distance(string tree1, string tree2, double distance) : tree1(tree1), tree2(tree2), distance(distance) {}
};

vector<Point> findCircleIntersections(Point A, double rA, Point B, double rB) {
    vector<Point> intersections;

    double d = sqrt(pow(B.x - A.x, 2) + pow(B.y - A.y, 2));

    // Check if circles intersect
    if (d > rA + rB || d < abs(rA - rB)) {
        cout << "No intersection points." << endl;
        return {};
    }

    // Midpoint along the center-line
    double x2 = A.x + (B.x - A.x) * (rA * rA - rB * rB + d * d) / (2 * d * d);
    double y2 = A.y + (B.y - A.y) * (rA * rA - rB * rB + d * d) / (2 * d * d);

    double h = sqrt(rA * rA - pow((rA * rA - rB * rB + d * d) / (2 * d), 2));

    // Perpendicular vector
    double rx = -(B.y - A.y) * (h / d);
    double ry = (B.x - A.x) * (h / d);

    // Intersection points
    intersections.push_back({x2 + rx, y2 + ry});
    intersections.push_back({x2 - rx, y2 - ry});

    return intersections;
}

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

    Tree newTree = unknownTrees[index];  // Select the tree at the 'index' position in the sorted list

     cout << "\n\n NewTree selected: ID = " << newTree.id << ", X = " << newTree.x << ", Y = " << newTree.y << endl;
    // Find all distances that involve newTree
    vector<Distance> newTreeDistances;

    for (const auto& dist : distances) {
        if (newTree.id == dist.tree1 || newTree.id == dist.tree2) {
            newTreeDistances.push_back(dist);
        }
    }

    cout << "All Distances to newTree: ";
    for (const auto& distance : newTreeDistances) {
        cout << "Tree1 = " << distance.tree1 << ", Tree2 = " << distance.tree2 << ", Distance = " << distance.distance << endl;
    }
    cout << endl;
    vector<string> involvedTrees;




    //this block is pretty buggy

    // Filter out distances that don't involve any known tree. If a new known tree is discovered, it is added to involvedTrees
    newTreeDistances.erase(remove_if(newTreeDistances.begin(), newTreeDistances.end(), [&](const Distance& dist) {
        bool matchesKnownTree = false;
        for (const auto& knownTree : knownTrees) {
            if (dist.tree1 == knownTree.id || dist.tree2 == knownTree.id) {
                matchesKnownTree = true;
                // Add tree1 to involvedTrees if it's not already there
                if (dist.tree1 != newTree.id && find(involvedTrees.begin(), involvedTrees.end(), dist.tree1) == involvedTrees.end()) {
                    involvedTrees.push_back(dist.tree1);
                }

                // Add tree2 to involvedTrees if it's not already there and not newTree itself
                if (dist.tree2 != newTree.id && find(involvedTrees.begin(), involvedTrees.end(), dist.tree2) == involvedTrees.end()) {
                    involvedTrees.push_back(dist.tree2);
                }
                break;  // Exit loop early if a match is found
            }
        }
        return !matchesKnownTree;  // Remove the distance if it doesn't match any known tree
    }), newTreeDistances.end());
    //finds newTree itself too, which it shouldn't so heres a quick fix where we simply cut it
    involvedTrees.erase(remove(involvedTrees.begin(), involvedTrees.end(), newTree.id), involvedTrees.end());


    cout << "Involved trees after processing: ";
    for (const auto& treeId : involvedTrees) {
        cout << treeId << " ";
    }
    cout << endl;

    if (newTreeDistances.size() < 2) {
        cout << "adding failcount" << endl;
        processTreesAndDistances(knownTrees, unknownTrees, distances, failcount + 1);
    } else {

        //this entire thing can be repeated with involved Trees[1] und [2] etc if this doesnt yield a intersection

        // Find the actual Tree objects for involvedTrees[0] and involvedTrees[1]
        Tree TreeA, TreeB;
        for (const auto& tree : knownTrees) {
            if (tree.id == involvedTrees[0]) {
                TreeA = tree;
            }
            if (tree.id == involvedTrees[1]) {
                TreeB = tree;
            }
        }

        // Debugging: Print TreeA and TreeB information
        cout << "TreeA: ID = " << TreeA.id << ", X = " << TreeA.x << ", Y = " << TreeA.y << endl;
        cout << "TreeB: ID = " << TreeB.id << ", X = " << TreeB.x << ", Y = " << TreeB.y << endl;

        // Find distances associated with TreeA and TreeB
        Distance distA, distB;
        for (const auto& dist : newTreeDistances) {
            if (TreeA.id == dist.tree1 || TreeA.id == dist.tree2) {
                distA = dist;
                break; // Stop after finding the first match
            }
        }
        for (const auto& dist : newTreeDistances) {
            if (TreeB.id == dist.tree1 || TreeB.id == dist.tree2) {
                distB = dist;
                break; // Stop after finding the first match
            }
        }

        // Debugging: Print distances associated with TreeA and TreeB
        cout << "DistA: Tree1 = " << distA.tree1 << ", Tree2 = " << distA.tree2 << ", Distance = " << distA.distance << endl;
        cout << "DistB: Tree1 = " << distB.tree1 << ", Tree2 = " << distB.tree2 << ", Distance = " << distB.distance << endl;

        // Calculate intersections (Graphically or mathematically)
        Point TreePointA = {TreeA.x, TreeA.y};
        Point TreePointB = {TreeB.x, TreeB.y};
        vector<Point> intersections = findCircleIntersections(TreePointA, distA.distance, TreePointB, distB.distance);
        if (intersections.empty()){
            processTreesAndDistances(knownTrees, unknownTrees, distances, ++failcount);
        }
        // Debugging: Print the intersection points
        cout << "Intersection 1: X = " << intersections[0].x << ", Y = " << intersections[0].y << endl;
        cout << "Intersection 2: X = " << intersections[1].x << ", Y = " << intersections[1].y << endl;

        // Find which intersection is closer to newTree
        double error0 = pow(intersections[0].x - newTree.x, 2) + pow(intersections[0].y - newTree.y, 2);
        double error1 = pow(intersections[1].x - newTree.x, 2) + pow(intersections[1].y - newTree.y, 2);
        if (error0 > error1) {
            newTree.x = intersections[1].x;
            newTree.y = intersections[1].y;
        } else {
            newTree.x = intersections[0].x;
            newTree.y = intersections[0].y;
        }

        // Add newTree to knownTrees
        knownTrees.push_back(newTree);

        // Remove newTree from unknownTrees
        unknownTrees.erase(remove_if(unknownTrees.begin(), unknownTrees.end(),
            [&](const Tree& tree) { return tree.id == newTree.id; }),
            unknownTrees.end());

        // Reset failcount
        failcount = 0;

        // Continue the recursion with the updated lists
        processTreesAndDistances(knownTrees, unknownTrees, distances, failcount);
    }

    cout << "Error: Unable to find tree in the unknown list!" << endl;
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

    /*cout << "\n--- Trees ---\n";
    printTrees(trees);

    cout << "\n--- Distances ---\n";
    printDistances(distances);
    */
    //split the tree array
    vector<Tree> knownTrees(trees.begin(), trees.begin() + 16);
    vector<Tree> unknownTrees(trees.begin() + 14, trees.end());
    processTreesAndDistances(knownTrees, unknownTrees, distances, 0);
    printTrees(knownTrees);
    return 0;
}
