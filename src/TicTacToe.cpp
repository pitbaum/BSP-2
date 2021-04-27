#include <iostream>
#include <fstream>
#include <ctime>
#include <tuple>
#include <vector>
#include <sstream>
using namespace std;

//A Game of tic tac toe.
class GameGrid {
  private:
    int width = 3;  //width of the playfield.
    int height = 3; //height of the playfield.
    int goal = 3; //how many to connect, to win.

  public:
    char status[3][3];  //2D array with the saved status of the fields.

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
    bool nextMove(char currentPlayer, int col, int row) {
      if(col > 0 && col <= 3 && row > 0 && row <= 3) {
        if(status[row-1][col-1] == ' ') {
          //set the mark on the field to the one of the current player.
          status[row-1][col-1] = currentPlayer;
          return true;
        }
        else {
          cout << "This space isn't free." << endl;
          return false;
        }
      }
      else {
        cout << "The input needs to be within the bounds of the field." << endl;
        return false;
      }
      cout << "unexpected input check." << endl;
      return false;
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
      if(diagonalDown(currentPlayer)){
        cout << "diagonal down win \n";
        return true;
      }
      if(diagonalUp(currentPlayer)){
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
        int alignedCount = 0;
        for(int j = 0; j < height; ++j)
        {
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
        int alignedCount = 0;
        for(int j = 0; j < width; ++j)
        {
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

    bool diagonalDown(char currentPlayer) {
      for(int i = 0; i < width; ++i)
      {
        for(int j = 0; j < height; ++j)
        {
          if(status[i][j] == currentPlayer)
          {
            int alignedCount = 0;
            int x = 0, y = 0;
            while((i+x) < width && (i+y) < height) {
              if(status[i+x][j+y] == currentPlayer)
               ++alignedCount;
              else
                alignedCount = 0;
              if(alignedCount >= goal)
                return true;
              ++x;
              ++y;
            }
          }
        }
      }
      return false;
    }

    bool diagonalUp(char currentPlayer) {
      for(int i = 0; i < width; ++i)
      {
        for(int j = 0; j < height; ++j)
        {
          int alignedCount = 0;
          if(status[i][j] == currentPlayer)
          {
            int x = 0, y = 0;
            cout << "current coords" <<  j << i << endl;
            while((i+x) < width && (j-y) >= 0) {
              if(status[i+x][j-y] != currentPlayer) {
               alignedCount = 0;
              }
              else {
                ++alignedCount;
              }
              if(alignedCount >= goal) {
                return true;
              }
              ++x;
              ++y;
              cout << "counted: " << alignedCount << " " << currentPlayer << endl;
            }
            alignedCount = 0;
          }
        }
      }
      return false;
    }
};

//parent class for all agents.
class Agent {
  private:
    string currentGameState; //String that includes positions of the game.
    char playerSign; //'X' or 'O'

  public:
    vector<string> possibleMoves; //all possible next moves.
    string chosenMove;
    //updates the currentGameState.
    void gameStateLoader(char pGrid[3][3]) {
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
      currentGameState = grid;
    }

    //updates the list of possible moves.
    void moveLoader() {
      possibleMoves.clear(); //empty the vector before updating.
      for(int i = 0; i < currentGameState.size(); i++) {
        if(currentGameState[i] == ' ') {
          string moveState = currentGameState;
          moveState[i] = playerSign;
          possibleMoves.push_back(moveState);
          cout << moveState << endl;
        }
      }
    }

    void setPlayerSign(char pPlayerSign) {
      playerSign = pPlayerSign;
    }

    char getPlayerSign() {
      return playerSign;
    }

    //returns the col and row for the game to set the mark on.
    tuple<int, int> makeMove() {
      cout << "current Game state = " << currentGameState << "; chosenMove = " << chosenMove << endl;
      for (int i = 0; i < currentGameState.size(); ++i)
      {
        if(currentGameState[i] != chosenMove[i]) {
          //the game expects values between 1 and maxgridsize.
          int row = (i / 3) + 1;
          int col = (i % 3) + 1;
          return {col, row};
        }
      }
      cout << "makeMove error, tuple return Null ";
      return {4, 4};
    }
};

class RandomAgent: public Agent {
  public:
    //chooses a move randomly from the move list.
    void policy() {
      if(possibleMoves.size() > 1) {
        chosenMove = possibleMoves[rand() % (possibleMoves.size() - 1)];
      }
      else
        chosenMove = possibleMoves.front();
      cout<< chosenMove;
    }

};

class StatisticAgent: public Agent {
  public:
    //makes the move that has the most open potential to win.
    void policy() {

    }
};

class Userplayer {
public:
  tuple<int, int> input() {
    string request = "Enter the coloumn and row to put your mark on: ";
    int col, row;
    //input the column and row.
    cout << request;
    cin >> col >> row;
    return {col, row};
  }
};

class State {
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

class ReinforcementLearning: public Agent {
  private:
    vector<State> previousMoves; //list of moves taken in the game.
    int reward; //if win = 1, if loose = -1, if draw = 0.
    vector<State> lookUpTable; //knowledge representation.
    int explorationRate = 75; //percentage of the moves used to explore. (max 100 min 0)

  public:
    ReinforcementLearning() {
      //reads from file the knowledge representation.
      //initializes at the start of the program the lookUpTable.
      string readText;
      ifstream readFile("agent3x3.txt");
      int stateIndexer = 0;
      int currentPosition = 0;
      int currentValue = 0;
      stringstream ss;
      while (getline (readFile, readText)) {
        if(stateIndexer%2 == 0){
          string currentPositionString = readText;
          //convert string to int.
          ss << currentPositionString;
          ss >> currentPosition;
        }
        else {
          string currentValueString = readText;
          //convert string to int.
          ss << currentValueString;
          ss >> currentValue;
          State currentState(currentPosition, currentValue);
          lookUpTable.push_back(currentState);
        }
        stateIndexer++;
      }
      readFile.close();
    }

};

//random Agent executer and initializer.
void rnPLayerInit(RandomAgent &rnPlayer, GameGrid &game1) {
  bool validInput = false;
  while(!validInput) {
    rnPlayer.gameStateLoader(game1.status);
    rnPlayer.moveLoader();
    rnPlayer.policy();
    auto [col, row] = rnPlayer.makeMove();
    validInput = game1.nextMove(rnPlayer.getPlayerSign(), col, row);
  }
}

//main function to run tic tac toe.
int main() {
  srand(time(0));
  GameGrid game1;
  int currentRound = -1;
  char currentPlayer;

  //random agent.
  RandomAgent rnPlayer;
  rnPlayer.setPlayerSign('X');


  //human player
  Userplayer human1;

  //game loop.
  while(!game1.hasWon(currentPlayer) && !game1.isDraw())
  {
    currentRound++;
    if(currentRound % 2 == 0)
      currentPlayer = 'X';
    else
      currentPlayer = 'O';
    game1.display();
    //executes random player as 1st player.
    if(rnPlayer.getPlayerSign() == currentPlayer)
        rnPLayerInit(rnPlayer, game1);
    //executes second player as human.
    else {
      bool validInput = false;
      while(!validInput) {
        auto [col, row] = human1.input();
        validInput = game1.nextMove(currentPlayer, col, row);
      }
    }
  }
  game1.display();
  cout << "End of game.";
  return 0;
}