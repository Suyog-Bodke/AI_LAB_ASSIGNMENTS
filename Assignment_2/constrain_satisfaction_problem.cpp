// missionary_cannibal.cpp
#include <iostream>
#include <limits>

class MissionaryCannibal {
public:
    MissionaryCannibal()
        : left_m(3), left_c(3), left_b(1),
          right_m(0), right_c(0), right_b(0) {}

    void display_state() const {
        std::cout << "Left side: " << left_m << " Missionaries, "
                  << left_c << " Cannibals, Boat: " << (left_b == 1 ? "Yes" : "No") << "\n";
        std::cout << "Right side: " << right_m << " Missionaries, "
                  << right_c << " Cannibals, Boat: " << (right_b == 1 ? "Yes" : "No") << "\n";
    }

    // Move given numbers. to_right == true means boat is moving from left -> right.
    bool move(int missionaries, int cannibals, bool to_right) {
        if (missionaries + cannibals > 2 || missionaries < 0 || cannibals < 0 || (missionaries == 0 && cannibals == 0))
            return false;

        if (to_right) {
            if (left_m >= missionaries && left_c >= cannibals && left_b == 1) {
                left_m -= missionaries;
                left_c -= cannibals;
                right_m += missionaries;
                right_c += cannibals;
                left_b = 0;
                right_b = 1;
                return true;
            }
        } else { // moving right -> left
            if (right_m >= missionaries && right_c >= cannibals && right_b == 1) {
                right_m -= missionaries;
                right_c -= cannibals;
                left_m += missionaries;
                left_c += cannibals;
                right_b = 0;
                left_b = 1;
                return true;
            }
        }
        return false;
    }

    bool check_lose() const {
        // If cannibals outnumber missionaries on either side (and there's at least one missionary there) -> lose
        if ((left_m > 0 && left_c > left_m) || (right_m > 0 && right_c > right_m))
            return true;
        return false;
    }

    bool win() const {
        return (right_m == 3 && right_c == 3);
    }

    // helper to know if boat is currently on left
    bool is_boat_on_left() const { return left_b == 1; }

private:
    int left_m, left_c, left_b;
    int right_m, right_c, right_b;
};

int main() {
    MissionaryCannibal game;

    std::cout << "Missionaries and Cannibals puzzle\n";
    std::cout << "Rules: Move 1 or 2 people by boat. Never let cannibals outnumber missionaries on either side.\n\n";

    while (true) {
        game.display_state();

        if (game.win()) {
            std::cout << "\nCongratulations! All missionaries and cannibals have successfully crossed the river.\n";
            break;
        }

        if (game.check_lose()) {
            std::cout << "\nYou lose. Cannibals outnumber missionaries on one side.\n";
            break;
        }

        int missionaries = 0, cannibals = 0;
        std::cout << "\nBoat is on the " << (game.is_boat_on_left() ? "LEFT" : "RIGHT") << " side.\n";
        std::cout << "Enter number of missionaries to move (0-2): ";

        if (!(std::cin >> missionaries)) {
            std::cout << "Invalid input. Please enter integers.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cout << "Enter number of cannibals to move (0-2): ";
        if (!(std::cin >> cannibals)) {
            std::cout << "Invalid input. Please enter integers.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        bool to_right = game.is_boat_on_left();

        if (!game.move(missionaries, cannibals, to_right)) {
            std::cout << "Invalid move. Try again. (Max 2 people, must have enough people on that side, and boat must be there)\n";
        }

        std::cout << "----------------------------------------\n";
    }

    return 0;
}
