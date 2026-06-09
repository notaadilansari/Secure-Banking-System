#include <iostream>
#include "json.hpp"
#include <fstream>
#include <random>
#include <string>
#include <vector>

using namespace std;
using json = nlohmann::json;

class bank {
public:
    struct Transaction {
        int id;
        string type;
        string status;
    };

    struct person {
        string username, password;
        float balance = 0;
        vector<Transaction> transaction_history; 
    };

    person current_user;

    bank(string name, string pass) {
        current_user.username = name;
        current_user.password = pass;
    }

    
    bool load_user_data() {
        ifstream file("data.json");
        if (!file.is_open() || file.peek() == ifstream::traits_type::eof()) return false;

        json people_list;
        file >> people_list;
        file.close();

        for (auto& person_json : people_list) {
            if (person_json["username"] == current_user.username && person_json["password"] == current_user.password) {
                current_user.balance = person_json["balance"];
                current_user.transaction_history.clear();
                if (person_json.contains("transactions") && person_json["transactions"].is_array()) {
                    for (auto& tx : person_json["transactions"]) {
                        Transaction t;
                        t.id = tx["transaction id"];
                        t.type = tx["type"];
                        t.status = tx["status"];
                        current_user.transaction_history.push_back(t);
                    }
                }
                return true; 
            }
        }
        return false; 
    }

    void registration() {
        json people_list = json::array();
        
        
        ifstream input_file("data.json");
        if (input_file.is_open() && input_file.peek() != ifstream::traits_type::eof()) {
            input_file >> people_list;
        }
        input_file.close();

        json person_json;
        person_json["username"] = current_user.username;
        person_json["password"] = current_user.password;
        person_json["balance"] = current_user.balance;
        person_json["transactions"] = json::array(); 

        people_list.push_back(person_json);

        ofstream file("data.json");
        if (file.is_open()) {
            file << people_list.dump(4);
            file.close();
        }
    }
};

class operation : public bank {
public:
    operation(string name, string pass) : bank(name, pass) {}

    void deposit(float amount) {
        Transaction tx;
        tx.type = "Deposit";
        
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(1203994, 2387833);
        tx.id = distrib(gen);
        
        if (amount > 0) {
           
            current_user.balance += amount;
            tx.status = "Success";
            cout << "Deposit Successful! ID: " << tx.id << endl;
        } else {
            tx.status = "Failed";
            cout << "Deposit Failed!" << endl;
        }
        check_balance();

        current_user.transaction_history.push_back(tx);
        save_to_json();
    }

    void withdraw(float amount) {
        Transaction tx;
        tx.type = "Withdraw";

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(1203994, 2387833);
        tx.id = distrib(gen);

        if (amount <= current_user.balance && amount > 0) {
            current_user.balance -= amount;
            tx.status = "Success";
            cout << "Withdrawal Successful! ID: " << tx.id << endl;
        } else {
            tx.status = "Failed";
            cout << "Withdrawal Failed! Insufficient Funds." << endl;
        }

        current_user.transaction_history.push_back(tx);
        save_to_json();
    }

    void check_balance(){
        cout << "Current Balance - " << current_user.balance << endl;
    }

    void transaction_history(){
        ifstream file("data.json");
        if (!file.is_open()) {
            cout << "Error: Could not open data.json" << endl;
            return;
        }

        json people_list;
        file >> people_list;
        file.close();

        for (const auto& person_json : people_list) {
            
            if (person_json["username"] == current_user.username) {
                cout << "=========================================" << endl;
                cout << "Account Holder: " << person_json["username"] << endl;
                cout << "Current Balance: $" << person_json["balance"] << endl;
                cout << "----------------------------------------=" << endl;
                cout << "TRANSACTION HISTORY:" << endl;

                if (person_json.contains("transactions") && person_json["transactions"].is_array()) {
                    for (const auto& tx : person_json["transactions"]) {
                        int id = tx["transaction id"];
                        string type = tx["type"];
                        string status = tx["status"];

                        cout << "[ID: " << id << "] " << type << " -> " << status << endl;
                    }
                }
                cout << "=========================================" << endl;
            }
        }
    }

private:
    void save_to_json() {
        
        ifstream file_in("data.json");
        json people_list = json::array();
        if (file_in.is_open() && file_in.peek() != ifstream::traits_type::eof()) {
            file_in >> people_list;
        }
        file_in.close();

        for (auto& person_json : people_list) {
            if (person_json["username"] == current_user.username) {
                person_json["balance"] = current_user.balance;

                json tx_array = json::array();
                for (const auto& tx : current_user.transaction_history) {
                    json tx_json;
                    tx_json["transaction id"] = tx.id;
                    tx_json["type"] = tx.type;
                    tx_json["status"] = tx.status;
                    tx_array.push_back(tx_json);
                }
                person_json["transactions"] = tx_array;
                break;
            }
        }

        ofstream file_out("data.json");
        if (file_out.is_open()) {
            file_out << people_list.dump(4);
            file_out.close();
        }
    }
};

int main() {
    int user_choice1, user_choice2;
    cout << "=========================================" << endl << endl;
    cout << "         SECURE BANKING SYSTEM           " << endl << endl;
    cout << "=========================================" << endl << endl;
    
    while (true) {
        cout << "1.Register" << endl
             << "2.Login" << endl
             << "3.Exit" << endl;
        cout << "Enter your choice - ";
        cin >> user_choice1;
        cout << endl;
        
        if (user_choice1 == 1) {
            string username, password;
            cout << "Enter username - ";
            cin >> username;
            cout << "Enter password - ";
            cin >> password;
            operation user(username, password);
            user.registration();
            cout << "Account created successfully!!" << endl;
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> distrib(1203994029, 2387833394);
            cout << "Account number - " << distrib(gen) << endl;
            cout << endl;
            cout << "=========================================" << endl << endl;
        }
        else if (user_choice1 == 2) {
            string username, password;
            cout << "Enter username - ";
            cin >> username;
            cout << "Enter password - ";
            cin >> password;
            
            operation user(username, password);
            
            
            if (user.load_user_data()) {
                while (true) {
                    cout << "Choose Operation : " << endl;
                    cout << "1.Deposit" << endl;
                    cout << "2.Withdraw" << endl;
                    cout << "3.Check balance" << endl;
                    cout << "4.View Transaction history" << endl;
                    cout << "5.Exit" << endl << endl;
                    cout << "-----------------------------------------" << endl << endl;
                    cout << "Enter choice : ";
                    cin >> user_choice2;
                    
                    if (user_choice2 == 1) {
                        float amount;
                        cout << "Enter amount to deposit - ";
                        cin >> amount;
                        user.deposit(amount);
                        cout << endl << endl;
                    }
                    else if (user_choice2 == 2) {
                        float amount;
                        cout << "enter amount to withdraw - ";
                        cin >> amount;
                        user.withdraw(amount);
                    }
                    else if (user_choice2 == 3) {
                        user.check_balance();
                    }
                    else if (user_choice2 == 4) {
                        user.transaction_history();
                    }
                    else if (user_choice2 == 5) {
                        cout << "Logging out..." << endl;
                        cout << "Thanks for using secure banking system" << endl;  
                        break; 
                    }
                }
            } else {
                
                cout << "Invalid username or password!" << endl << endl;
            }
        }
        else if (user_choice1 == 3) {
            break;
        }
    }
    
    return 0;
}
