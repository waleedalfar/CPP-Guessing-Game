#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>

using namespace std;

/*----------------------------------------------------
class to model player
----------------------------------------------------*/
class Player {
public:
    Player() : name(""), num_guesses(0) {}
    Player(string nm, int ng) : name(nm), num_guesses(ng) {}

    string getName() const { return name; }
    int getNumGuesses() const { return num_guesses; }

    void setName(string nm) { name = nm; }
    void setNumGuesses(int ng) { num_guesses = ng; }

    void playGame() {
        cout << "Please enter your name to start:\n";
        cin >> name;

        srand(static_cast<unsigned int>(time(nullptr)));
        int numberToGuess = 10 + (rand() % 91); // random number between 10-100
        double squareRoot = sqrt(numberToGuess);
        cout << fixed << squareRoot << " is the square root of what number?\n";

        bool done = false;
        num_guesses = 0;

        while (!done) {
            int guess = GetGuess();
            num_guesses++;
            cout << (guess != numberToGuess ? (guess < numberToGuess ? "Too low, guess again: " : "Too high, guess again: ") : "");
            done = (guess == numberToGuess);
        }

        cout << "You got it, baby!\n";
        cout << "You made " << num_guesses << " guesses.\n";
    }

private:
    string name;
    int num_guesses;

    int GetGuess() {
        int guess;
        cout << "Guess a value between 10 and 100:\n";
        cin >> guess;
        return guess;
    }
};

/*----------------------------------------------------
class for the LeaderBoard
----------------------------------------------------*/
class LeaderBoard {
public:
    LeaderBoard() {
        leaders.resize(NUM_LEADERS, Player("", 1000)); // initialization
    }

    void readLeaders(const string& filename) {
        FILE* file = fopen(filename.c_str(), "r");
        if (!file) return;

        char name[30];
        int guesses;
        for (int i = 0; i < NUM_LEADERS; i++) {
            if (fscanf(file, "%s %d", name, &guesses) == 2) {
                leaders[i] = Player(name, guesses);
            } else {
                break;
            }
        }

        fclose(file);
    }

    void saveLeaders(const string& filename) {
        FILE* file = fopen(filename.c_str(), "w");
        if (!file) {
            cerr << "Error saving leaderboard!\n";
            return;
        }

        for (int i = 0; i < NUM_LEADERS; i++) {
            if (leaders[i].getNumGuesses() < 1000) {
                fprintf(file, "%s %d\n", leaders[i].getName().c_str(), leaders[i].getNumGuesses());
            }
        }

        fclose(file);
    }

    void insertPlayer(const Player& player) {
        for (int i = 0; i < NUM_LEADERS; i++) {
            if (player.getNumGuesses() < leaders[i].getNumGuesses()) {
                for (int j = NUM_LEADERS - 1; j > i; j--) {
                    leaders[j] = leaders[j - 1];
                }
                leaders[i] = player;
                break;
            }
        }
    }

    void display() const {
        cout << "Here are the current leaders:\n";
        for (int i = 0; i < NUM_LEADERS; i++) {
            if (leaders[i].getNumGuesses() < 1000) {
                cout << leaders[i].getName() << " made " << leaders[i].getNumGuesses() << " attempts\n";
            }
        }
    }

private:
    static const int NUM_LEADERS = 5;
    vector<Player> leaders; 
};

/*----------------------------------------------------
main
prompts the user to play or quit and then enters a loop
to allow the user to play as many times as they like
----------------------------------------------------*/
int main() {
    LeaderBoard leaderboard;
    leaderboard.readLeaders("leaderboard.txt");

    cout << "Welcome! Press 'q' to quit or any other key to continue:\n";
    char c;
    bool game_over = false;

    while (!game_over) {
        cin >> c;
        if (c == 'q') {
            game_over = true;
            cout << "Bye Bye!\n";
        } else {
            Player current_player;
            current_player.playGame();
            leaderboard.insertPlayer(current_player);
            leaderboard.display();
            cout << "Press 'q' to quit or any other key to play again:\n";
        }
    }

    leaderboard.saveLeaders("leaderboard.txt");
    return 0;
}