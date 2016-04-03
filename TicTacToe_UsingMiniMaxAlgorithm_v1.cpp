#include<iostream>
#include<stdio.h>
#include<map>
#include<vector>
#include<algorithm>
#include<string>
#include<ctime>
using namespace std;


char maxPlayer = 'x';
char minPlayer = 'o';

char getNext(char turn)
{
    if(turn == 'x')
        return 'o';
    return 'x';
}

class Action
{
    public:
    int row,col;
    char move;
    Action(){};
    Action(int row,int col,char move)
    {
        this->row = row;
        this->col = col;
        this->move = move;
    }
    void printAction()
    {
        printf(" %d %d %c ",row,col,move);
    }
};

class State
{
    public:
    vector<vector<char> > board;
    char turn;

    State(){}
    State(char turn)
    {
        vector<char> r1;
        board.push_back(r1);
        vector<char> r2;
        board.push_back(r2);
        vector<char> r3;
        board.push_back(r3);

        for(int i=0; i<3; i++)
            for(int j=0; j<3; j++)
                board[i].push_back(' ');
        this->turn = turn;
    }

    State(char turn,vector<vector<char> > board)
    {
        this->turn = turn;
        this->board = board;
    }

    void printBoard()
    {
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<3; j++)
                cout<<board[i][j]<<" ";
            cout<<endl;
        }
        printf("\n");
    }

    vector<Action> getPossibleActions()
    {
        vector<Action> actions;
        for(int i=0; i<3; i++)
            for(int j=0; j<3; j++)
                if(board[i][j] == ' ')
                    actions.push_back(Action(i,j,turn));
        return actions;
    }

    State getNextState(Action action)
    {
        vector<vector<char> > tempBoard = board;
        tempBoard[action.row][action.col] = action.move;
        return State(getNext(action.move),tempBoard);
    }

    bool isWinning(char c)
    {
        //row victory
        for(int i=0; i<3; i++)
        {
            if(board[i][0] == c && board[i][1] == c && board[i][2] == c)
                return true;
        }
        //column victory
        for(int j=0; j<3; j++)
        {
            if(board[0][j] == c && board[1][j] == c && board[2][j] == c)
                return true;
        }
        //diagonal victory
        if(board[0][0] == c && board[1][1] == c && board[2][2] == c)
            return true;
        if(board[0][2] == c && board[1][1] == c && board[2][0] == c)
            return true;
        return false;
    }

    bool isDraw()
    {
        for(int i=0; i<3; i++)
            for(int j=0; j<3; j++)
                if(board[i][j] == ' ')
                    return false;
        return true;
    }

    int getStateUtility()
    {
        //game has ended
        if(isWinning(maxPlayer))
            return 10;
        else if(isWinning(minPlayer))
            return -10;
        else if(isDraw())
            return 0;

        //game has not ended yet
        int q;
        if(turn == maxPlayer)
            q = 1;
        else q = -1;
        int maxUtility = -1000;
        vector<Action> possibleActions = getPossibleActions();
        for(int i=0,l=possibleActions.size(); i<l; i++)
        {
            State nextState = getNextState(possibleActions[i]);
            int stateUtility = nextState.getStateUtility();
            if(q*stateUtility > maxUtility)
                maxUtility = q*stateUtility;
        }
        return q*maxUtility;
    }

    Action getOptimalAction()
    {
        vector<Action> possibleActions = getPossibleActions();
        int maxUtility = -1000;
        int index = -1;
        for(int i=0,l=possibleActions.size(); i<l; i++)
        {
            State nextState = getNextState(possibleActions[i]);
            int stateUtility = nextState.getStateUtility();
            if(stateUtility > maxUtility)
            {
                maxUtility = stateUtility;
                index = i;
                if(stateUtility == 10)
                    break;
            }
        }
        return possibleActions[index];
    }

    bool gameOver()
    {
        if(isWinning(maxPlayer))
        {
            printf("Player %c Wins!\n",maxPlayer);
            return true;
        }
        else if(isWinning(minPlayer))
        {
            printf("Player %c Wins!\n",minPlayer);
            return true;
        }
        else if(isDraw())
        {
            printf("It's A Draw!\n");
            return true;
        }
        return false;
    }

    bool isValidMove(int r,int c)
    {
        return board[r][c] == ' ';
    }

};



int main()
{

    State gameBoard(minPlayer);

    while(1)
    {
        cout<<"Current State: \n";
        gameBoard.printBoard();

        if(gameBoard.gameOver())
            break;

        printf("\nPlayer %c's Turn...\n",gameBoard.turn);

        if(gameBoard.turn == minPlayer)
        {
            int r,c;
            do
            {
                printf("\nEnter legal row and column: ");
                scanf("%d %d",&r,&c);
            }while(!gameBoard.isValidMove(r,c));
            gameBoard = gameBoard.getNextState(Action(r,c,gameBoard.turn));
        }
        else
        {
            clock_t begin = clock();
            gameBoard = gameBoard.getNextState(gameBoard.getOptimalAction());
            clock_t end = clock();
            double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
            printf("Time taken: %f sec\n",elapsed_secs);
        }
    }


    return 0;
}
