// family_tree.cpp
#include <bits/stdc++.h>
using namespace std;

// Helpers
vector<string> unique_sorted(vector<string> v) {
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    return v;
}

class FamilyTree {
public:
    // facts
    set<string> males;
    set<string> females;
    // parent -> list of children
    unordered_map<string, vector<string>> children_of;
    // child -> list of parents
    unordered_map<string, vector<string>> parents_of;

    void assert_male(const string &name) { males.insert(name); }
    void assert_female(const string &name) { females.insert(name); }

    void assert_parent(const string &p, const string &c) {
        children_of[p].push_back(c);
        parents_of[c].push_back(p);
    }

    vector<string> get_children(const string &p) const {
        auto it = children_of.find(p);
        if (it == children_of.end()) return {};
        return it->second;
    }

    vector<string> get_parents(const string &c) const {
        auto it = parents_of.find(c);
        if (it == parents_of.end()) return {};
        return it->second;
    }

    // Rules
    // sibling(X, Y) :- parent(P, X), parent(P, Y), X \= Y
    vector<string> siblings_of(const string &X) const {
        vector<string> result;
        auto parents = get_parents(X);
        for (const string &p : parents) {
            auto kids = get_children(p);
            for (const string &k : kids) {
                if (k != X) result.push_back(k);
            }
        }
        return unique_sorted(result);
    }

    // grandparent(X, Y) :- parent(X, Z), parent(Z, Y)
    vector<string> grandparents_of(const string &Y) const {
        vector<string> result;
        auto parents = get_parents(Y);
        for (const string &p : parents) {
            auto gparents = get_parents(p); // parents of parent are grandparents
            for (const string &gp : gparents) result.push_back(gp);
        }
        return unique_sorted(result);
    }

    // mother(X, Y) :- parent(X, Y), female(X)
    vector<string> mothers_of(const string &Y) const {
        vector<string> result;
        auto parents = get_parents(Y);
        for (const string &p : parents) {
            if (females.count(p)) result.push_back(p);
        }
        return unique_sorted(result);
    }

    // father(X, Y) :- parent(X, Y), male(X)
    vector<string> fathers_of(const string &Y) const {
        vector<string> result;
        auto parents = get_parents(Y);
        for (const string &p : parents) {
            if (males.count(p)) result.push_back(p);
        }
        return unique_sorted(result);
    }

    // grandmother(X, Y) :- mother(X, Z), parent(Z, Y)
    vector<string> grandmothers_of(const string &Y) const {
        vector<string> result;
        auto parents = get_parents(Y);
        for (const string &p : parents) {
            auto moms = mothers_of(p);
            for (const string &m : moms) result.push_back(m);
        }
        return unique_sorted(result);
    }

    // grandfather(X, Y) :- father(X, Z), parent(Z, Y)
    vector<string> grandfathers_of(const string &Y) const {
        vector<string> result;
        auto parents = get_parents(Y);
        for (const string &p : parents) {
            auto dads = fathers_of(p);
            for (const string &d : dads) result.push_back(d);
        }
        return unique_sorted(result);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    FamilyTree ft;

    // Define gender facts (male and female)
    ft.assert_male("john");
    ft.assert_male("tom");
    ft.assert_male("lucas");
    ft.assert_female("mary");
    ft.assert_female("sarah");
    ft.assert_female("kate");
    ft.assert_female("lily");

    // Define family relationships (parent facts)
    ft.assert_parent("john", "sarah");
    ft.assert_parent("john", "tom");
    ft.assert_parent("mary", "sarah");
    ft.assert_parent("mary", "tom");
    ft.assert_parent("sarah", "kate");
    ft.assert_parent("sarah", "lucas");
    ft.assert_parent("tom", "lily");

    // Results similar to the Python+Prolog notebook
    cout << "Sarah's siblings:\n";
    auto siblings = ft.siblings_of("sarah");
    if (!siblings.empty()) {
        for (const auto &s : siblings) cout << s << "\n";
    } else {
        cout << "No siblings found\n";
    }

    cout << "\nKate's grandparents:\n";
    auto gparents = ft.grandparents_of("kate");
    if (!gparents.empty()) {
        for (const auto &g : gparents) cout << g << "\n";
    } else {
        cout << "No grandparents found\n";
    }

    cout << "\nTom's children:\n";
    auto tom_children = ft.get_children("tom");
    tom_children = unique_sorted(tom_children);
    if (!tom_children.empty()) {
        for (const auto &c : tom_children) cout << c << "\n";
    } else {
        cout << "No children found\n";
    }

    cout << "\nKate's mother:\n";
    auto kate_moms = ft.mothers_of("kate");
    if (!kate_moms.empty()) {
        // in original code you used next(...) to get the first; we'll print first if exists
        cout << kate_moms.front() << "\n";
    } else {
        cout << "No mother found\n";
    }

    cout << "\nKate's father:\n";
    auto kate_dads = ft.fathers_of("kate");
    if (!kate_dads.empty()) {
        cout << kate_dads.front() << "\n";
    } else {
        cout << "No father found\n";
    }

    cout << "\nKate's grandmother(s):\n";
    auto kate_gmas = ft.grandmothers_of("kate");
    if (!kate_gmas.empty()) {
        for (const auto &g : kate_gmas) cout << g << "\n";
    } else {
        cout << "No grandmother found\n";
    }

    cout << "\nKate's grandfather(s):\n";
    auto kate_gpas = ft.grandfathers_of("kate");
    if (!kate_gpas.empty()) {
        for (const auto &g : kate_gpas) cout << g << "\n";
    } else {
        cout << "No grandfather found\n";
    }

    return 0;
}
