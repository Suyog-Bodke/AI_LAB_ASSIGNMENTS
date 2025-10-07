#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// Structure to hold a rule (conditions → conclusion)
struct Rule {
    vector<string> conditions;
    string conclusion;
};

// Backward chaining function
bool backward_chaining(const string& goal, const vector<string>& known_facts, const vector<Rule>& inference_rules) {
    // Check if the goal is already known
    if (find(known_facts.begin(), known_facts.end(), goal) != known_facts.end()) {
        cout << "Goal " << goal << " is already known." << endl;
        return true;
    }

    // Try to find a rule that can conclude this goal
    for (const auto& rule : inference_rules) {
        if (rule.conclusion == goal) {
            cout << "Trying to prove goal: " << goal << " using rule with conclusion " << rule.conclusion << endl;

            bool all_conditions_met = true;

            // Try to prove each condition recursively
            for (const auto& condition : rule.conditions) {
                if (!backward_chaining(condition, known_facts, inference_rules)) {
                    all_conditions_met = false;
                    break;
                }
            }

            if (all_conditions_met) {
                cout << "Goal " << goal << " has been proven." << endl;
                return true;
            }
        }
    }

    cout << "Goal " << goal << " cannot be proven." << endl;
    return false;
}

int main() {
    // Initial known facts
    vector<string> known_facts = {"X", "Y"};

    // Inference rules (conditions → conclusion)
    vector<Rule> inference_rules = {
        {{"X", "Y"}, "Z"},
        {{"Z"}, "W"},
        {{"W"}, "V"}
    };

    string goal_to_prove = "V";

    cout << "\nStarting backward chaining to prove goal: " << goal_to_prove << endl;
    bool can_prove_goal = backward_chaining(goal_to_prove, known_facts, inference_rules);

    if (can_prove_goal)
        cout << "Final conclusion: The goal " << goal_to_prove << " can be proven." << endl;
    else
        cout << "Final conclusion: The goal " << goal_to_prove << " cannot be proven." << endl;

    return 0;
}