#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

class ExpenseSplitter {
private:
    unordered_map<string, unordered_map<string, double>> balances;

public:
    void addExpense(string payer, double amount, vector<string> participants) {
        int n = participants.size();
        double splitAmount = amount / n;

        for (string& person : participants) {
            if (person == payer) continue;
            balances[person][payer] += splitAmount;
        }
    }

    void showBalances() {
        // Net off mutual debts
        for (auto& debtor : balances) {
            for (auto& creditor : debtor.second) {
                string a = debtor.first;
                string b = creditor.first;

                double ab = balances[a][b];
                double ba = balances[b][a];

                if (ba > 0) {
                    if (ab > ba) {
                        balances[a][b] -= ba;
                        balances[b][a] = 0;
                    } else {
                        balances[b][a] -= ab;
                        balances[a][b] = 0;
                    }
                }
            }
        }

        // Show final balances
        bool anyBalance = false;
        for (auto& debtor : balances) {
            for (auto& creditor : debtor.second) {
                if (creditor.second > 0.0) {
                    cout << fixed << setprecision(2)
                         << debtor.first << " owes " << creditor.first
                         << " Rs. " << creditor.second << endl;
                    anyBalance = true;
                }
            }
        }

        if (!anyBalance) {
            cout << "No balances to show. Everyone is settled up!" << endl;
        }
    }
};

int main() {
    ExpenseSplitter splitter;
    int numExpenses;

    cout << "Enter number of expenses: ";
    cin >> numExpenses;
    cin.ignore();

    for (int i = 0; i < numExpenses; ++i) {
        string payer;
        double amount;
        int numParticipants;

        cout << "\nExpense " << i + 1 << ":\n";
        cout << "Enter payer's name: ";
        getline(cin, payer);

        cout << "Enter total amount: ";
        cin >> amount;

        cout << "Enter number of participants: ";
        cin >> numParticipants;
        cin.ignore();

        vector<string> participants(numParticipants);
        cout << "Enter participant names:\n";
        for (int j = 0; j < numParticipants; ++j) {
            getline(cin, participants[j]);
        }

        splitter.addExpense(payer, amount, participants);
    }

    cout << "\nFinal Balances:\n";
    splitter.showBalances();

    return 0;
}
