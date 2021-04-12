#include <iostream>
#include <fstream>
#include <ctime>
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
  srand(time(0));
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

//Agent that uses reinforcement learning.
class Agent1 {
  private:
    string currentGameState;  //string on the current game state.
    State listOfCurrentStates[9]; //list of moves taken in the game.
    int reward; //if win = 1, if loose = -1, if draw = 0.
    State listOfStates[255168]; //look up table.
    int explorationRate = 75; //percentage of the moves used to explore. (max 100 min 0)
    bool isPlayer1;  //true if agent is player 1, false if agent is player 2.

  public:
    Agent1(bool pIsPlayer1)
    {
      //reads from file the knowledge representation.
      //initializes at the start of the program the listOfStates.
      string readText;
      ifstream readFile("agent3x3.txt");
      int stateIndexer = 0;
      int currentPosition = 0;
      int currentValue = 0;
      while (getline (readFile, readText)) {
        if(stateIndexer%2 == 0)
          currentPosition = (int) readText;
        else {
          currentValue = (int) readText;
          listOfStates[(stateIndexer - 1)/2] = new State(currentPosition, currentValue);
        }
        stateIndexer++;
      }
      readFile.close();
      isPlayer1 = pIsPlayer1;
    }

    int makeMove() //returns the chosen move.
    {

    }

    string Policy()//chooses and returns the move to execute.
    {
      //creates an array possibleMoves[].
      //Which consist of all the moves possible to make at the current game state.
      int moveCounter = 0;
      for(char& c : currentGameState) {
        if(c == " ") {
          moveCounter++;
        }
      }
      string possibleMoves[moveCounter];
      for(int i = 0; i < currentGameState.size(); i++)
      {
        if(currentGameState[i] == " ")
        {
          string move = currentGameState;
          if(isPlayer1)
            move[i] = "X";
          else
            move[i] = "O";
          possibleMoves[moveCounter] = move;
          moveCounter--;
        }
      }

      //finds the possible moves in the look up table and searches for the state with the highest value.
      //if a possible state is not yet in the table, the state will be added.
      int highestIndex;
      int highestValue = -2;
      for(int j = 0; j < moveCounter; j++)
      {
        bool foundInTable = false;
       for(int listOfStatesIterator = 0; listOfStatesIterator < listOfStates.size(); listOfStatesIterator++)
       {
          if(possibleMoves[j] == listOfStates[listOfStatesIterator].positions)
          {
            foundInTable = true;
            if(listOfStates[listOfStatesIterator].reward > highestValue)
              highestIndex = listOfStatesIterator;
          }
          if(foundInTable)
            break;
          if(listOfStatesIterator == listOfStates.size()-1)
          {
            listOfStates[listOfStatesIterator] = new State(possibleMoves[j], 0);
          }
        }
      }

      //If the random exploration rate is triggered.
      //Choose a random move.
      //Else return best valued move.
      if((rand() % 100) > explorationRate)
        return possibleMoves(rand() % moveCounter)
      else
        return listOfStates[highestIndex].positions;
    }

    string gridConverterTo1D(char pGrid[3][3]) //converts the entered 2D into 1D string.
    {
      char position[9];
      int currentPosition = 0;
      for (int i = 0; i < 3; ++i)
      {
        for (int j = 0; j < 3; ++j)
        {
          position[currentPosition] = pGrid[i][j];
          currentPosition++;
        }
      }
      string grid;
      for (int i = 0; i < sizeof(position); ++i)
      {
        grid += position[i];
      }
      return grid;
    }

    //distibutes the reward link to the different states to the look up table.
    void rewardUpdate(int pEndGameStatus)
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
};

//knowledge representation of the Agent1
class State() {
  public:
    //number code that tells you the state of every Field.
    //" " = empty field, X field of status player 1, O (letter) field of status player 2.
    string positions;
    //relative reward value distributed to the field.
    int reward = 0;

    State(int pPositions, int pReward)
    {
      positions = pPositions;
      reward = pReward;
    }
};

