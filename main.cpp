#include <iostream>
#include <vector>
#include <queue>
#include <set>

using namespace std; // so i dont have to put std everywhere

// Hard coded preloaded puzzles the user can select to compute
vector<vector<int>> trivial1 = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
};

vector<vector<int>> trivial2 = {
    {1, 2, 3},
    {4, 5, 6},
    {0, 7, 8} 
};

vector<vector<int>> easy1 = {
    {1, 2, 3},
    {5, 0, 6},
    {4, 7, 8}
};

vector<vector<int>> easy2 = {
    {1, 3, 6},
    {5, 0, 2},
    {4, 7, 8}
};

vector<vector<int>> med1 = {
    {1, 3, 6},
    {5, 0, 7},
    {4, 8, 2}
};

vector<vector<int>> med2 = {
    {1, 6, 7},
    {5, 0, 3},
    {4, 8, 2}
};

vector<vector<int>> hard1 = {
    {7, 1, 2},
    {4, 8, 5},
    {6, 3, 0}
};

vector<vector<int>> hard2 = {
    {0, 7, 2},
    {4, 6, 1},
    {3, 5, 8}
};

vector<vector<int>> goalState = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
};

struct TreeNode{
    vector<vector<int>> board;
    TreeNode *parent;    // pointer to a parent of the matrices type
    int depth;
    int cost;

    // constructor
    TreeNode(const vector<vector<int>> node, TreeNode* parent, int depth, int cost)
        :board(node), parent(parent), depth(depth), cost(cost){}
};

// Forward function declarations in order to read the code how it runs sequentially
vector<vector<int>> selectPuzzle();
void printPuzzle(const vector<vector<int>>& userPuzzle);
void selectSearchAlgorithm(vector<vector<int>> puzzle);
void uniformCostSearch(vector<vector<int>>& puzzle, int heuristic);


// Prompt user to select a puzzle (see helper selectPuzzle()) or create one
// Passes the users selected puzzle to selectSearchAlgorithm() to proceed with computation
int main() {
    int puzzleType = 0;

    cout << "Hello World! I am THE 8-puzzle Calculator. Enter \"1\" to choose from a seclection of premade puzzles, or enter \"2\" to create your own 8-puzzle!" << endl;
    cin >> puzzleType;
    
    if (puzzleType == 1){
        selectSearchAlgorithm(selectPuzzle());  // user chooses an 8-puzzle from the hard coded vectors, and an algorithm
    }else if (puzzleType == 2){
        // Create unique puzzle
        vector<int> row1, row2, row3;
        cout << "Please enter your custom 8-puzzle over valid digits 0-8, where 0 indicates the blank tile." << endl;
        cout << "Enter the three digits for row 1 (enter after each digit)" << endl;
        for (int i = 0; i < 3; i++){            // get one digit at a time for the first row
            int userDigitInput;     
            cin >> userDigitInput;
            row1.push_back(userDigitInput);     // append the current digit to the end of the row
        }

        cout << "Enter the three digits for row 2 (press enter after each digit)" << endl;
        for (int i = 0; i < 3; i++){            // get one digit at a time for the second row
            int userDigitInput;     
            cin >> userDigitInput;
            row2.push_back(userDigitInput);     // append the current digit to the end of the row
        }

        cout << "Enter the three digits for row 3 (press enter after each digit)" << endl;
        for (int i = 0; i < 3; i++){            // get one digit at a time for the third row
            int userDigitInput;     
            cin >> userDigitInput;
            row3.push_back(userDigitInput);     // append the current digit to the end of the row
        }

        vector<vector<int>> userPuzzle = {row1, row2, row3};
        printPuzzle(userPuzzle);
        selectSearchAlgorithm(userPuzzle);

    }else{
        cout << "Invalid selected puzzle. Please try again." << endl;
    }

    return 0;
}

// User enters a number that indicates the difficulty premade 8-puzzle selected
vector<vector<int>> selectPuzzle(){
    int userPuzzle = 0;
    
    cout << "Please enter the difficulty of the puzzle you would like to compute on a scale of 1 to 8" << endl;
    cin >> userPuzzle;
    
    if (userPuzzle == 1){
        cout << "You selected difficulty 1: 8-puzzle trivial-1" << endl;
        printPuzzle(trivial1);
        return trivial1;
    }else if (userPuzzle == 2){
        cout << "You selected difficulty 2: 8-puzzle trivial-2" << endl;
        printPuzzle(trivial2);
        return trivial2;
    }else if (userPuzzle == 3){
        cout << "You selected difficulty 3: 8-puzzle easy-1" << endl;
        printPuzzle(easy1);
        return easy1;
    }else if (userPuzzle == 4){
        cout << "You selected difficulty 4: 8-puzzle easy-2" << endl;
        printPuzzle(easy2);
        return easy2;
    }else if (userPuzzle == 5){
        cout << "You selected difficulty 5: 8-puzzle medium-1" << endl;
        printPuzzle(med1);
        return med1;
    }else if (userPuzzle == 6){
        cout << "You selected difficulty 6: 8-puzzle medium-2" << endl;
        printPuzzle(med2);
        return med2;
    }else if (userPuzzle == 7){
        cout << "You selected difficulty 7: 8-puzzle hard-1" << endl;
        printPuzzle(hard1);
        return hard1;
    }else if (userPuzzle == 8){
        cout << "You selected difficulty 8: 8-puzzle hard-2" << endl;
        printPuzzle(hard2);
        return hard2;
    }

    //otherwise, just choose the hardest one >:)
    cout << "Invalid difficulty. Commencing hardest puzzle..." << endl;
    printPuzzle(hard2);
    return hard2;
}

//const pass by reference to view and print the 8-puzzles
void printPuzzle(const vector<vector<int>>& userPuzzle){
    // nested for loop for 2D [i,j] indexed vector
    for (int i = 0; i < userPuzzle.size(); i++){
        for (int j = 0; j < userPuzzle[i].size(); j++){
            cout << userPuzzle[i][j] << " ";
        }
        cout << endl;
    }
}

// User selects an algorithm to run on there chosen 8-puzzle.
void selectSearchAlgorithm(vector<vector<int>> puzzle){
    int selectAlg = 0;
    cout << "Please select an algorithm for your puzzle. Enter:" << endl;
    cout << "\"1\" for Uniform Cost Search" << endl;
    cout << "\"2\" for the Misplaced Tile Heuristic" << endl;
    cout << "\"3\" for the Manhattan Heuristic" << endl;
    cin >> selectAlg;

    if (selectAlg == 1){
        uniformCostSearch(puzzle, 0);   // No heuristic, i.e. basically BFS
    }else if (selectAlg == 2){
        uniformCostSearch(puzzle, 1);   // Misplaced Tile heuristic
    }else{
        uniformCostSearch(puzzle, 2);   // Manhattan heuristic
    }

    return;
}

void uniformCostSearch(vector<vector<int>>& puzzle, int heuristic){
    TreeNode* startState = new TreeNode(puzzle, nullptr, 0, 0);    
    queue<TreeNode*> FIFO;                          // Creates a First In First Out data structure for BFS
    FIFO.push(startState);                          // Start the working queue with the initial puzzle
    set<vector<vector<int>>> visited;               // Keeps track of children to prevent repeat visits to the identical states
    int currQueueSize = 1;                          // 1 so far since the root startState
    int maxQueueSize = 1;                           
    int maxNodesExpanded = 0;

    while(!FIFO.empty()){                           // If the working queue is empty, then I have checked the entire search space and found no answer (or the puzzle was invalid)
        TreeNode* currNode = FIFO.front();          // Store temporarily to perform operations then move to visited, also used to store parent for children
        FIFO.pop();                                 // Remove the current node from FIFO
        maxQueueSize--;

        if (currNode->board == goalState){           // If goalState is found, return the BFS 
            cout << "Puzzle Solved!" << endl;
            printPuzzle(currNode->board);            // change to show path later?
            cout << "Depth: " << currNode->depth << endl;
            cout << "Max Queue Size: " << maxQueueSize << endl;
            cout << "Max Nodes Expanded: " << maxNodesExpanded << endl;
            // Print path?
            return;                                  // end search now that the goal state is reached
        }


        // PERFORM UCS -------------------
        // Locate the empty tile position 0 by traversing the array
        int zeroI = -1, zeroJ = -1;          
        for (int i = 0; i < 3 && zeroI == -1; i++){         // && zeroI = -1 Since I dont need to search the rest of the matrix for 0 if I already found it
            for (int j = 0; j < 3; j++){
                if (currNode->board[i][j] == 0){
                    zeroI = i;
                    zeroJ = j;
                }
            }
        }

        // Check for valid swaps by using a 3x3 bound for the matrices
        // Check up swap
        // ADD FUNCTION FOR HEURISTICS TO COST
        if (zeroI - 1 >= 0){    // if this condition is valid, then I need to make a new child with a board and link to parent
            vector<vector<int>> childBoard = currNode->board;
            swap(childBoard[zeroI][zeroJ], childBoard[zeroI - 1][zeroJ]);
            if (visited.find(childBoard) == visited.end()){                   // the new childBoard hasn't been visted before (prevents moving back and forth between the same swaps)
                FIFO.push(new TreeNode(childBoard, currNode, currNode->depth + 1, currNode->cost + 1));  // put the new child on the working queue
                visited.insert(childBoard);
                currQueueSize++;    // keep track of pushes to the queue
            }
        }
        // Check down swap
        if (zeroI + 1 <= 2){ 
            vector<vector<int>> childBoard = currNode->board;
            swap(childBoard[zeroI][zeroJ], childBoard[zeroI + 1][zeroJ]);
            if (visited.find(childBoard) == visited.end()){
                FIFO.push(new TreeNode(childBoard, currNode, currNode->depth + 1, currNode->cost + 1));
                visited.insert(childBoard);
                currQueueSize++;
            }
        }
        // Check left swap
        if (zeroJ - 1 >= 0){
            vector<vector<int>> childBoard = currNode->board;
            swap(childBoard[zeroI][zeroJ], childBoard[zeroI][zeroJ - 1]);
            if (visited.find(childBoard) == visited.end()){
                FIFO.push(new TreeNode(childBoard, currNode, currNode->depth + 1, currNode->cost + 1));
                visited.insert(childBoard);
                currQueueSize++;
            }
        }
        // Check right swap
        if (zeroJ + 1 <= 2){
            vector<vector<int>> childBoard = currNode->board;
            swap(childBoard[zeroI][zeroJ], childBoard[zeroI][zeroJ + 1]);
            if (visited.find(childBoard) == visited.end()){
                FIFO.push(new TreeNode(childBoard, currNode, currNode->depth + 1, currNode->cost + 1));
                visited.insert(childBoard);
                currQueueSize++;
            }
        }
        
        maxQueueSize = max(maxQueueSize, currQueueSize);    // update max queue size after checking all 4 possible children
        maxNodesExpanded++;                                 // the current board has visited all its possible children
    }
    
    // return "failure" (no solution)
    cout << "failure, there is no solution in the search space, or the 8-puzzle is invalid" << endl;
};