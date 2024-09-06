#include <iostream>
#include <iomanip>
#include <string>

// Class to represent a sparse matrix using a linked list structure
class SparseMatrix {
private:
    // Node structure to represent each non-zero element in the matrix
    struct Node {
        int row, col, value;
        Node* right;  // Pointer to the next element in the same row
        Node* down;   // Pointer to the next element in the same column
        Node(int r, int c, int v) : row(r), col(c), value(v), right(nullptr), down(nullptr) {}
    };

    Node* headRow;  // Pointer to the first node in the row-wise list
    Node* headCol;  // Pointer to the first node in the column-wise list
    int numRows, numCols;  // Dimensions of the matrix

    // Helper function to insert a node in row-wise order
    void insertRowWise(Node* newNode) {
        // If the list is empty or new node should be the first in the list
        if (!headRow || newNode->row < headRow->row) {
            newNode->right = headRow;
            headRow = newNode;
        } else {
            Node* current = headRow;
            // Traverse the list to find the correct position
            while (current->right && current->right->row <= newNode->row) {
                // If same row, ensure column-wise ordering
                if (current->right->row == newNode->row && current->right->col > newNode->col) break;
                current = current->right;
            }
            // Insert the new node
            newNode->right = current->right;
            current->right = newNode;
        }
    }

    // Helper function to insert a node in column-wise order
    void insertColWise(Node* newNode) {
        // Similar to insertRowWise, but for column-wise ordering
        if (!headCol || newNode->col < headCol->col) {
            newNode->down = headCol;
            headCol = newNode;
        } else {
            Node* current = headCol;
            while (current->down && current->down->col <= newNode->col) {
                if (current->down->col == newNode->col && current->down->row > newNode->row) break;
                current = current->down;
            }
            newNode->down = current->down;
            current->down = newNode;
        }
    }

public:
    // Constructor to initialize the sparse matrix
    SparseMatrix(int rows, int cols) : headRow(nullptr), headCol(nullptr), numRows(rows), numCols(cols) {}

    // Method to add a new element to the sparse matrix
    void addElement(int row, int col, int value) {
        if (value == 0) return; // Don't add zero elements to maintain sparsity

        Node* newNode = new Node(row, col, value);
        insertRowWise(newNode);  // Insert into row-wise list
        insertColWise(newNode);  // Insert into column-wise list
    }

    // Method to display the sparse matrix in linked list format
    void display() const {
        std::cout << "Sparse Matrix Representation (Linked List):\n";
        Node* current = headRow;
        while (current) {
            std::cout << "[" << current->row << " " << current->col << " " << current->value << "]";
            if (current->right) {
                std::cout << " -> ";
            } else {
                std::cout << " -> NULL\n";
            }
            current = current->right;
        }
    }

    // Method to display the original matrix (including zero elements)
    void displayOriginal() const {
        std::cout << "Original Matrix:\n";
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                Node* current = headRow;
                // Search for the element in the linked list
                while (current && (current->row < i || (current->row == i && current->col < j))) {
                    current = current->right;
                }
                // If element found, display its value; otherwise, display 0
                if (current && current->row == i && current->col == j) {
                    std::cout << std::setw(2) << current->value << " ";
                } else {
                    std::cout << std::setw(2) << "0 ";
                }
            }
            std::cout << "\n";
        }
    }

    // Method to display the matrix in a column-wise traversal
    void displayColumnWise() const {
        std::cout << "Column-wise Traversal:\n";
        Node* current = headCol;
        while (current) {
            std::cout << "Column " << current->col << ": ";
            Node* rowNode = current;
            while (rowNode) {
                std::cout << "[" << rowNode->row << "," << rowNode->value << "] ";
                rowNode = rowNode->down;
            }
            std::cout << "\n";
            current = current->right;
        }
    }

    // Method to calculate the sum of elements in a specific row
    int getRowSum(int row) const {
        int sum = 0;
        Node* current = headRow;
        while (current && current->row <= row) {
            if (current->row == row) {
                sum += current->value;
            }
            current = current->right;
        }
        return sum;
    }

    // Destructor to free allocated memory
    ~SparseMatrix() {
        Node* current = headRow;
        while (current) {
            Node* next = current->right;
            delete current;
            current = next;
        }
    }
};

int main() {
    // Create a sparse matrix with dimensions 4x5
    SparseMatrix sparseMatrix(4, 5);

    // Add non-zero elements to the sparse matrix
    sparseMatrix.addElement(0, 2, 3);
    sparseMatrix.addElement(0, 4, 4);
    sparseMatrix.addElement(1, 2, 5);
    sparseMatrix.addElement(1, 3, 7);
    sparseMatrix.addElement(3, 1, 2);
    sparseMatrix.addElement(3, 2, 6);

    // Display the original matrix (including zero elements)
    std::cout << "Original Matrix:\n";
    sparseMatrix.displayOriginal();

    // Display the sparse matrix representation
    std::cout << "\nSparse Matrix Representation:\n";
    sparseMatrix.display();

    // Display the matrix in column-wise traversal
    std::cout << "\nColumn-wise Traversal:\n";
    sparseMatrix.displayColumnWise();

    // Calculate and display the sum of elements in row 1
    std::cout << "\nSum of elements in row 1: " << sparseMatrix.getRowSum(1) << std::endl;

    return 0;
}
