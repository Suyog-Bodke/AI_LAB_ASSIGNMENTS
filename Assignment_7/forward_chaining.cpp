#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
using namespace std;

// Structure to hold a rule (conditions → conclusion)
struct Rule {
    vector<string> conditions;
    string conclusion;
};

// Forward chaining function
set<string> apply_forward_chaining(vector<string> initial_facts, vector<Rule> inference_rules) {
    set<string> known_facts(initial_facts.begin(), initial_facts.end());
    bool new_fact_found = true;

    while (new_fact_found) {
        new_fact_found = false;

        for (auto& rule : inference_rules) {
            bool all_conditions_met = true;

            // Check if all rule conditions are in known facts
            for (auto& condition : rule.conditions) {
                if (known_facts.find(condition) == known_facts.end()) {
                    all_conditions_met = false;
                    break;
                }
            }

            // If conditions are met and conclusion is not yet known
            if (all_conditions_met && known_facts.find(rule.conclusion) == known_facts.end()) {
                known_facts.insert(rule.conclusion);
                cout << "Derived new fact: " << rule.conclusion << endl;
                new_fact_found = true;
            }
        }
    }

    return known_facts;
}

int main() {
    // Initial facts
    vector<string> initial_facts = {"X", "Y"};

    // Inference rules (conditions → conclusion)
    vector<Rule> inference_rules = {
        {{"X", "Y"}, "Z"},
        {{"Z"}, "W"},
        {{"W"}, "V"}
    };

    cout << "Starting facts: ";
    for (auto& f : initial_facts) cout << f << " ";
    cout << endl;

    // Apply forward chaining
    set<string> derived_facts = apply_forward_chaining(initial_facts, inference_rules);

    // Print final derived facts
    cout << "Final derived facts: ";
    for (auto& f : derived_facts) cout << f << " ";
    cout << endl;

    return 0;
}