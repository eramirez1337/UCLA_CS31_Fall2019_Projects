// vampires.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the utterly trivial
// functions (marked TRIVIAL).  Then get Arena::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;                // max number of rows in the arena
const int MAXCOLS = 20;                // max number of columns in the arena
const int MAXVAMPIRES = 100;           // max number of vampires allowed

const int NORTH = 0;
const int EAST  = 1;
const int SOUTH = 2;
const int WEST  = 3;
const int NUMDIRS = 4;

const int EMPTY      = 0;
const int HAS_POISON = 1;

const int VAMP_DEAD = 0;            //Vampire has died after eating two poison vials
const int VAMP_POISONED = 1;        //Vampire has drunk a poison blood vial
const int VAMP_FULL_HEALTH = 2;     //Vampire vants to zuck your bluud oooooooo!!!!

const int VAMP_TRIED_TO_MOVE_LAST_TURN = 0;
const int VAMP_DID_MOVE_LAST_TURN = 1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
              // type name, since it's mentioned in the Vampire declaration.

class Vampire
{
  public:
      // Constructor
    Vampire(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    void move();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_vampHealth;
    int    m_movementLastTurn;
};

class Player
{
  public:
      // Constructor
    Player(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    string dropPoisonVial();
    string move(int dir);
    void   setDead();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};

class Arena
{
  public:
      // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();

      // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     vampireCount() const;
    int     getCellStatus(int r, int c) const;
    int     numberOfVampiresAt(int r, int c) const;
    void    display(string msg) const;

      // Mutators
    void setCellStatus(int r, int c, int status);
    bool addVampire(int r, int c);
    bool addPlayer(int r, int c);
    void moveVampires();

  private:
    int      m_grid[MAXROWS][MAXCOLS];
    int      m_rows;
    int      m_cols;
    Player*  m_player;
    Vampire* m_vampires[MAXVAMPIRES];
    int      m_nVampires;
    int      m_turns;

      // Helper functions
    void checkPos(int r, int c, string functionName) const;
    bool isPosInBounds(int r, int c) const;
};

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nVampires);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    string takePlayerTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int lowest, int highest);
bool decodeDirection(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();

bool checkMove(const Arena& a, int dir, int r, int c); //Helper non-class functions
int checkVamps(const Arena& a, int dir, int r, int c); //Helper non-class functions

///////////////////////////////////////////////////////////////////////////
//  Vampire implementation
///////////////////////////////////////////////////////////////////////////

Vampire::Vampire(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** A vampire must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Vampire created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_vampHealth = VAMP_FULL_HEALTH;
    m_movementLastTurn = VAMP_DID_MOVE_LAST_TURN;
}

int Vampire::row() const
{
    return m_row;
}

int Vampire::col() const
{
    return m_col;
}

bool Vampire::isDead() const
{
    if (m_vampHealth == VAMP_DEAD) {
        return true;
    }
    return false;
}

void Vampire::move()
{
    int dir = randInt(NORTH, WEST); //Move in a random direction. The value of North is the lowest and the value of West is the highest, which is what the function declaration wants
    if (m_vampHealth == VAMP_FULL_HEALTH)
    {
        attemptMove(*m_arena, dir, m_row, m_col);
    }
    else if (m_vampHealth == VAMP_POISONED)
    {
        if (m_movementLastTurn == VAMP_TRIED_TO_MOVE_LAST_TURN)
        {
            attemptMove(*m_arena, dir, m_row, m_col);
            m_movementLastTurn = VAMP_DID_MOVE_LAST_TURN;
        }
        else if (m_movementLastTurn == VAMP_DID_MOVE_LAST_TURN)
        {
            m_movementLastTurn = VAMP_TRIED_TO_MOVE_LAST_TURN;
        }
    }
    
    if (m_arena -> getCellStatus(m_row, m_col) == HAS_POISON) { //This will detect if the vampire ran into a coordinate with poison
        if (m_vampHealth == VAMP_FULL_HEALTH) {
            m_movementLastTurn = VAMP_DID_MOVE_LAST_TURN; //If the vampire had full health, its next move will be nullified
        }
        m_vampHealth--; //Decrease its health
        m_arena -> setCellStatus(m_row, m_col, EMPTY); //Since the vampire ate the poison, there shouldn't be a vial anymore
    }
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

string Player::dropPoisonVial()
{
    if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
        return "There's already a poisoned blood vial at this spot.";
    m_arena->setCellStatus(m_row, m_col, HAS_POISON);
    return "A poisoned blood vial has been dropped.";
}

string Player::move(int dir)
{
    if (!attemptMove(*m_arena, dir, m_row, m_col)) { //If a move wasn't attempted, then the player stands
        return "Player couldn't move; player stands.";
    }
    else if (m_arena -> numberOfVampiresAt(m_row, m_col) > 0)
    {
        setDead(); //If you ran into any number of vampires on that coordinate, you died
        return "Player walked into a vampire and died.";
    }
    else
    {
        switch (dir) {
            case NORTH:
                return "Player moved north.";
                break;
            case EAST:
                return "Player moved east.";
                break;
            case SOUTH:
                return "Player moved south.";
                break;
            case WEST:
                return "Player moved west.";
            default:
                break; //If the input is none of these directions, it will break from the switch statement and go to the return statement at the bottom
        }
    }
    return "Player couldn't move; player stands.";
}

bool Player::isDead() const
{
    if (m_dead == true) {
        return true;
    }
    return false;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nVampires = 0;
    m_turns = 0;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
    delete m_player; //Delete the player pointer
    for (int k = 0; k < vampireCount(); k++) {
        delete m_vampires[k]; //Delete the vampire pointers
    }
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::vampireCount() const
{
    return m_nVampires;
}

int Arena::getCellStatus(int r, int c) const
{
    checkPos(r, c, "Arena::getCellStatus");
    return m_grid[r-1][c-1];
}

int Arena::numberOfVampiresAt(int r, int c) const
{
    int count = 0;
    for (int k = 0; k < vampireCount(); k++)
    {
        if (m_vampires[k]->row() == r && m_vampires[k]->col() == c && !m_vampires[k]->isDead()) //Checks each vamp to see if they are at the position
        {
            count++;
        }
    }
    return count;
}

void Arena::display(string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
      // Fill displayGrid with dots (empty) and stars (poisoned blood vials)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r-1][c-1] = (getCellStatus(r,c) == EMPTY ? '.' : '*');

      // Indicate each vampire's position
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
        {
            if (numberOfVampiresAt(r, c) != 0)
            {
                switch (numberOfVampiresAt(r, c))
                {
                    case 1:
                        displayGrid[r-1][c-1] = 'V';
                        break;
                    case 2:
                        displayGrid[r-1][c-1] = '2';
                        break;
                    case 3:
                        displayGrid[r-1][c-1] = '3';
                        break;
                    case 4:
                        displayGrid[r-1][c-1] = '4';
                        break;
                    case 5:
                        displayGrid[r-1][c-1] = '5';
                        break;
                    case 6:
                        displayGrid[r-1][c-1] = '6';
                        break;
                    case 7:
                        displayGrid[r-1][c-1] = '7';
                        break;
                    case 8:
                        displayGrid[r-1][c-1] = '8';
                        break;
                    default:
                        displayGrid[r-1][c-1] = '9';
                        break;
                }
            }
        }
    }

      // Indicate player's position
    if (m_player != nullptr)
        displayGrid[m_player->row()-1][m_player->col()-1] = (m_player->isDead() ? 'X' : '@');

      // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
            cout << displayGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;

      // Write message, vampire, and player info
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << vampireCount() << " vampires remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player!" << endl;
    else if (m_player->isDead())
        cout << "The player is dead." << endl;
    cout << m_turns << " turns have been taken." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
    checkPos(r, c, "Arena::setCellStatus");
    m_grid[r-1][c-1] = status;
}

bool Arena::addVampire(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;

      // Don't add a vampire on a spot with a poisoned blood vial
    if (getCellStatus(r, c) != EMPTY)
        return false;

      // Don't add a vampire on a spot with a player
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;

    //Can't have more than the max amount of vampires on the grid
    if (vampireCount() == MAXVAMPIRES) {
        return false;
    }
    
    //Dynamically allocating new vampires
    m_vampires[vampireCount()] = new Vampire(this, r, c);
    m_nVampires++;
    return true;
}

bool Arena::addPlayer(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;

      // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

      // Don't add a player on a spot with a vampire
    if (numberOfVampiresAt(r, c) > 0)
        return false;

    m_player = new Player(this, r, c);
    return true;
}

void Arena::moveVampires()
{
    // Move all vampires
    for (int k = 0; k < vampireCount(); k++) {
        m_vampires[k] -> move();
    }
    //If the vampires move onto the player, set the player dead
    if (numberOfVampiresAt(m_player -> row(), m_player -> col()) > 0) {
        m_player -> setDead();
    }
    
    //Count the number of vampires for another use
    const int count = m_nVampires;

    //Check to make sure if a vampire is dead or not. If it is, set a pointer to it and delete that vampire
    int temp = vampireCount() - 1;
    for (int i = 0; i < vampireCount(); i++)
    {
        Vampire* holder = nullptr;
        if (m_vampires[i]->isDead())
        {
            holder = m_vampires[i];
            m_vampires[i] = m_vampires[temp];
            m_vampires[temp] = holder;
            holder = nullptr;
            temp--;
            m_nVampires--;
            i--;
        }
    }
    
    
    
    
    
    // Deletes all nullptrs
    for (int j = count - 1; j > m_nVampires - 1; j--)
        delete m_vampires[j];
      // Another turn has been taken
    m_turns++;
}

bool Arena::isPosInBounds(int r, int c) const
{
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
}

void Arena::checkPos(int r, int c, string functionName) const
{
    if (r < 1  ||  r > m_rows  ||  c < 1  ||  c > m_cols)
    {
        cout << "***** " << "Invalid arena position (" << r << ","
             << c << ") in call to " << functionName << endl;
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nVampires)
{
    if (nVampires < 0)
    {
        cout << "***** Cannot create Game with negative number of vampires!" << endl;
        exit(1);
    }
    if (nVampires > MAXVAMPIRES)
    {
        cout << "***** Trying to create Game with " << nVampires
             << " vampires; only " << MAXVAMPIRES << " are allowed!" << endl;
        exit(1);
    }
    int nEmpty = rows * cols - nVampires - 1;  // 1 for Player
    if (nEmpty < 0)
    {
        cout << "***** Game created with a " << rows << " by "
             << cols << " arena, which is too small too hold a player and "
             << nVampires << " vampires!" << endl;
        exit(1);
    }

      // Create arena
    m_arena = new Arena(rows, cols);

      // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
    m_arena->addPlayer(rPlayer, cPlayer);

      // Populate with vampires
    while (nVampires > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (r == rPlayer && c == cPlayer)
            continue;
        m_arena->addVampire(r, c);
        nVampires--;
    }
}

Game::~Game()
{
    delete m_arena;
}

string Game::takePlayerTurn()
{
    for (;;)
    {
        cout << "Your move (n/e/s/w/x or nothing): ";
        string playerMove;
        getline(cin, playerMove);

        Player* player = m_arena->player();
        int dir;

        if (playerMove.size() == 0)
        {
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
                return player->move(dir);
            else
                return player->dropPoisonVial();
        }
        else if (playerMove.size() == 1)
        {
            if (tolower(playerMove[0]) == 'x')
                return player->dropPoisonVial();
            else if (decodeDirection(playerMove[0], dir))
                return player->move(dir);
        }
        cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
    }
}

void Game::play()
{
    m_arena->display("");
    Player* player = m_arena->player();
    if (player == nullptr)
        return;
    while ( ! player->isDead()  &&  m_arena->vampireCount() > 0)
    {
        string msg = takePlayerTurn();
        m_arena->display(msg);
        if (player->isDead())
            break;
        m_arena->moveVampires();
        m_arena->display(msg);
    }
    if (player->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

  // Return a uniformly distributed random int from lowest to highest, inclusive
int randInt(int lowest, int highest)
{
    if (highest < lowest)
        swap(highest, lowest);
    static random_device rd;
    static default_random_engine generator(rd());
    uniform_int_distribution<> distro(lowest, highest);
    return distro(generator);
}

bool decodeDirection(char ch, int& dir)
{
    switch (tolower(ch))
    {
      default:  return false;
      case 'n': dir = NORTH; break;
      case 'e': dir = EAST;  break;
      case 's': dir = SOUTH; break;
      case 'w': dir = WEST;  break;
    }
    return true;
}

  // Return false without changing anything if moving one step from (r,c)
  // in the indicated direction would run off the edge of the arena.
  // Otherwise, update r and c to the position resulting from the move and
  // return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
    if (dir == NORTH) //Moving up means a decrease in row numbers
          {
              r--;
              if(!(r >= 1  &&  r <= a.rows()  &&  c >= 1  &&  c <= a.cols())) //Make sure a movement isn't out of bounds
              {
                  r++;
                  return false;
              }
              return true;
          }
        else if (dir == EAST) //Moving east means an increase in col numbers
          {
              c++;
              if(!(r >= 1  &&  r <= a.rows()  &&  c >= 1  &&  c <= a.cols()))
              {
                  c--;
                  return false;
              }
              return true;
          }
        else if (dir == WEST) //Moving west means a decrease in col numbers
          {
              c--;
              if(!(r >= 1  &&  r <= a.rows()  &&  c >= 1  &&  c <= a.cols()))
              {
                  c++;
                  return false;
              }
              return true;
          }
        else if (dir == SOUTH) //Moving south means an increase in row numbers
          {
              r++;
              if(!(r >= 1  &&  r <= a.rows()  &&  c >= 1  &&  c <= a.cols()))
              {
                  r--;
                  return false;
              }
              return true;
          }
        return false;
}

//Remember those helper non-class functions? Well, here they are. They will assist with recommend move
bool checkMove(const Arena& a, int dir, int r, int c) //Essentially the same as attempt move
{
    if (dir == NORTH)
    {
        r--;
        if(!(r >= 1  &&  r <= a.rows()  &&  c >= 1  &&  c <= a.cols()))
        {
            r++;
            return false;
        }
        return true;
        
    }
    else if (dir == EAST)
    {
        c++;
        if(!(r >= 1  &&  r <= a.rows()  &&  c >= 1  &&  c <= a.cols()))
        {
            c--;
            return false;
        }
        return true;
    }
    else if (dir == WEST)
    {
        c--;
        if(!(r >= 1  &&  r <= a.rows()  &&  c >= 1  &&  c <= a.cols()))
        {
            c++;
            return false;
        }
        return true;
    }
    else if (dir == SOUTH)
    {
        r++;
        if(!(r >= 1  &&  r <= a.rows()  &&  c >= 1  &&  c <= a.cols()))
        {
            r--;
            return false;
        }
        return true;
    }
    return false;
}

//Another helper function
int checkVamps(const Arena& a, int dir, int r, int c) //Checks the number of vampires around the player movements
{
    int vampCounter = MAXVAMPIRES;
    if (dir == NORTH)
    {
        r--;
        vampCounter = a.numberOfVampiresAt(r - 1, c) + a.numberOfVampiresAt(r, c + 1) + a.numberOfVampiresAt(r, c - 1);
    }
    else if (dir == EAST)
    {
        c++;
        vampCounter = a.numberOfVampiresAt(r, c + 1) + a.numberOfVampiresAt(r + 1, c) + a.numberOfVampiresAt(r - 1, c);
    }
    else if (dir == WEST)
    {
        c--;
        vampCounter = a.numberOfVampiresAt(r, c - 1) + a.numberOfVampiresAt(r + 1, c) + a.numberOfVampiresAt(r - 1, c);
    }
    else if (dir == SOUTH)
    {
        r++;
        vampCounter = a.numberOfVampiresAt(r + 1, c) + a.numberOfVampiresAt(r, c + 1) + a.numberOfVampiresAt(r, c - 1);
    }
    return vampCounter;
}

  // Recommend a move for a player at (r,c):  A false return means the
  // recommendation is that the player should drop a poisoned blood vial and
  // not move; otherwise, this function sets bestDir to the recommended
  // direction to move and returns true.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
      //Defined an array that would check if the player is next to vampires in any four directions
          int vampTracker[NUMDIRS] = {MAXVAMPIRES, MAXVAMPIRES, MAXVAMPIRES, MAXVAMPIRES};
          
          // This bool will be changed to true if there is at least one safe position to move to.
          bool oneSafeMove = false;
          
          // Determines whether there are any vampires directly next to the player in any direction
          // In case there is vampire south of the player:
          if (a.numberOfVampiresAt(r + 1, c) != 0)
          {
              // Checks whether it is possible to move in each direction, and how many vampires the player would be next to if they were to move in that direction.
              if (checkMove(a, NORTH, r, c) == true && a.numberOfVampiresAt(r - 1, c) == 0)
              {
                  vampTracker[NORTH] = checkVamps(a, NORTH, r, c);
                  oneSafeMove = true;
              }
              if (checkMove(a, WEST, r, c) == true && a.numberOfVampiresAt(r, c - 1) == 0)
              {
                  vampTracker[WEST] = checkVamps(a, WEST, r, c);
                  oneSafeMove = true;
              }
              if (checkMove(a, EAST, r, c) == true && a.numberOfVampiresAt(r, c + 1) == 0)
              {
                  vampTracker[EAST] = checkVamps(a, EAST, r, c);
                  oneSafeMove = true;
              }
              if (oneSafeMove == false)
                  return false;
              
              // This code finds which direction will expose the player to the least amount of vampires based on the vampTracker array, and sets bestDir to that direction.
              int vampCount = MAXVAMPIRES;
              int safestDir = 0;
              for (int i = 0; i < NUMDIRS; i++)
              {
                  if (vampTracker[i] < vampCount)
                  {
                      safestDir = i;
                      vampCount = vampTracker[i];
                  }
              }
              bestDir = safestDir;
              return true;
          }
          
          // In case there is a vampire north of the player
          if (a.numberOfVampiresAt(r - 1, c) != 0) // North
          {
              if (checkMove(a, SOUTH, r, c) == true && a.numberOfVampiresAt(r + 1, c) == 0)
              {
                  vampTracker[SOUTH] = checkVamps(a, SOUTH, r, c);
                  oneSafeMove = true;
              }
              if (checkMove(a, WEST, r, c) == true && a.numberOfVampiresAt(r, c - 1) == 0)
              {
                  vampTracker[WEST] = checkVamps(a, WEST, r, c);
                  oneSafeMove = true;
              }
              if (checkMove(a, EAST, r, c) == true && a.numberOfVampiresAt(r, c + 1) == 0)
              {
                  vampTracker[EAST] = checkVamps(a, EAST, r, c);
                  oneSafeMove = true;
              }
              if (oneSafeMove == false)
                  return false;
              
              int vampCount = MAXVAMPIRES;
              int safestDir = 0;
              for (int i = 0; i < NUMDIRS; i++)
              {
                  if (vampTracker[i] < vampCount)
                  {
                      safestDir = i;
                      vampCount = vampTracker[i];
                  }
              }
              bestDir = safestDir;
              return true;
          }
          
          // In case there is vampire east of the player
          if (a.numberOfVampiresAt(r, c + 1) != 0) // East
          {
              if (checkMove(a, WEST, r, c) == true && a.numberOfVampiresAt(r, c - 1) == 0)
              {
                  vampTracker[WEST] = checkVamps(a, WEST, r, c);
                  oneSafeMove = true;
              }
              if (checkMove(a, SOUTH, r, c) == true && a.numberOfVampiresAt(r + 1, c) == 0)
              {
                  vampTracker[SOUTH] = checkVamps(a, SOUTH, r, c);
                  oneSafeMove = true;
              }
              if (checkMove(a, NORTH, r, c) == true && a.numberOfVampiresAt(r - 1, c) == 0)
              {
                  vampTracker[NORTH] = checkVamps(a, NORTH, r, c);
                  oneSafeMove = true;
              }
              if (oneSafeMove == false)
                  return false;
              
              int vampCount = MAXVAMPIRES;
              int safestDir = 0;
              for (int i = 0; i < NUMDIRS; i++)
              {
                  if (vampTracker[i] < vampCount)
                  {
                      safestDir = i;
                      vampCount = vampTracker[i];
                  }
              }
              bestDir = safestDir;
              return true;
          }
          
          // In case there is vampire west of the player
          if (a.numberOfVampiresAt(r, c - 1) != 0) // West
          {
              if (checkMove(a, EAST, r, c) == true && a.numberOfVampiresAt(r, c + 1) == 0)
              {
                  vampTracker[EAST] = checkVamps(a, EAST, r, c);
                  oneSafeMove = true;
              }
              if (checkMove(a, SOUTH, r, c) == true && a.numberOfVampiresAt(r + 1, c) == 0)
              {
                  vampTracker[SOUTH] = checkVamps(a, SOUTH, r, c);
                  oneSafeMove = true;
              }
              if (checkMove(a, NORTH, r, c) == true && a.numberOfVampiresAt(r - 1, c) == 0)
              {
                  vampTracker[NORTH] = checkVamps(a, NORTH, r, c);
                  oneSafeMove = true;
              }
              if (oneSafeMove == false)
                  return false;
              
              int vampCount = MAXVAMPIRES;
              int safestDir = 0;
              for (int i = 0; i < NUMDIRS; i++)
              {
                  if (vampTracker[i] < vampCount)
                  {
                      safestDir = i;
                      vampCount = vampTracker[i];
                  }
              }
              bestDir = safestDir;
              return true;
          }
          
          // If there are no vampires around the player and the player's current position has a poison vial on it, it will determine if there is another safe position to move to without a poison vial on it and move the player there.
          if (a.getCellStatus(r, c) == HAS_POISON)
          {
              if (checkMove(a, WEST, r, c) == true && a.numberOfVampiresAt(r, c - 1) == 0)
              {
                  vampTracker[WEST] = checkVamps(a, WEST, r, c);
              }
              if (checkMove(a, EAST, r, c) == true && a.numberOfVampiresAt(r, c + 1) == 0)
              {
                  vampTracker[EAST] = checkVamps(a, EAST, r, c);
              }
              if (checkMove(a, SOUTH, r, c) == true && a.numberOfVampiresAt(r + 1, c) == 0)
              {
                  vampTracker[SOUTH] = checkVamps(a, SOUTH, r, c);
              }
              if (checkMove(a, NORTH, r, c) == true && a.numberOfVampiresAt(r - 1, c) == 0)
              {
                  vampTracker[NORTH] = checkVamps(a, NORTH, r, c);
              }
              
              // Finds a position to move the player to that will expose the player to zero vampires and where the cell is empty.
              for (int i = 0; i < NUMDIRS; i++)
                  if (vampTracker[i] == 0)
                      switch(i)
                  {
                      case NORTH:
                          if (checkMove(a, NORTH, r, c) == true)
                          {
                              if (a.getCellStatus(r - 1, c) == EMPTY)
                              {
                                  bestDir = i;
                                  return true;
                              }
                          }
                          break;
                      case EAST:
                          if (checkMove(a, EAST, r, c) == true)
                          {
                              if (a.getCellStatus(r, c + 1) == EMPTY)
                              {
                                  bestDir = i;
                                  return true;
                              }
                          }
                          break;
                      case SOUTH:
                          if (checkMove(a, SOUTH, r, c) == true)
                          {
                              if (a.getCellStatus(r + 1, c) == EMPTY)
                              {
                                  bestDir = i;
                                  return true;
                              }
                          }
                          break;
                      case WEST:
                          if (checkMove(a, WEST, r, c) == true)
                          {
                              if (a.getCellStatus(r, c - 1) == EMPTY)
                              {
                                  bestDir = i;
                                  return true;
                              }
                          }
                          break;
                  }
              return false;
          }
          return false;
}

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(10, 12, 1);

      // Play the game
    g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                                                        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif
