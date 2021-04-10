#include <iostream>
using namespace std;

class GameGrid {
  private:
    int width = 3;  //width of the playfield.
    int height = 3; //height of the playfield.
    int goal = 3; //how many to connect, to win.
    char status[3][3];  //2D array with the saved status of the fields.

  public:
    //initializes all the fields on the grid at the start to be empty.
    GameGrid() {
      for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
          status[i][j] = ' ';
        }
      }
    }

    //displays the playfield.
    void display() {
      for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
          cout << status[i][j] << "|";
          if(j == 2)
            cout << endl;
        }
      }
    }

    //receives and executes the users next move.
    void nextMove(char currentPlayer) {
      string request = "Enter the coloumn and row to put your mark on: ";
      int col, row;
      //as long as the values are out of bounds, reask input.
      bool isValid = false;
      while(!isValid) {
        //input the column and row.
        cout << request;
        cin >> col >> row;
        if(col > 0 && col <= 3 && row > 0 && row <= 3) {
          if(status[row-1][col-1] == ' ')
            isValid = true;
          else
            cout << "This space isn't free." << endl;
        }
        else
          cout << "The input needs to be within the bounds of the field." << endl;
      }
      //set the mark on the field to the one of the current player.
        status[row-1][col-1] = currentPlayer;
    }

    //checks if the game was already won by a user.
    bool hasWon(char currentPlayer) {
      if(horizontal(currentPlayer)){
        cout << "horizontal win \n";
        return true;
      }
      if(vertical(currentPlayer)){
        cout << "vertical win \n";
        return true;
      }
      if(diagonal(currentPlayer, false)){
        cout << "diagonal down win \n";
        return true;
      }
      if(diagonal(currentPlayer, true)){
        cout << "diagonal up win \n";
        return true;
      }
      return false;
    }

    bool isDraw() {
      for (int i = 0; i < width; ++i)
      {
        for (int j = 0; j < height; ++j)
        {
          if(status[i][j] == ' ')
            return false;
        }
      }
      return true;
    }

  private:
    bool horizontal(char currentPlayer) {
      for(int i = 0; i < width; ++i)
      {
        for(int j = 0; j < height; ++j)
        {
          int alignedCount = 0;
          if(status[i][j] == currentPlayer)
            ++alignedCount;
          else
            alignedCount = 0;
          if(alignedCount >= goal)
              return true;
        }
      }
      return false;
    }

    bool vertical(char currentPlayer) {
      for(int i = 0; i < height; ++i)
      {
        for(int j = 0; j < width; ++j)
        {
          int alignedCount = 0;
          if(status[j][i] == currentPlayer)
            ++alignedCount;
          else
            alignedCount = 0;
          if(alignedCount >= goal)
            return true;
        }
      }
      return false;
    }

    bool diagonal(char currentPlayer, bool isUp) {
      for(int i = 0; i < width; ++i)
      {
        for(int j = 0; j < height; ++j)
        {
          if(status[i][j] == currentPlayer)
          {
            int alignedCount = 1;
            int x = i, y = j;
            while(x < width && y < height)
            {
              ++x;
              if(isUp) //if it is used for upwards diagonals
                --y;
              else  //if it is used for downwards diagonals
                ++y;
              if(status[x][y] == currentPlayer)
                ++alignedCount;
              else
                alignedCount = 0;
              if(alignedCount >= goal)
                return true;
            }
          }
        }
      }
      return false;
    }
};

int main() {
  GameGrid game1;
  int currentRound = -1;
  char currentPlayer;
  //game loop.
  while(!game1.hasWon(currentPlayer) && !game1.isDraw())
  {
    currentRound++;
    if(currentRound % 2 == 0)
      currentPlayer = 'X';
    else
      currentPlayer = 'O';
    game1.display();
    game1.nextMove(currentPlayer);
  }
  game1.display();
  cout << "End of game.";
  return 0;
}

class Agent1 {
  private:
    int gridSize = 9; // the size of the play field.
    char grid[9];
    State listOfCurrentStates[9];
    int reward; //if win = 1, if loose = -1, if draw = 0.
    State listOfStates[255168]; //look up table.
    int explorationRate = 25 //percentage of the moves used to explore. (max 100 min 0)

  public:
    Agent1()
    {
      //for (int i = 0; i < 255168; ++i)
       //{
         //listOfStates[i] = fileRead;
       //}
    }

    int makeMove() //returns the chosen move.
    {

    }

    void Policy()//chooses the move to execute.
    {

    }

    void rewardUpdate(int pEndGameStatus) //distibutes the reward link to the different states to the look up table.
    {
      for (int i = 0; i < listOfCurrentStates.size(); --i)
      {
        for (int j = 0; j < listOfStates.size(); ++j)
        {
          if(listOfCurrentStates[i].positions == listOfStates[j])
          {
            listOfStates[j].reward = listOfCurrentStates[i].reward + 0.2(listOfCurrentStates[i+1].reward - listOfCurrentStates[i].reward);
          }
          if(i == listOfCurrentStates.size())
          {
            if(listOfCurrentStates[i].positions == listOfStates[j])
              listOfStates[j].reward = reward;
          }
        }
      }
    }

    void updateGrid(char pGrid[3][3]) //updates the game grid of the player and the actual game.
    {
      int index = 0;
      for (int i = 0; i < 9; ++i)
      {
        for (int j = 0; j < count; ++j)
       {
          grid[index] = pGrid[i][j];
          index++;
        }
      }

    }
};

class State() {
  public:
    int positions;
    int reward = 0;

    State(int pPositions, int pReward)
    {
      positions = pPositions;
      reward = pReward;
    }
};

