#include <iostream>
#include <ctime>
#include <cstdlib>
#include <unistd.h>  // Library for sleep()
#include <string>    // Library for isdigit() / menu option validation
#include <stdlib.h>  // Library for system("clear");

using namespace std;

 //Global variables
   enum MenuOptions {Match = 1, Display = 2, Giveup = 3, Exit = 4};
   const int SIZE = 4;

// FUNCTIONS AND DEFINITIONS ************************************

void displayHeader()
{
        cout << "*** Welcome to Keaton, David and Peter's Memory Card Game ***" << endl;
}

// function checks username is valid and returns it
string getName(bool validUser)
{
    // Username validation loop
	string username;
    while (!validUser)
    {
        validUser = true;  // Assume the username is valid
        cout << "Enter username: ";
        getline(cin, username);

        // Check for special characters in the username
        for (int i = 0; i < username.length(); ++i)
        {
            char ch = username.at(i);
            if (ispunct(ch))
            {
                cout << "No special characters, try again." << endl;
                validUser = false;
                break;
            }
        }
    }
	cout << "Welcome " << username << "!" << endl;
	return username;
}

// function returns a randomly generated index of 2D array
int genRandomIndex()
{
	return rand() % SIZE; // returns random number between 0 and SIZE - 1
}

// function shuffles the "cards" in the 2D array
void shuffle(int cards[SIZE][SIZE]) // MIGHT have to pass by reference!
{
	for (int i = 0; i < 15; ++i) // "shuffling"
	{
	   int x1 = genRandomIndex();
	   int y1 = genRandomIndex();
	   int x2 = genRandomIndex();
	   int y2 = genRandomIndex();

	   // to swap two randomly selected cards
	   swap(cards[x1][y1], cards[x2][y2]);
	}
}

// function to determine where the card is and the cards status (face up / down)
void initialize(int cards[SIZE][SIZE], int cardStatus[SIZE][SIZE])
{
	int value = 1;
	for (int i = 0; i < SIZE; ++i)
	{
	   for (int j = 0; j < SIZE; ++j)
	   {
	      cards[i][j] = value;
	      cardStatus[i][j] = 0; // to set all cards face down
	      if ((i * SIZE + j) % 2 == 1) ++value; // pair cards
	   }
	}
	// calling shuffle function
	shuffle(cards);

}

// Function to display the cards in a coordinate system
void showCards(int cards[SIZE][SIZE], int cardStatus[SIZE][SIZE], bool showReal = false)
{
    cout << "     0 1 2 3   " << endl; // WAS \t0 \t1 \t2 \t3
    cout << "   __________  " << endl;
    // to iterate over each row
    for (int i = 0; i < SIZE; ++i)
    {
            cout << " " << i << " | "; // show row number with |
            // to iterate over each column
            for (int j = 0; j < SIZE; ++j)
            {
                // to check if we should show the real card or if the card is face up
                if (showReal || cardStatus[i][j] == 1)
                {
                  cout << cards[i][j] << " "; // print the card value // WAS " \t"
                }
                else
                {
                   cout << "* "; // print '*' for face down cards
                }
            }
            cout << endl; // to move to the next line after each row
     }
}

// Function to check if all cards are face up
bool allFaceUp(int cardStatus[SIZE][SIZE])
{
    // to iterate over each row
    for (int i = 0; i < SIZE; ++i)
    {
        // iterate over each column
        for (int j = 0; j < SIZE; ++j)
        {
            // if any cards is not face up, return false
            if (cardStatus[i][j] == 0)  // If any cards status is 0/face down then false
            {
                return false;
            }
        }
    }
    return true; // all cards are face up
}

// Function to check if the input coordinates are valid
bool checkInputs(int x, int y, int cardStatus[SIZE][SIZE])
{
    // to check if x and y are within the valid range and the card is not already faced up
    return (x >= 0 && x < SIZE && y >= 0 && y < SIZE && cardStatus[x][y] == 0);
}

// function to match cards and update their status if they match
void match(int cards[SIZE][SIZE], int cardStatus[SIZE][SIZE], int &points)
{
    // to display the current arrangement of cards
    showCards(cards, cardStatus);

    int x1;
    int y1;
    int x2;
    int y2;
    // to prompt the user to enter coordinates of the cards
    cout << "Enter coordinates for the first card (x,y): ";
    cin >> x1 >> y1;
    cout << "Enter coordinates for the second card (x,y): ";
    cin >> x2 >> y2;

    // to check if the input coordinates are valid
    if (!(checkInputs(x1, y1,cardStatus)) || !(checkInputs(x2, y2,cardStatus))) // Not sure if these conditions syntax is correct
    {
        cout << "Invalid coordinates. Please enter values between 0 and " << SIZE - 1 << ".\n";
        return;
    }

    // to ensure the two selected cards are not the same
    if (x1 == x2 && y1 == y2)
    {
        cout << "The coordinate values of the two cards should be different.\n";
        return;
    }

    // to check if either card is already faced up
    if (cardStatus[x1][y1] == 1 || cardStatus[x2][y2] == 1)
    {
        cout << "One or both of the cards are already faced up.\n";
        return;
    }

    // to extract card values using the provided coordinates
    int card1 = cards[x1][y1];
    int card2 = cards[x2][y2];

    // to check if the card values match
    if (card1 == card2)
    {
        // to set the cards' status to face up
        cardStatus[x1][y1] = 1;
        cardStatus[x2][y2] = 1;
        // increase points by 5
        points += 5;
        cout << "Cards match! Point: " << points << "\n";
    } else
    {
        cout << "Cards do not match. Memorize their positions." << endl;
	cout << endl; //  THIS IS INTENTIONAL!! Leave in for formatting 
        // to show unmatched cards temporarily for 5 seconds
        showCards(cards, cardStatus, true);
        sleep(5); // to freeze screen for 5 seconds
        system("clear"); // clear the screen // WAS CLS
        // decrease points by 1
        points -= 1;
        cout << "Points: " << points << "\n";
    }
}

// Function to display one identical pair
void display(int cards[SIZE][SIZE], int cardStatus[SIZE][SIZE], int &points) 
{
    // Check if points are sufficient for display
    if (points < 10) 
    {
        cout << "Insufficient points to display identical pairs. You need at least 10 points." << endl;
        return;
    }

    // Loop through the array to find the first unmatched identical pair
    bool pairFound = false;
    for (int i = 0; i < SIZE && !pairFound; i++) 
    {
        for (int j = 0; j < SIZE && !pairFound; j++) 
	{
            // Check if the card is face down
            if (cardStatus[i][j] == 0) 
	    {
                int firstCardValue = cards[i][j];

                // Look for another card with the same value that is also face down
                for (int x = 0; x < SIZE && !pairFound; x++) 
		{
                    for (int y = 0; y < SIZE && !pairFound; y++) 
		    {
                        if (x == i && y == j) continue;  // Skip the first card
                        if (cards[x][y] == firstCardValue && cardStatus[x][y] == 0) 
			{
                            // Display the pair by setting their status to face up
                            cardStatus[i][j] = 1;
                            cardStatus[x][y] = 1;
                            pairFound = true;  // Exit loops when we found pair
                        }
                    }
                }
            }
        }
    }

    // Deduct points for displaying a pair
    if (pairFound) 
    {
        points -= 10;
        cout << "An identical pair has been displayed. 10 points have been deducted." << endl;
    } 
	else 
	{
        cout << "No unmatched pairs left to display." << endl;
        }
}

//FUNCTIONS ENDING *******************************************************************************************

int main()
{
	// Declared Variable/s ***
    srand(time(0));      // seed random number generator

	int points = 50;  // player's starting points : Step 13
    int menuChoice;   // player's choice for Menu
    bool validUser = false;
	string userName;
    char playAgain;   // play again input
	bool showReal = true; //NEW!!!!!!!!!!
	bool exitGame = false;

	int cards[SIZE][SIZE] =  // 16 cards array : Step 14 (A)
        {
	 {1, 1, 2, 2},
	 {3, 3, 4, 4},
	 {5, 5, 6, 6},
	 {7, 7, 8, 8}
	};

	int cardStatus[SIZE][SIZE]; // array to hold cards status (up = 1 or down = 0) : Step 14(B)

        displayHeader(); // Program title block

        initialize(cards, cardStatus); // Step 15
        userName = getName(validUser); // Step 16
        cout << "Points Total: " << points << "\n" <<  endl;

        do
        {
          cout << "Enter which option you would like to do: " << endl;
          cout << "Match = 1 " << endl;
          cout << "Display = 2 " << endl;
          cout << "Give Up = 3 " << endl;
          cout << "Exit = 4 " << endl;
          cin >> menuChoice;     // Build a isdigit() input validation with loop ******

            switch(menuChoice)
            {
               case Match:
                      cout << "You choose Match\n" << endl;
                               match(cards,cardStatus,points);  // Calls the function : Needs testing ******

                               if (allFaceUp(cardStatus)) // fixed cardstatus
                               {
                                  cout << userName << " has WON the game!!!" << endl;
                                  cout << "Your TOTAL POINTS are: " << points << "\n" << endl;
                                  cout << "Would you like to play again? Press Y for yes or N for no: ";
                                  cin >> playAgain;
                                  playAgain = toupper(playAgain);

                                  if (playAgain == 'y' || playAgain == 'Y')
                                   {
                                     initialize(cards, cardStatus);  // Start game again
                                     points = 50;
                                   }
                                   else {
                                    exitGame = true;
				    return 0;
                                   }

                               }
                      break;
               case Display:
                      cout << "You choose Display" << endl;
                               display(cards, cardStatus, points);// Put Display() here
                      break;
               case Giveup:
                      cout << "You choose to Give Up" << endl;
                               showCards(cards, cardStatus, showReal);
                      cout << "Would you like to play again? Press Y for yes or Any key for no: ";
                                  cin >> playAgain;
                                  playAgain = toupper(playAgain);

                                  if (playAgain == 'y' || playAgain == 'Y')
                                   {
                                     initialize(cards, cardStatus);  // Start game again
                                   }
                                  else
                                   {
                                    cout << "\nWe hope you have enjoyed our game " << userName << endl;
                                    cout << "Your Final Points are: " << points << endl;
                                    cout << "\n Have a Nice Day :)\n" << endl;
                                    exitGame = true;
									return 0;
                                   }
                       break;
               case Exit:
                      cout << "\nWe hope you have enjoyed our game " << userName << endl;
                      cout << "Your Final Points are: " << points << endl;
                      cout << "\n Have a Nice Day :)\n" << endl;
                      exitGame = true;
                      break;
               default:
                      cout << "Invalid input, Please try again " << endl;
                      break;
            } // switch case closing brackett

        }while(menuChoice != Exit);  // uses the menuChoice as the exit condition


   return 0;
}

