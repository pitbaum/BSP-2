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
            while((i+x) < width && (j+y) < height) {
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