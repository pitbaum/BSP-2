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
  public:
    char playerSign; //'X' or 'O'
    string currentGameState; //String that includes positions of the game.
    vector<string> possibleMoves; //all possible next moves.
    string chosenMove;
    char opponentSign;

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

    //initializes the sign of the opponent dependant on the own Sign.
    void findOpSign() {
      if(playerSign == 'O')
        opponentSign = 'X';
      else if(playerSign == 'X')
        opponentSign = 'O';
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
    }

};

class StatisticAgent: public Agent {
  private:
    double ownEvaluation = 1;
    double opponentEvaluation = 2;
    double emptyFieldEvaluation = 0.5;
    int gridSize = 3;

  public:
    vector<double> fieldEvaluation;
    //makes the move that has the most open potential.
    void policy() {
      findOpSign();
      for (int i = 0; i < currentGameState.size(); ++i) {
        double currentEvaluation = 0;
        if(currentGameState[i] == ' ') {
          int iteration = 1;
          while(i - iteration >= 0 && int(i/gridSize) == int((i-iteration)/gridSize)) {
            if(currentGameState[i-1] == playerSign)
              currentEvaluation += ownEvaluation *iteration;
            else if(currentGameState[i-1] == opponentSign)
              currentEvaluation += opponentEvaluation * iteration;
            else if(currentGameState[i-1] == ' ')
              currentEvaluation += emptyFieldEvaluation * iteration;
            ++iteration;
          }

          iteration = 1;
          while(i+iteration < currentGameState.size() && int(i/gridSize) == int((i-iteration)/gridSize)) {
           if(currentGameState[i+1] == playerSign)
              currentEvaluation += ownEvaluation * iteration;
            else if(currentGameState[i+1] == opponentSign)
              currentEvaluation += opponentEvaluation * iteration;
            else if(currentGameState[i+1] == ' ')
              currentEvaluation += emptyFieldEvaluation * iteration;
            ++iteration;
          }

          iteration = 1;
          while(i - (gridSize * iteration) >= 0){
            if(currentGameState[i-gridSize] == playerSign)
              currentEvaluation += ownEvaluation * iteration;
            else if(currentGameState[i-gridSize] == opponentSign)
              currentEvaluation += opponentEvaluation * iteration;
            else if(currentGameState[i-gridSize] == ' ')
              currentEvaluation += emptyFieldEvaluation * iteration;
            ++iteration;
          }

          iteration = 1;
          while(i + (gridSize * iteration) < currentGameState.size()) {
            if(currentGameState[i+gridSize] == playerSign)
              currentEvaluation += ownEvaluation * iteration;
            else if(currentGameState[i+gridSize] == opponentSign)
              currentEvaluation += opponentEvaluation * iteration;
            else if(currentGameState[i+gridSize] == ' ')
              currentEvaluation += emptyFieldEvaluation * iteration;
            ++iteration;
          }
        }
        fieldEvaluation.push_back(currentEvaluation);
      }

      int chosenIndex = 0;
      for(int i = 0; i < fieldEvaluation.size(); i++) {
        if(fieldEvaluation[i] > fieldEvaluation[chosenIndex]){
          chosenIndex = i;
        }
      }

      string help = currentGameState;
      help[chosenIndex] = playerSign;
      chosenMove = help;
      fieldEvaluation.clear();
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

struct State {
    //number code that tells you the state of every Field.
    //The format is the same as in the gamestate and possiblemoves strings.
    string positions;
    //relative reward value distributed to the field.
    double reward;

    State(string pPos, double pReward) {
      positions = pPos;
      reward = pReward;
    }
};

class ReinforcementLearning: public Agent {
  private:
    vector<State> previousMoves; //list of moves taken in the game.
    double reward = 50; //if win = 100, if loose = 0, if draw = 50.
    vector<State> lookUpTable; //knowledge representation.
    int explorationRate = 10; //percentage of the moves used to explore. (max 100 min 0)

  public:
    ReinforcementLearning() {
      //reads from file the knowledge representation.
      //initializes at the start of the program the lookUpTable.
      string readText;
      fstream readFile("agent3x3.txt");
      cout << "opened txt file" << endl;
      int stateIndexer = 0;
      string currentPosition;
      double currentValue = 0;
      while (getline (readFile, readText)) {
        if(stateIndexer % 2 == 0){
          currentPosition = readText;
        }
        else {
          string currentValueString = readText;
          //convert string to double.
          currentValue = atof(currentValueString.c_str());
          struct State currentState = State(currentPosition, currentValue);
          lookUpTable.push_back(currentState);
        }
        stateIndexer++;
      }
      readFile.close();
    }

    //updates knowledge at the end of the round.
    void updateKnowledge() {
      //update current lookuptable.
      for (int i = previousMoves.size() - 1; i >= 0; --i)
      {
        bool isInside = false;
        for (int j = 0; j < lookUpTable.size(); ++j)
        {
          if(previousMoves[i].positions == lookUpTable[j].positions) {
            isInside = true;
            //if not the last move of the game.
            if(previousMoves.size() - 1 != i) {
              previousMoves[i].reward = lookUpTable[j].reward + 0.25 * (previousMoves[i+1].reward - lookUpTable[j].reward);
              lookUpTable[j].reward = previousMoves[i].reward;
            }
            else
              previousMoves[i].reward = lookUpTable[j].reward;
          }
        }

        //if the move was not in the table add it.
        if(isInside == false) {
          lookUpTable.push_back(previousMoves[i]);
          //Case of victory or defeat.
          if(previousMoves.size() - 1 == i) {
            lookUpTable[lookUpTable.size()-1].reward = reward;
            previousMoves[i].reward = lookUpTable[lookUpTable.size()-1].reward;
          }
          else {
            previousMoves[i].reward = previousMoves[i].reward + 0.25 * (previousMoves[i+1].reward - previousMoves[i].reward);
            lookUpTable[lookUpTable.size()-1].reward = previousMoves[i].reward;
          }
        }
      }

      //update txt file to save knowledge.
      int x = 0;
      ofstream textFile("agent3x3.txt");
      while(x < lookUpTable.size() * 2) {
        if(x % 2 == 0)
          textFile << lookUpTable[x/2].positions << endl;
        else{
          //cout << lookUpTable[x/2].reward << endl;
          //cout << "index = " << x/2 << endl;
          textFile << lookUpTable[x/2].reward << endl;
        }
        x++;
      }
      textFile.close();
    }

    void policy() {
      bool isExploration;
      double maxReward = -1;
      int maxIndex = -1;
      //exploitation moves.
      if(rand() % 100 > explorationRate)
      {
        for (int i = 0; i < possibleMoves.size(); ++i) {
          for(int j = 0; j < lookUpTable.size(); ++j) {
            if(lookUpTable[j].positions == possibleMoves[i]) {
              if(maxReward <= lookUpTable[j].reward) {
                maxReward = lookUpTable[j].reward;
                maxIndex = i;
              }
            }
          }
        }
      }
      //exploration moves.
      else {
        if(possibleMoves.size() > 1) {
          chosenMove = possibleMoves[rand() % (possibleMoves.size() - 1)];
        }
        else
          chosenMove = possibleMoves.front();
        }
      if(maxReward == -1 || maxIndex == -1) {
        if(possibleMoves.size() > 1)
        chosenMove = possibleMoves[rand() % (possibleMoves.size() - 1)];
        else
          chosenMove = possibleMoves.front();
      }
      else {
        chosenMove = possibleMoves[maxIndex];
      }

      int n = 0;
      bool isInside = false;
      while(n < lookUpTable.size())
      {
        if(chosenMove == lookUpTable[n].positions) {
          struct State chosenKnownState = lookUpTable[n];
          previousMoves.push_back(chosenKnownState);
          isInside = true;
        }
        n++;
      }
      if(isInside == false) {
        struct State chosenNewState = State(chosenMove, 0);
        previousMoves.push_back(chosenNewState);
      }
    }

    void setReward(double pReward) {
      reward = pReward;
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

void statPlayerInit(StatisticAgent &statPlayer, GameGrid &game1) {
  bool validInput = false;
  while(!validInput) {
    statPlayer.gameStateLoader(game1.status);
    statPlayer.policy();
    auto[col, row] = statPlayer.makeMove();
    validInput = game1.nextMove(statPlayer.getPlayerSign(), col, row);
  }
}

void aiPlayerInit(ReinforcementLearning &ai1, GameGrid &game1) {
  bool validInput = false;
  while(!validInput) {
    ai1.gameStateLoader(game1.status);
    ai1.moveLoader();
    ai1.policy();
    auto [col, row] = ai1.makeMove();
    validInput = game1.nextMove(ai1.getPlayerSign(), col, row);
  }
}

void aiPlayerEnd(ReinforcementLearning &ai1, char outcome) {
  if(outcome == 'w') {
    ai1.setReward(10);
  }
  else if(outcome == 'l') {
    ai1.setReward(0);
  }
  else if(outcome == 'd') {
    ai1.setReward(6);
  }
  ai1.updateKnowledge();
}

//main function to run tic tac toe.
int main() {
  int execution_rate = 0;
  int winrate = 0;
  int drawrate = 0;
  srand(time(0));
  while(execution_rate < 5000) {
    GameGrid game1;
    int currentRound = -1;
    char currentPlayer;
    bool isDraw = false;
    bool isWin = false;

    //random agent.
    RandomAgent rnPlayer;
    rnPlayer.setPlayerSign('O');

    //statistic Agent.
    StatisticAgent statPlayer;
    statPlayer.setPlayerSign('O');


    //human player.
    Userplayer human1;

    //reinforcement learning agent.
    ReinforcementLearning ai1;
    ai1.setPlayerSign('X');

    //game loop.
    while(!isWin && !isDraw)
    {
      currentRound++;
      if(currentRound % 2 == 0)
        currentPlayer = 'X';
      else
        currentPlayer = 'O';
      game1.display();
      //executes random player as 1st player.
      if(rnPlayer.getPlayerSign() == currentPlayer){
        cout << currentPlayer << " moved: " << endl;
          rnPLayerInit(rnPlayer, game1);
      }

      //executes second player as human.
      /*else {
        cout << "your move: " << endl;
        bool validInput = false;
        while(!validInput) {
          auto [col, row] = human1.input();
          validInput = game1.nextMove(currentPlayer, col, row);
        }
      } /**/

      //executes the second player as statPlayer.
      /*else {
        cout << currentPlayer << " moved: " << endl;
        statPlayerInit(statPlayer, game1);
      } /**/

      else {
        cout << currentPlayer << " moved: " << endl;
        aiPlayerInit(ai1, game1);
      }

      isWin = game1.hasWon(currentPlayer);
      isDraw = game1.isDraw();
    }
    if(currentPlayer == ai1.getPlayerSign()) {
      if(isWin == true) {
        aiPlayerEnd(ai1, 'w');
        winrate++;
      }
    }
    else if(currentPlayer != ai1.getPlayerSign()) {
      if(isWin == true) {
        aiPlayerEnd(ai1, 'l');
      }
    }
    if(isDraw == true && isWin != true) {
      aiPlayerEnd(ai1, 'd');
      drawrate++;
    }
    game1.display();
    cout << "End of game." << endl;
    execution_rate++;
  }
  cout << "winrate: " << winrate << endl;
  cout << "draws: " << drawrate << endl;
  return 0;
}