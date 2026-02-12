#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <chrono>

// so i dont have to put std everywhere
using namespace std;
using namespace std::chrono;

// Hard coded puzzles the user can select to compute
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

// Object for handling the variables puzzle board, parent, depth, and cost for each respective state
struct TreeNode{
    vector<vector<int>> board;
    TreeNode *parent;    // pointer to a parent of the matrices type
    int depth;
    int cost;

    // constructor
    TreeNode(const vector<vector<int>>& node, TreeNode* parent, int depth, int cost)
        :board(node), parent(parent), depth(depth), cost(cost){}
};

// Object so that the priority queue knows what variable (cost) to order on
struct Compare{
    bool operator()(const TreeNode* a, const TreeNode* b) const{
        return a->cost > b->cost;
    }
};

// Forward function declarations in order to read the code how it runs sequentially
vector<vector<int>> selectPuzzle();
void printPuzzle(const vector<vector<int>>& userPuzzle);
void selectSearchAlgorithm(const vector<vector<int>>& puzzle);
void uniformCostSearch(const vector<vector<int>>& puzzle, int heuristic);
int misplacedTile(const vector<vector<int>>& childBoard);
int manhattan(const vector<vector<int>>& childBoard);


// Prompt user to select a puzzle (see helper selectPuzzle()) or create one
// Passes the users selected puzzle to selectSearchAlgorithm() to proceed with computation
int main() {
    int puzzleType = 0;

    cout << "Hello World! I am THE 8-puzzle Calculator. Enter \"1\" to choose from a selection of premade puzzles, or enter \"2\" to create your own 8-puzzle!" << endl;
    cin >> puzzleType;
    
    if (puzzleType == 1){
        selectSearchAlgorithm(selectPuzzle());  // user chooses an 8-puzzle from the hard coded vectors, and an algorithm
    }else if (puzzleType == 2){                 // Create unique puzzle
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

// User enters a number that indicates the difficulty which correlates to the premade hard coded 8-puzzles
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
        cout << "[ ";
        for (int j = 0; j < userPuzzle[i].size(); j++){
            cout << userPuzzle[i][j];
            if (j != userPuzzle[i].size() - 1){
                cout << ", ";
            }
        }
        cout << " ]" << endl;
    }
}

// User selects an algorithm to run on there chosen 8-puzzle.
void selectSearchAlgorithm(const vector<vector<int>>& puzzle){
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

void uniformCostSearch(const vector<vector<int>>& puzzle, int heuristic){
    TreeNode* startState = new TreeNode(puzzle, nullptr, 0, 0);                                   // first state in the working queue
    priority_queue<TreeNode*, vector<TreeNode*>, Compare> priorityQueue;                          // The priority queue will act as FIFO with uniform cost
    priorityQueue.push(startState);                          // Start the working queue with the initial puzzle
    
    set<vector<vector<int>>> visited;               // Keeps track of states fully explored to prevent repeat visits to the identical states
    int currQueueSize = 1;                          // 1 so far since the root startState
    int maxQueueSize = 1;                           // compares with current max queue size for updating
    int maxNodesExpanded = 0;                       // updates each time all possible children of a state are explored for the current board
    
    // declare clock start and stop variables
    high_resolution_clock::time_point start;
    high_resolution_clock::time_point stop;
    start = high_resolution_clock::now();                  // starts the timer

    while(!priorityQueue.empty()){                         // If the working queue is empty, then I have checked the entire search space and found no answer (or the puzzle was invalid)
        TreeNode* currNode = priorityQueue.top();          // Store temporarily to perform operations then move to visited, also used to store parent for children
        priorityQueue.pop();                               // Remove the current node from priorityQueue
        maxQueueSize--;

        if (!visited.count(currNode->board)){           // if this is a new board, expand, otherwise skip to next board state iteration
            visited.insert(currNode->board);            // Now that the currBoard has been popped and is being expanded, add it to visited
            int hOfN = 0;                               // Set to corresponding heuristic, and output the h(n) details for the current state
            if (heuristic == 1){
                hOfN = misplacedTile(currNode->board);
            }else if (heuristic == 2){
                hOfN = manhattan(currNode->board);
            }
            cout << "The best state to expand with a g(n) = " << currNode->depth << " and h(n) = " << hOfN << " is:" << endl;
            printPuzzle(currNode->board);

            if (currNode->board == goalState){                      // If goalState is found...
                stop = high_resolution_clock::now();                // ends timer
                duration<double, milli> duration = stop - start;    // converts the difference of time to double milliseconds type
                
                // output computation details
                cout << "Puzzle Solved!" << endl << endl;
                cout << "Solution depth was : " << currNode->depth << endl;
                cout << "Number of nodes expanded: " << maxNodesExpanded << endl;
                cout << "Max queue size: " << maxQueueSize << endl;
                cout << "Completion Time: " << duration.count() << " milliseconds" << endl;
                return;                                             // end search now that the goal state is reached
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

            // 4 Checks for valid swaps (up/down/left/right) by using a 3x3 bound for the matrices
            // Check if up swap is valid, and create the child state with the proper cost according the heuristic
            if (zeroI - 1 >= 0){    // if this condition is valid, then I need to make a new child with a board and link to parent
                vector<vector<int>> childBoard = currNode->board;                           // create the new board premove
                swap(childBoard[zeroI][zeroJ], childBoard[zeroI - 1][zeroJ]);               // slide the upper tile into 0 position
                if (heuristic == 0){                                          // put the new child on the working queue, updating cost according to the heuristic selected
                    priorityQueue.push(new TreeNode(childBoard, currNode, currNode->depth + 1, currNode->cost + 1));  
                }else if (heuristic == 1){
                    priorityQueue.push(new TreeNode(childBoard, currNode, currNode->depth + 1, currNode->depth + 1 + misplacedTile(childBoard)));  // switch cost to f(n) = g(n) + h(n) for A* heuristics
                }else if (heuristic == 2){
                    priorityQueue.push(new TreeNode(childBoard, currNode, currNode->depth + 1, currNode->depth + 1 + manhattan(childBoard)));
                }
                currQueueSize++;    // keep track of pushes to the queue
            }
            // Check if down swap is valid, and create the child state with the proper cost according the heuristic
            if (zeroI + 1 <= 2){ 
                vector<vector<int>> childBoard = currNode->board;
                swap(childBoard[zeroI][zeroJ], childBoard[zeroI + 1][zeroJ]);
                if (heuristic == 0){                                          // put the new child on the working queue, updating cost according to the heuristic selected
                    priorityQueue.push(new TreeNode(childBoard, currNode, currNode->depth + 1, currNode->cost + 1));  
                }else if (heuristic == 1){
                    priorityQueue.push(new TreeNode(childBoard, currNode, currNode->depth + 1, currNode->depth + 1 + misplacedTile(childBoard)));  
                }else if (heuristic == 2){
                    priorityQueue.push(new TreeNode(childBoard, currNode, currNode->depth + 1, currNode->depth + 1 + manhattan(childBoard)));  
                }
                currQueueSize++;
            }
            // Check if left swap is valid, and create the child state with the proper cost according the heuristic
            if (zeroJ - 1 >= 0){
                vector<vector<int>> childBoard = currNode->board;
                swap(childBoard[zeroI][zeroJ], childBoard[zeroI][zeroJ - 1]);
                if (heuristic == 0){                                          // put the new child on the working queue, updating cost according to the heuristic selected
                    priorityQueue.push(new TreeNode(childBoard, currNode, currNode->depth + 1, currNode->cost + 1));  
                }else if (heuristic == 1){
                    priorityQueue.push(new TreeNode(childBoard, currNode, currNode->depth + 1, currNode->depth + 1 + misplacedTile(childBoard)));  
                }else if (heuristic == 2){
                    priorityQueue.push(new TreeNode(childBoard, currNode, currNode->depth + 1, currNode->depth + 1 + manhattan(childBoard)));  
                }
                currQueueSize++;
            }
            // Check if right swap is valid, and create the child state with the proper cost according the heuristic
            if (zeroJ + 1 <= 2){
                vector<vector<int>> childBoard = currNode->board;
                swap(childBoard[zeroI][zeroJ], childBoard[zeroI][zeroJ + 1]);
                if (heuristic == 0){                                         // put the new child on the working queue, updating cost according to the heuristic selected
                    priorityQueue.push(new TreeNode(childBoard, currNode, currNode->depth + 1, currNode->cost + 1));  
                }else if (heuristic == 1){
                    priorityQueue.push(new TreeNode(childBoard, currNode, currNode->depth + 1, currNode->depth + 1 + misplacedTile(childBoard)));  
                }else if (heuristic == 2){
                    priorityQueue.push(new TreeNode(childBoard, currNode, currNode->depth + 1, currNode->depth + 1 + manhattan(childBoard)));  
                }
                currQueueSize++;
            }
            
            maxQueueSize = max(maxQueueSize, currQueueSize);    // update max queue size after checking all 4 possible children
            maxNodesExpanded++;                                 // the current board has visited all its possible children
        }
    }
    // return "failure" (no solution)
    cout << "failure, there is no solution in the search space, or the 8-puzzle is invalid" << endl;
};

// Counts the number of that aren't in its solved state
int misplacedTile(const vector<vector<int>>& childBoard){
    int misplaced = 0;
    for (int i = 0; i < 3; i++){                            // traverse each position of a board
        for (int j = 0; j < 3; j++){
            if (childBoard[i][j] != goalState[i][j] && childBoard[i][j] != 0){       // compare the current board to the goal board at the specified psoition
                misplaced+=1;                               // if the tile is not in the goal position, increases the cost by 1     
            }
        }
    }
    return misplaced;
}

// Totals the counts of distance from tiles to their solved state
int manhattan(const vector<vector<int>>& childBoard){
    int distance = 0;               // cost is the difference in distance from the goal position
    int goalNum = 0;                // stores the value at the indices for the board goalState
    int currNum = 0;                // stores the value at the indices for the board childBoard

    // traverse the goalState board
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            goalNum = goalState[i][j];                          // store the current position value at goalState

            if (goalNum != 0){                                      // exclude 0
                // traverse the childBoard, comparing each position to each position on the goalState board
                for (int x = 0; x < 3; x++){
                    for (int y = 0; y < 3; y++){
                        currNum = childBoard[x][y];                 // store the current position value at childBoard
                        if (currNum == goalNum && currNum!= 0){     // compare if the values are the same
                            distance+= abs(x-i) + abs(y-j);         // calculate the difference in distance between the goalState position and childBoard position
                        }
                    }
                }
            }
        }
    }
    return distance;
}