#include<iostream>
#include<thread>
#include<chrono>
#include<unordered_map>
#include<deque>
#include<algorithm>
#include<random>
#include<vector>

using namespace std;

//Class for utility functions
class Utility {
public:
    //function to create a delay of 2 seconds
    static void delay(float seconds) {
        this_thread::sleep_for(chrono::duration<float>(seconds));
    }

    //function to extract the value from card name (1H -> 1)
    static int get_card_value(string card) {
        return stoi(card.substr(0, card.length() - 1));
    }
};

class Player {
public:
    int player_count;
    string name;
    deque<string> cards;

    //Default Constructor
    Player() {}

    //Get and remove top card
    string get_top_card() {
        string card = cards.front();
        cards.pop_front();
        return card;
    }

    //Push cards to bottom of deck
    void push_to_bottom(vector<string>& cards_on_table) {
        for (string card : cards_on_table) {
            cards.push_back(card);
        }
        cards_on_table.clear();
    }
};

class Cards {
public:
    vector<string> deck = {
        "1H", "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "10H", "11H", "12H", "13H",
        "1S", "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "10S", "11S", "12S", "13S",
        "1C", "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "10C", "11C", "12C", "13C",
        "1D", "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "10D", "11D", "12D", "13D"
    };

    //Default Constructor
    Cards() {}

    //Function to shuffle the deck
    void shuffle_deck() {
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        mt19937 gen(seed);
        shuffle(deck.begin(), deck.end(), gen);
    }

    //Function to distribute 5 random cards to each player
    void distribute_cards(Player& p1, Player& p2) {
        for (int i = 0; i < 10; i += 2) {
            p1.cards.push_back(deck[i]);
            p2.cards.push_back(deck[i + 1]);
        }
    }
};

class Game {
public:
    Player p1;
    Player p2;
    Cards c_obj;

    int round = 0;
    const int MAX_ROUNDS = 30;

    Game() {}

    void display_rules() {
        cout << "\n=== Welcome to the CLI Card Duel! ===\n";
        cout << "=======================================\n";
        cout << "Players: 2\n";
        cout << "Deck: 52 cards (1-13 in each suit)\n\n";
    
        cout << " How to Play:\n";
        cout << "   -> Each player is dealt 5 random cards (no duplicates).\n";
        cout << "   -> On each turn, both players play their top card.\n";
        cout << "       >> Higher card wins both and places them at the bottom of their stack.\n";
        cout << "       >> If tied, cards stay on table, and next cards are played.\n";
        cout << "       >> This continues until tie is broken.\n";
        cout << "   -> Repeat until one player has no cards left.\n";
        cout << "   -> Player with cards remaining is the WINNER!\n";
        cout << "   -> If both run out at the same time: It's a DRAW!\n";
        cout << "   -> If the number of rounds exceeds the limit, the player with more cards wins.\n";
        cout << "       >> If both have the same number of cards, it's a DRAW.\n";
        cout << "=======================================\n\n";
    }

    //Function to get player names
    void get_player_info() {
        p1.player_count = 1;
        cout << "Enter player1 name: ";
        cin >> p1.name;

        p2.player_count = 2;
        cout << "Enter player2 name: ";
        cin >> p2.name;

        cout << "---------------------------------------------------\n\n";
        cout << "Player 1: " << p1.name << "\t\t" << "Player 2: " << p2.name << endl;
        cout << "-----------------------------------------------------\n\n";
    }

    //Function to simulate the game
    void play() {
        vector<string> cards_on_table;

        cout << "Let's start the Game!\n\n";
        Utility::delay(1.5);

        //shuffle the deck
        c_obj.shuffle_deck();

        //distribute 5 cards to each player
        c_obj.distribute_cards(p1, p2);

        //start the game
        while (!p1.cards.empty() && !p2.cards.empty() && round < MAX_ROUNDS) {
            round++;

            string card1 = p1.get_top_card();
            string card2 = p2.get_top_card();

            cards_on_table.push_back(card1);
            cards_on_table.push_back(card2);

            int val1 = Utility::get_card_value(card1);
            int val2 = Utility::get_card_value(card2);

            cout << "ROUND " << round << "\n";
            cout << "------------------------------\n";
            cout << "| " << p1.name << ": " << card1 << "  vs  " << p2.name << ": " << card2 << " |\n";
            Utility::delay(1);

            if (val1 > val2) {
                p1.push_to_bottom(cards_on_table);
                cout << "| " << p1.name << " gets the cards! |\n";
            }
            else if (val2 > val1) {
                p2.push_to_bottom(cards_on_table);
                cout << "| " << p2.name << " gets the cards! |\n";
            }
            else {
                cout << "| It's a tie! Cards stay on table! |\n";
            }

            cout << "------------------------------\n";

            cout << p1.name << " : " << p1.cards.size() << " cards  ";
            cout << p2.name << " : " << p2.cards.size() << " cards\n";
            cout << "Cards on Table: " << cards_on_table.size() << "\n";
            cout << "------------------------------\n\n";

            Utility::delay(1.5);
        }

        // Game over - declare result
        cout << "\n=== GAME OVER ===\n";

        if (round >= MAX_ROUNDS) {
            cout << "\n>> Game ended due to too many rounds.\n";
        }

        int cards_p1 = p1.cards.size();
        int cards_p2 = p2.cards.size();

        if (cards_p1 == cards_p2) {
            cout << ">> Result: It's a DRAW!\n";
        }
        else {
            string winner = (cards_p1 > cards_p2) ? p1.name : p2.name;
            cout << "\n>> WINNER: " << winner << "  <<< ## >>>\n";
        }
    }
};

int main() {
    Game game;
    game.display_rules();
    game.get_player_info();
    game.play();
    return 0;
}
