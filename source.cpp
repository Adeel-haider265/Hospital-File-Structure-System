#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>

using namespace std;

// Node structure for the n-ary tree
struct Node {
    string name;
    bool isDirectory;
    Node* parent;
    Node* firstChild;
    Node* nextSibling;
};

// Function to create a new node
Node* createNode(string name, bool isDirectory, Node* parent) {
    Node* newNode = new Node;
    newNode->name = name;
    newNode->isDirectory = isDirectory;
    newNode->parent = parent;
    newNode->firstChild = nullptr;
    newNode->nextSibling = nullptr;
    return newNode;
}

// Function to create a new directory
void createDirectory(Node* parent, string name) {
    Node* newNode = createNode(name, true, parent);
    if (parent->firstChild == nullptr) {
        parent->firstChild = newNode;
    } else {
        Node* current = parent->firstChild;
        while (current->nextSibling != nullptr) {
            current = current->nextSibling;
        }
        current->nextSibling = newNode;
    }
    cout << "Directory created: " << name << endl;
}

// Function to create a new file
void createFile(Node* parent, string name) {
    Node* newNode = createNode(name, false, parent);
    if (parent->firstChild == nullptr) {
        parent->firstChild = newNode;
    } else {
        Node* current = parent->firstChild;
        while (current->nextSibling != nullptr) {
            current = current->nextSibling;
        }
        current->nextSibling = newNode;
    }
    cout << "File created: " << name << endl;
}

// Function to copy a node
Node* copyNode(Node* node, Node* newParent) {
    Node* newNode = createNode(node->name, node->isDirectory, newParent);
    if (node->isDirectory) {
        Node* current = node->firstChild;
        while (current != nullptr) {
            copyNode(current, newNode);
            current = current->nextSibling;
        }
    }
    return newNode;
}

// Function to copy a file or directory
void copy(Node* node, Node* newParent) {
    Node* newNode = copyNode(node, newParent);
    if (newParent->firstChild == nullptr) {
        newParent->firstChild = newNode;
    } else {
        Node* current = newParent->firstChild;
        while (current->nextSibling != nullptr) {
            current = current->nextSibling;
        }
        current->nextSibling = newNode;
    }
    cout << "Copied: " << node->name << endl;
}

// Function to cut a node
Node* cutNode(Node* node, Node* newParent) {
    Node* newNode = copyNode(node, newParent);
    if (newParent->firstChild == nullptr) {
        newParent->firstChild = newNode;
    } else {
        Node* current = newParent->firstChild;
        while (current->nextSibling != nullptr) {
            current = current->nextSibling;
        }
        current->nextSibling = newNode;
    }
    cout << "Cut: " << node->name << endl;
    return newNode;
}

// Function to cut a file or directory
void cut(Node* node, Node* newParent) {
    Node* newNode = cutNode(node, newParent);
    if (newParent->firstChild == nullptr) {
        newParent->firstChild = newNode;
    } else {
        Node* current = newParent->firstChild;
        while (current->nextSibling != nullptr) {
            current = current->nextSibling;
        }
        current->nextSibling = newNode;
    }
    cout << "Cut: " << node->name << endl;
}

// Function to paste a node
void paste(Node* node, Node* newParent) {
    if (newParent->firstChild == nullptr) {
        newParent->firstChild = node;
    } else {
        Node* current = newParent->firstChild;
        while (current->nextSibling != nullptr) {
            current = current->nextSibling;
        }
        current->nextSibling = node;
    }
    cout << "Pasted: " << node->name << endl;
}

// Function to merge two nodes
void merge(Node* node1, Node* node2) {
    if (node1->isDirectory && node2->isDirectory) {
        Node* current = node2->firstChild;
        while (current != nullptr) {
            paste(current, node1);
            current = current->nextSibling;
        }
        cout << "Merged: " << node2->name << " into " << node1->name << endl;
    } else {
        cout << "Merge failed: Both nodes should be directories" << endl;
    }
}

// Function to delete a node
void deleteNode(Node* node) {
    if (node->parent != nullptr) {
        Node* parent = node->parent;
        if (parent->firstChild == node) {
            parent->firstChild = node->nextSibling;
        } else {
            Node* current = parent->firstChild;
            while (current->nextSibling != node) {
                current = current->nextSibling;
            }
            current->nextSibling = node->nextSibling;
        }
        delete node;
        cout << "Deleted" << endl;
    } else {
        cout << "Cannot delete root node" << endl;
    }
}

// Function to search for a node by name
Node* searchByName(Node* node, string name) {
    if (node->name == name) {
        return node;
    }
    if (node->isDirectory) {
        Node* current = node->firstChild;
        while (current != nullptr) {
            Node* result = searchByName(current, name);
            if (result != nullptr) {
                return result;
            }
            current = current->nextSibling;
        }
    }
    return nullptr;
}

// Function to search for a node by path
Node* searchByPath(Node* node, string path) {
    stack<string> directories;
    string directory;
    for (char c : path) {
        if (c == '/') {
            directories.push(directory);
            directory = "";
        } else {
            directory += c;
        }
    }
    directories.push(directory);

    Node* current = node;
    while (!directories.empty()) {
        string dir = directories.top();
        directories.pop();
        if (dir == "") {
            continue;
        }
        if (current->isDirectory) {
            Node* result = searchByName(current, dir);
            if (result != nullptr) {
                current = result;
            } else {
                cout << "Path not found: " << path << endl;
                return nullptr;
            }
        } else {
            cout << "Invalid path: " << path << endl;
            return nullptr;
        }
    }
    return current;
}

// Function to import file structure from a text file
void importFileStructure(Node* root, string filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            string name;
            bool isDirectory;
            if (line[0] == 'D') {
                name = line.substr(2);
                isDirectory = true;
            } else if (line[0] == 'F') {
                name = line.substr(2);
                isDirectory = false;
            } else {
                cout << "Invalid file structure" << endl;
                return;
            }
            createNode(name, isDirectory, root);
        }
        file.close();
        cout << "File structure imported" << endl;
    } else {
        cout << "Unable to open file: " << filename << endl;
    }
}

// Function to export file structure to a text file
void exportFileStructure(Node* node, string filename) {
    ofstream file(filename);
    if (file.is_open()) {
        stack<pair<Node*, int>> nodes;
        nodes.push(make_pair(node, 0));
        while (!nodes.empty()) {
            Node* current = nodes.top().first;
            int level = nodes.top().second;
            nodes.pop();
            for (int i = 0; i < level; i++) {
                file << "  ";
            }
            if (current->isDirectory) {
                file << "D " << current->name << endl;
                Node* child = current->firstChild;
                while (child != nullptr) {
                    nodes.push(make_pair(child, level + 1));
                    child = child->nextSibling;
                }
            } else {
                file << "F " << current->name << endl;
            }
        }
        file.close();
        cout << "File structure exported" << endl;
    } else {
        cout << "Unable to create file: " << filename << endl;
    }
}

// Function to display file structure in level order
void displayFileStructure(Node* node) {
    if (node == nullptr) {
        cout << "File structure is empty." << endl;
        return;
    }

    queue<Node*> q;
    q.push(node);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        cout << current->name << endl;

        if (current->isDirectory) {
            Node* child = current->firstChild;
            while (child != nullptr) {
                q.push(child);
                child = child->nextSibling;
            }
        }
    }
}

// Function to open a directory by name or path
void openDirectory(Node* root, string directoryName) {
    Node* directory = searchByPath(root, directoryName);
    if (directory != nullptr && directory->isDirectory) {
        cout << "Directory Name: " << directory->name << endl;
        cout << "Directory Path: " << directoryName << endl;
    } else {
        cout << "Directory not found: " << directoryName << endl;
    }
}

int main() {
    Node* root = createNode("root", true, nullptr);

    while (true) {
        cout << "File Management System Menu" << endl;
        cout << "1. Create Directory" << endl;
        cout << "2. Create File" << endl;
        cout << "3. Copy" << endl;
        cout << "4. Cut" << endl;
        cout << "5. Paste" << endl;
        cout << "6. Merge" << endl;
        cout << "7. Delete" << endl;
        cout << "8. Search by Name" << endl;
        cout << "9. Search by Path" << endl;
        cout << "10. Export File Structure" << endl;
        cout << "11. Import File Structure" << endl;
        cout << "12. Display File Structure" << endl;
        cout << "13. Open Directory" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                string name;
                cout << "Enter directory name: ";
                cin >> name;
                createDirectory(root, name);
                break;
            }
            case 2: {
                string name;
                cout << "Enter file name: ";
                cin >> name;
                createFile(root, name);
                break;
            }
            case 3: {
                string sourceName, destinationName;
                cout << "Enter source directory/file name: ";
                cin >> sourceName;
                cout << "Enter destination directory name: ";
                cin >> destinationName;
                Node* source = searchByName(root, sourceName);
                Node* destination = searchByName(root, destinationName);
                if (source != nullptr && destination != nullptr) {
                    copy(source, destination);
                }
                break;
            }
            case 4: {
                string sourceName, destinationName;
                cout << "Enter source file name: ";
                cin >> sourceName;
                cout << "Enter destination directory name: ";
                cin >> destinationName;
                Node* source = searchByName(root, sourceName);
                Node* destination = searchByName(root, destinationName);
                if (source != nullptr && destination != nullptr) {
                    cut(source, destination);
                }
                break;
            }
            case 5: {
                string sourceName, destinationName;
                cout << "Enter source directory/file name: ";
                cin >> sourceName;
                cout << "Enter destination directory name: ";
                cin >> destinationName;
                Node* source = searchByName(root, sourceName);
                Node* destination = searchByName(root, destinationName);
                if (source != nullptr && destination != nullptr) {
                    paste(source, destination);
                }
                break;
            }
            case 6: {
                string node1Name, node2Name;
                cout << "Enter first directory/file name: ";
                cin >> node1Name;
                cout << "Enter second directory/file name: ";
                cin >> node2Name;
                Node* node1 = searchByName(root, node1Name);
                Node* node2 = searchByName(root, node2Name);
                if (node1 != nullptr && node2 != nullptr) {
                    merge(node1, node2);
                }
                break;
            }
            case 7: {
                string nodeName;
                cout << "Enter directory/file name: ";
                cin >> nodeName;
                Node* node = searchByName(root, nodeName);
                if (node != nullptr) {
                    deleteNode(node);
                }
                break;
            }
            case 8: {
                string nodeName;
                cout << "Enter node name: ";
                cin >> nodeName;
                Node* result = searchByName(root, nodeName);
                if (result != nullptr) {
                    cout << "Found: " << result->name << endl;
                }
                break;
            }
            case 9: {
                string path;
                cout << "Enter path: ";
                cin >> path;
                Node* result = searchByPath(root, path);
                if (result != nullptr) {
                    cout << "Found: " << result->name << endl;
                }
                break;
            }
            case 10: {
                string filename;
                cout << "Enter filename: ";
                cin >> filename;
                exportFileStructure(root, filename);
                break;
            }
            case 11: {
                string filename;
                cout << "Enter filename: ";
                cin >> filename;
                importFileStructure(root, filename);
                break;
            }
            case 12: {
                // string filename;
                // cout << "Enter filename: ";
                // cin >> filename;
                // importFileStructure(root, filename);
                displayFileStructure(root); // Call the level order display function
                break;
            }
            case 13: {
                int choice;
                cout << endl << "1. Open by name" << endl << "2. Open by path" << endl;
                cin >> choice;
                if (choice == 1) {
                    string directoryName;
                    cout << "Enter directory name: ";
                    cin >> directoryName;
                    openDirectory(root, directoryName);
                } else if (choice == 2) {
                    string path;
                    cout << "Enter path: ";
                    cin >> path;
                    openDirectory(root, path);
                } else {
                    cout << "Invalid choice. Please try again." << endl;
                }
                break;
            }
            case 0: {
                return 0;
            }
            default: {
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }
    }
}
