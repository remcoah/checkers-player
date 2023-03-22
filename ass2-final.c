/* Program to print and play checker games.

  Skeleton program written by Artem Polyvyanyy, artem.polyvyanyy@unimelb.edu.au,
  September 2021, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.

  Student Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: Remco Holstege 1217370
  Dated:     30/09/2021

*/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <string.h>

/* #define's -----------------------------------------------------------------*/
#define BOARD_SIZE          8       // board size
#define ROWS_WITH_PIECES    3       // number of initial rows with pieces
#define CELL_EMPTY          '.'     // empty cell character
#define CELL_BPIECE         'b'     // black piece character
#define CELL_WPIECE         'w'     // white piece character
#define CELL_BTOWER         'B'     // black tower character
#define CELL_WTOWER         'W'     // white tower character
#define COST_PIECE          1       // one piece cost
#define COST_TOWER          3       // one tower cost
#define TREE_DEPTH          3       // minimax tree depth
#define COMP_ACTIONS        10      // number of computed actions
#define MAX_INPT_LEN        6       // max length of input line
#define MIN_BOARD           0       // minimum value for row/col
#define MAX_BOARD           7       // maximum value for row/col
#define MAX_CHKRS           12      // maximum amount of checkers a player has

// output strings
#define BRD_DVDR "=====================================\n" // board divider
#define COL_HDR "     A   B   C   D   E   F   G   H\n"     // column header
#define HB_LINE "   +---+---+---+---+---+---+---+---+\n"// horizontal board line

/* type definitions  ---------------------------------------------------------*/
typedef unsigned char board_t[BOARD_SIZE][BOARD_SIZE];  // board type

typedef struct {                    // cell type
    int row, col;
} cell_t;

typedef struct {                    // action type
    cell_t source, target;
} action_t;

typedef struct treenode treenode_t; // node type for dynamic tree
struct treenode {
    board_t board;
    action_t action, bestnextaction;
    int turn, childfreq, cost;
    treenode_t **children;
};

// taken and modified from Alistair's textbook program "listops.c"
typedef struct node node_t;         // node type for linked list
struct node {
	action_t data;
	node_t *next;
};

// taken and modified from Alistair's textbook program "listops.c"
typedef struct {                    // linked list type
	node_t *head;
	node_t *foot;
} list_t;

/* function prototypes -------------------------------------------------------*/
int         col_to_int(char col);
char        int_to_col(int col);
void        set_board(board_t board);
char        row_piece(int row);
void        print_board(board_t board);
void        print_action(action_t action);
int         cost(board_t board);
action_t    cnvrt_action(char* str);
list_t*     make_empty_list(void);
int         list_length(list_t *list);
int         is_empty_list(list_t *list);
void        free_list(list_t *list);
list_t*     insert_at_foot(list_t *list, action_t value);
action_t    get_head(list_t *list);
list_t*     remove_head(list_t *list);
int         read_line(char* str);
char        read_input_to(list_t* inputlist);
void        error_action(board_t board, action_t action, int turn);
int         valid_action(board_t board, action_t action, int turn);
int         is_legal_action(board_t board, action_t action, int turn);
void        do_action(board_t board, action_t action, int *turn);
void        copy_board(board_t dest, board_t src);
int         oppchkr(char chkr1, char chkr2);
cell_t      northwest(cell_t cell);
cell_t      northeast(cell_t cell);
cell_t      southwest(cell_t cell);
cell_t      southeast(cell_t cell);
list_t*     find_actions(board_t board, int turn);
int         best_next_cost(treenode_t *node, int depth);
void        add_children(treenode_t *parent, int depth);
treenode_t* add_leaf(treenode_t* parent, action_t action);
void        recursive_free_tree(treenode_t* leaf);
int         do_best_action(board_t board, int *turn);

/* where it all happens ------------------------------------------------------*/
int main(int argc, char *argv[]) {
    // IMLEMENT STAGE 0
    board_t board;
    set_board(board);
    int turns = 0;

    // reading actions
    list_t* actionlist = make_empty_list();
    char input_act = read_input_to(actionlist);

    // printing initial board
    printf("BOARD SIZE: %dx%d\n", BOARD_SIZE, BOARD_SIZE);
    printf("#BLACK PIECES: %d\n#WHITE PIECES: %d\n", MAX_CHKRS, MAX_CHKRS);
    print_board(board);

    // print actions and updated board
    action_t action;
    while (!is_empty_list(actionlist)) {
        action = get_head(actionlist);
        actionlist = remove_head(actionlist);
        do_action(board, action, &turns);

        // action is valid and has been implemented, print updated board
        printf(BRD_DVDR);
        if(turns%2 != 0) {
            // odd turns have occurred, action was black's
            printf("BLACK ACTION #%d: ", turns);
        } else {
            // even turns have occurred, action was white's
            printf("WHITE ACTION #%d: ", turns);
        }

        print_action(action);
        printf("\nBOARD COST: %d\n", cost(board));
        print_board(board);
    }
    free_list(actionlist);

    // IMPLEMENT STAGE 1 if required
    if (input_act == 'A') {
        do_best_action(board, &turns);
    }

    // IMPLEMENT STAGE 2 if required
    if (input_act == 'P') {
        int i=0;
        while(i<COMP_ACTIONS && do_best_action(board, &turns)) {
            i++;
        }
    }

    return EXIT_SUCCESS;            // exit program with the success code
}

// converts column ASCII representation to its number
int col_to_int(char col) {
    return col - 'A';
}

// converts column number to its ASCII representation 
char int_to_col(int col) {
    return col + 'A';
}

// sets pieces on a given board
void set_board(board_t board) {
    // iterate over each cell
    for(int row=0; row<BOARD_SIZE; row++) {
        for(int col=0; col<BOARD_SIZE; col++) {
            if(row%2 == 0) {
                // even row, so even columns are empty
                if(col%2 == 0) {
                    board[row][col] = CELL_EMPTY;
                } 
                else {
                    // add piece corresponding to the row
                    board[row][col] = row_piece(row);
                }
            }
            else {
                // odd row, so odd columns are empty
                if(col%2 != 0) {
                    board[row][col] = CELL_EMPTY;
                } 
                else {
                    // add piece corresponding to the row
                    board[row][col] = row_piece(row);
                }
            }
        }
    }
}

// returns corresponding piece depending on row of board
char row_piece(int row) {
    if(row < ROWS_WITH_PIECES) {
        // at top of board, so piece is white
        return CELL_WPIECE;
    } 
    else if (row >= BOARD_SIZE-ROWS_WITH_PIECES) {
        // at bottom of board, so piece is black
        return CELL_BPIECE;
    } 
    else {
        // at middle of board, so no piece is in this cell
        return CELL_EMPTY;
    }
}

// prints board in required format
void print_board(board_t board) {
    // print column header and horizontal line
    printf(COL_HDR);
    printf(HB_LINE);

    for(int row=0; row<BOARD_SIZE; row++) {
        // label row
        printf("%2d |", row + 1);
        for(int col=0; col<BOARD_SIZE; col++) {
            printf(" %c |", board[row][col]);
        }
        
        // row complete, print horizontal line
        printf("\n%s", HB_LINE);
    }
}

// prints action data type out as string
void print_action(action_t action) {
    printf("%c%d-%c%d", int_to_col(action.source.col), action.source.row+1, 
    int_to_col(action.target.col), action.target.row+1);
}

// calculates the cost of a board
int cost(board_t board) {
    int cost = 0;
    for(int row = 0; row < BOARD_SIZE; row++) {
        for(int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col] == CELL_BPIECE) {
                cost += COST_PIECE;
            } else if (board[row][col] == CELL_BTOWER) {
                cost += COST_TOWER;
            } else if (board[row][col] == CELL_WPIECE) {
                cost -= COST_PIECE;
            } else if (board[row][col] == CELL_WTOWER) {
                cost -= COST_TOWER;
            }
            
        }
    }
    return cost;
}

// converts string in format %c%d-%c%d to actiontype
action_t cnvrt_action(char* str) {
    char straction[MAX_INPT_LEN];
    strcpy(straction, str);
    if(strlen(straction) != MAX_INPT_LEN-1) {
        printf("ERROR: cnvrt_action() string is not formatted correctly.\n");
        exit(EXIT_FAILURE);
    }
    action_t action;
    action.source.col = col_to_int(straction[0]);
    action.source.row = straction[1] - '1';
    action.target.col = col_to_int(straction[3]);
    action.target.row = straction[4] - '1';
    return action;
}

// FROM ALISTAIR'S TEXTBOOK - makes empty list and returns pointer to it
list_t* make_empty_list(void) {
	list_t *list;
	list = (list_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;
	return list;
}

// returns length of linked list
int list_length(list_t *list){
    int len = 0;
    node_t* temp = list->head;
    while(temp) {
        len++;
        temp = temp->next;
    }
    return len;
}

// FROM ALISTAIR'S TEXTBOOK - determines if list is empty
int is_empty_list(list_t *list) {
	assert(list!=NULL);
	return list->head==NULL;
}

// FROM ALISTAIR'S TEXTBOOK - frees all elements in list
void free_list(list_t *list) {
	node_t *curr, *prev;
	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}
	free(list);
    list = NULL;
}

// FROM ALISTAIR's TEXTBOOK - inserts value to foot of list
list_t* insert_at_foot(list_t *list, action_t value) {
	node_t *new;
	new = (node_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	new->data = value;
	new->next = NULL;
	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = new;
	} else {
		list->foot->next = new;
		list->foot = new;
	}
	return list;
}

// MODIFIED FROM ALISTAIR's TEXTBOOK - returns value at head of list
action_t get_head(list_t *list) {
	assert(list!=NULL && list->head!=NULL);
	return list->head->data;
}

// FROM ALISTAIR's TEXTBOOK removes first value of list
list_t* remove_head(list_t *list) {
	node_t *oldhead;
	assert(list!=NULL && list->head!=NULL);
	oldhead = list->head;
	list->head = list->head->next;
	if (list->head==NULL) {
		/* the only list node just got deleted */
		list->foot = NULL;
	}
	free(oldhead);
	return list;
}

// reads line from stdin to str and returns boolean value showing if line read
int read_line(char* str) {
    int n=0, c;
    while ((c=getchar()) != EOF && c!='\n') {
        str[n++] = c;
    }
    if (c==EOF && n==0) {
        return 0;
    }
    str[n++] = '\0';
    return 1;
}

// reads move list from stdin, adds actions to inputlist, and returns command 
char read_input_to(list_t* inputlist) {
    char line[MAX_INPT_LEN];
    char command = 0;
    int isread = read_line(line);
    while(isread) {
        if(strlen(line) == 1) {
            // line is a command
            command = line[0];
        } 
        else {
            // line is an action
            action_t action = cnvrt_action(line);
            inputlist = insert_at_foot(inputlist, action);
        }
        isread = read_line(line);
    }
    return command;
}

// checks for errors for any action, given board, action, and amount of turns
// occurred, prints error message and exits if action is not valid
void error_action(board_t board, action_t action, int turn) {
    char sourceckr = board[action.source.row][action.source.col];
    char targetckr = board[action.target.row][action.target.col];
    if ((action.source.col < 0) || (action.source.col >= BOARD_SIZE) || 
    (action.source.row < 0) || (action.source.row >= BOARD_SIZE)) {
        // source cell is out of bounds
        printf("ERROR: Source cell is outside of the board.\n");
        exit(EXIT_FAILURE);
    } 
    else if ((action.target.col < 0) || (action.target.col >= BOARD_SIZE) || 
    (action.target.row < 0) || (action.target.row >= BOARD_SIZE)) {
        // target cell is out of bounds
        printf("ERROR: Target cell is outside of the board.\n");
        exit(EXIT_FAILURE);
    } 
    else if(sourceckr == CELL_EMPTY) {
        // source cell is empty
        printf("ERROR: Source cell is empty.\n");
        exit(EXIT_FAILURE);
    } 
    else if (targetckr != CELL_EMPTY) {
        // target cell is not empty
        printf("ERROR: Target cell is not empty.\n");
        exit(EXIT_FAILURE);
    } 
    else if ((turn%2 == 0) && ((sourceckr != CELL_BPIECE) && (sourceckr 
    != CELL_BTOWER))) {
        // even turns occurred (black's turn) and source holds white checker
        printf("ERROR: Source cell holds opponent's piece/tower.\n");
        exit(EXIT_FAILURE);
    } 
    else if ((turn%2 != 0) && ((sourceckr != CELL_WPIECE) && (sourceckr 
    != CELL_WTOWER))) {
        // odd turns occurred (white's turn) and source holds black checker
        printf("ERROR: Source cell holds opponent's piece/tower.\n");
        exit(EXIT_FAILURE);
    } 
    else if (!is_legal_action(board, action, turn)) {
        // action is not legal
        printf("ERROR: Illegal action.\n");
        exit(EXIT_FAILURE);
    }
}

// determines if actions are valid, similar to erroraction, but does not exit 
int valid_action(board_t board, action_t action, int turn) {
    char sourceckr = board[action.source.row][action.source.col];
    char targetckr = board[action.target.row][action.target.col];
    if(sourceckr == CELL_EMPTY) {
        // source cell is empty
        return 0;
    } 
    else if (targetckr != CELL_EMPTY) {
        // target cell is not empty
        return 0;
    } 
    else if ((action.source.col < 0) || (action.source.col >= BOARD_SIZE) || 
    (action.source.row < 0) || (action.source.row >= BOARD_SIZE)) {
        // source cell is out of bounds
        return 0;
    } 
    else if ((action.target.col < 0) || (action.target.col >= BOARD_SIZE) || 
    (action.target.row < 0) || (action.target.row >= BOARD_SIZE)) {
        // target cell is out of bounds
        return 0;
    } 
    else if ((turn%2 == 0) && ((sourceckr != CELL_BPIECE) && (sourceckr 
    != CELL_BTOWER))) {
        // even turns occurred (black's turn) and source holds white checker
        return 0;
    }
    else if ((turn%2 != 0) && ((sourceckr != CELL_WPIECE) && (sourceckr 
    != CELL_WTOWER))) {
        // odd turns occurred (white's turn) and source holds black checker
        return 0;
    } 
    else if (!is_legal_action(board, action, turn)) {
        // action is not legal
        return 0;
    } 
    else {
        // all errors checked, action is valid
        return 1;
    }
}

// determines if action is legal, given whos turn
// assumes source and target cells are in bounds, source cell is not empty, 
// target cell is empty, and correct player's checker is being moved
int is_legal_action(board_t board, action_t action, int turn) {
    char chkr = board[action.source.row][action.source.col];
    if (abs(action.source.col-action.target.col) != 
    abs(action.source.row-action.target.row)) {
        // action is not diagonal, not legal
        return 0;
    } 
    else if(turn%2 == 0) {
        // even turns have occurred, black's turn
        if ((chkr == CELL_BPIECE) && (action.source.row < action.target.row)) {
            // checker is a piece, can only move north, action is not legal
            return 0;
        } 
        else if (abs(action.source.col-action.target.col==2) && 
                abs(action.source.row-action.target.row)==2) {
            // is capture, check if valid (white checker is being captured)
            int rowchange = action.target.row - action.source.row;
            int colchange = action.target.col - action.source.col;
            char trgtchkr = board[action.source.row+(rowchange/2)]
                                 [action.source.col+(colchange/2)];
            if (!oppchkr(chkr,trgtchkr)) {
                // black trying to capture black cell, not legal
                return 0;
            }
        }
    } 
    else if (turn%2 != 0) {
        // odd turns have occurred, whites turn
        if ((chkr == CELL_WPIECE) && (action.source.row > action.target.row)) {
            // checker is a piece, can only move south, action is not legal
            return 0;
        } 
        else if (abs(action.source.col-action.target.col==2) && 
                 abs(action.source.row-action.target.row)==2) {
            // is capture, check if valid (black checker is being captured)
            int rowchange = action.target.row - action.source.row;
            int colchange = action.target.col - action.source.col;
            char trgtchkr = board[action.source.row+(rowchange/2)]
                                 [action.source.col+(colchange/2)];
            if (!oppchkr(chkr,trgtchkr)) {
                // white trying to capture white cell, not legal
                return 0;
            }
        }
    }
    // all conditions checked, move is legal
    return 1;
}

// implements action on board if valid, increases turns by 1
void do_action(board_t board, action_t action, int* turn) {
    error_action(board, action, *turn);       // check if action is valid
    char chkr = board[action.source.row][action.source.col];

    // check if checker becomes tower
    if((action.target.row == MIN_BOARD) && (chkr == CELL_BPIECE)) {
        // black piece moved to top row, becomes tower
        chkr = CELL_BTOWER;
    } else if ((action.target.row == MAX_BOARD) && chkr == CELL_WPIECE) {
        // white piece moved to bottom row, becomes tower
        chkr = CELL_WTOWER;
    }

    if(abs(action.source.col-action.target.col==2) || 
       abs(action.source.row-action.target.row)==2) {
        // action is capture (has moved 2 cells diagonally),
        // remove captured checker
        int rowchange = action.target.row - action.source.row;
        int colchange = action.target.col - action.source.col;
        board[action.source.row+(rowchange/2)][action.source.col+(colchange/2)]
        = CELL_EMPTY;
    }

    // move checker
    board[action.source.row][action.source.col] = CELL_EMPTY;
    board[action.target.row][action.target.col] = chkr;

    (*turn)++;
}

// copies board to dest from src
void copy_board(board_t dest, board_t src) {
    for (int row=0; row<BOARD_SIZE; row++) {
        for (int col=0; col<BOARD_SIZE; col++) {
            dest[row][col] = src[row][col];
        }
    }
}

// checks if 2 checkers are from opposite teams
int oppchkr(char chkr1, char chkr2) {
    // error checking
    if((chkr1 == CELL_EMPTY) || (chkr2 == CELL_EMPTY)) {
        // checking empty cell
        printf("ERROR: oppchkr() checking empty cell.\n");
        exit(EXIT_FAILURE);
    }

    if(((chkr1 == CELL_BPIECE) || (chkr1 == CELL_BTOWER)) && 
    ((chkr2 == CELL_WPIECE) || (chkr2 == CELL_WTOWER))) {
        return 1;
    } else if(((chkr1 == CELL_WPIECE) || (chkr1 == CELL_WTOWER)) && 
    ((chkr2 == CELL_BPIECE) || (chkr2 == CELL_BTOWER))) {
        return 1;
    }
    return 0;
}

// returns cell in NW (north-west) direction of input cell
cell_t northwest(cell_t cell) {
    cell_t nw;
    nw.row = cell.row - 1;
    nw.col = cell.col - 1;
    return nw;
}

// returns cell in NE (north-east) direction of input cell
cell_t northeast(cell_t cell) {
    cell_t ne;
    ne.row = cell.row - 1;
    ne.col = cell.col + 1;
    return ne;
}

// returns cell in SW (south-west) direction of input cell
cell_t southwest(cell_t cell) {
    cell_t sw;
    sw.row = cell.row + 1;
    sw.col = cell.col - 1;
    return sw;
}

// returns cell in SE (south-east) direction of input cell
cell_t southeast(cell_t cell){
    cell_t se;
    se.row = cell.row + 1;
    se.col = cell.col + 1;
    return se;
}

// makes linkedlist of potential moves and puts them in order as required by
// assignment specification, returns pointer to linkedlist
list_t* find_actions(board_t board, int turn) {
    list_t* movelist = make_empty_list();
    // iterate cells to find potential checkers
    for(int row=0; row<BOARD_SIZE; row++) {
        for(int col=0; col<BOARD_SIZE; col++) {
            if(board[row][col] != CELL_EMPTY) {
                // check for potential actions
                action_t potential;
                potential.source.row = row;
                potential.source.col = col;

                // array of function pointers in order of which directions to 
                // check first
                cell_t (*direction[]) (cell_t) = {northeast, southeast, 
                southwest, northwest};

                // now iterate over the 4 directions to see if an action is
                // possible in that direction
                for(int i=0; i<4; i++) {
                    potential.target = direction[i](potential.source);
                    if(valid_action(board, potential, turn)) {
                        // can move in this direction
                        insert_at_foot(movelist, potential);
                    } else {
                        // check if can capture in this direction, move target
                        // cell in direction once more
                        potential.target = direction[i](potential.target);
                        if(valid_action(board, potential, turn)) {
                            // can capture in this direction
                            insert_at_foot(movelist, potential);
                        }
                    }
                }
            }
        }
    }
    return movelist;
}

// finds best next cost of children leaves, updates node's best next action
int best_next_cost(treenode_t *node, int depth) {
    assert(node != NULL);
    if(depth == 0) {
        // base case, return cost of board
        return cost(node->board);
    } else if (node->childfreq == 0) {
        // no moves can be made, game is over
        if(node->turn%2 == 0) {
            // black can not make any moves
            return INT_MIN;
        } else {
            // white can not make any moves
            return INT_MAX;
        }
    }
    if(node->turn%2 == 0) {
        // even turns have occurred, black's turn, find action based on 
        // highest cost
        int bestcost = INT_MIN;
        for(int i=0;i<node->childfreq; i++) {
            int cmpcost = node->children[i]->cost;
            if(cmpcost > bestcost) {
                bestcost = cmpcost;
                node->bestnextaction = node->children[i]->action;
            }
        }
        return bestcost;
    }
    else {
        // odd turns have occurred, whites turn, find action based on 
        // lowest cost
        int bestcost = INT_MAX;
        for(int i=0;i<node->childfreq; i++) {
            int cmpcost = node->children[i]->cost;
            if(cmpcost < bestcost) {
                bestcost = cmpcost;
                node->bestnextaction = node->children[i]->action;
            }
        }
        return bestcost;
    }
}

// adds children of next potential boards to tree
void add_children(treenode_t *parent, int depth) {
    assert(parent != NULL);
    list_t* nextptnlmoves = find_actions(parent->board, parent->turn);
    parent->childfreq = list_length(nextptnlmoves);

    if((depth == 0) || (list_length(nextptnlmoves) == 0)) {
        // base case, no new children
        parent->childfreq = 0;

    } else {
        // make new level of children
        parent->children = (treenode_t**)malloc(parent->childfreq*
        sizeof(treenode_t*));
        assert(parent->children != NULL);

        // make new children for each possible move
        for(int i=0; i<parent->childfreq; i++) {
            (parent->children)[i] = add_leaf(parent, get_head(nextptnlmoves));
            remove_head(nextptnlmoves);
            add_children((parent->children)[i], depth-1);
        }

    }
    parent->cost = best_next_cost(parent, depth);
    free_list(nextptnlmoves);
}

// adds treenode of updated board, given an action
treenode_t* add_leaf(treenode_t* parent, action_t action) {
    treenode_t* leaf = (treenode_t*)malloc(sizeof(treenode_t));
    assert(leaf != NULL);
    leaf->action = action;
    leaf->turn = parent->turn;
    copy_board(leaf->board, parent->board);
    do_action(leaf->board, action, &(leaf->turn));
    return leaf;
}

// recursive free tree function, frees tree and all its children
void recursive_free_tree(treenode_t* leaf) {
    assert(leaf != NULL);
    if(leaf->childfreq) {
        // leaf has children, recursively free children
        for(int i=0; i<leaf->childfreq; i++) {
            recursive_free_tree(leaf->children[i]);
        }
        free(leaf->children);
    }
    free(leaf);
    leaf = NULL;
}

// implements the next best possible action on board, if no actions are 
// possible, prints winner of the game, returns boolean about whether action
// has been made
int do_best_action(board_t board, int *turn) {
    // a list of potential actions that can be taken from current board
    list_t* ptnlactions;
    ptnlactions = find_actions(board, *turn);
    int possibleactions = list_length(ptnlactions);
    free_list(ptnlactions);

    if(possibleactions == 0) {
        // no more actions can be made, game is over
        if(*turn%2 == 0) {
            // white can not make any more moves
            printf("\nBLACK WIN!\n");
        } else {
            // black can not make any more moves
            printf("\nWHITE WIN!\n");
        }
        return 0;
    }

    printf(BRD_DVDR);

    // make tree to determine the best action
    treenode_t* tree = (treenode_t*)malloc(sizeof(treenode_t));
    assert(tree != NULL);
    copy_board(tree->board, board);
    tree->turn = *turn;
    add_children(tree, TREE_DEPTH);

    // implement best action and print it
    do_action(board, tree->bestnextaction, turn);
    if(*turn%2 != 0) {
        // black's turn
        printf("*** BLACK ACTION #%d: ", *turn);
    } else {
        // white's turn
        printf("*** WHITE ACTION #%d: ", *turn);
    }
    print_action(tree->bestnextaction);

    // print updated board
    printf("\nBOARD COST: %d\n", cost(board));
    print_board(board);

    recursive_free_tree(tree);
    tree = NULL;
    return 1;
}

// DONE!!!!! pls give me H1 (if i deserve it) <3
/* algorithms are fun */
/* THE END -------------------------------------------------------------------*/