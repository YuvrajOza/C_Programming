#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

// ticket 

struct Ticket {
    int ticketID;
    string employeeName;
    string department;
    string issueTitle;
    string description;
    string category;
    string priority;
    string status;
    string technician;
};

// linked list for ticket


struct Node {
    Ticket ticket;
    Node* next;
    Node(Ticket t) : ticket(t), next(nullptr) {}
};

// undo stack & ticket history

struct change {
    int ticketID;
    string field;
    string oldvalue;
};

struct StackNode {
    change data;
    StackNode* next;
    StackNode(change c) : data(c), next(nullptr) {}
};

// Queue stores pointers so updates are reflected in the master ticket

struct QueueNode {
    Ticket* data;
    QueueNode* next;
    QueueNode(Ticket* t) : data(t), next(nullptr) {}
};

// BST for searching ticket struct

struct TreeNode {
    Ticket* data;
    TreeNode* left;
    TreeNode* right;
    TreeNode(Ticket* t) : data(t), left(nullptr), right(nullptr) {}
};



TreeNode* insertBST(TreeNode* root, Ticket* t) {
    if (root == nullptr) {
        return new TreeNode(t);
    }

    if (t->ticketID < root->data->ticketID) {
        root->left = insertBST(root->left, t);
    } else if (t->ticketID > root->data->ticketID) {
        root->right = insertBST(root->right, t);
    }

    return root;
}

TreeNode* searchBST(TreeNode* root, int id) {
    if (root == nullptr || root->data->ticketID == id) {
        
        return root;
    }

    if (id < root->data->ticketID) {
         

         return searchBST(root->left, id);
    }

    return searchBST(root->right, id);
}

// linked list based ticket list
class TicketList {
private:
    Node* head;

public:
    TicketList() : head(nullptr) {}

    Ticket* addticket(Ticket t) {
        Node* newNode = new Node(t);
        
        if (head == nullptr) {
            
            head = newNode;
        }  
        else {
              Node* temp = head;
  
               while (temp->next != nullptr) {
                     temp = temp->next;
               }

            temp->next = newNode;
        }

        return &newNode->ticket;
    }

    Ticket* findticket(int id) {
        Node* temp = head;

        while (temp != nullptr) {
            if (temp->ticket.ticketID == id) {
                return &temp->ticket;
            }
            temp = temp->next;
        }

        return nullptr;
    }

    void displaytickets() {
        if (head == nullptr) {
            cout << "No tickets available.\n";
            return;
        }

        Node* temp = head;

        while (temp != nullptr) {
            cout << "\nTicket ID: " << temp->ticket.ticketID << "\n";
            cout << "Employee Name: " << temp->ticket.employeeName << "\n";
            cout << "Department: " << temp->ticket.department << "\n";
            cout << "Issue Title: " << temp->ticket.issueTitle << "\n";
            cout << "Description: " << temp->ticket.description << "\n";
            cout << "Category: " << temp->ticket.category << "\n";
            cout << "Priority: " << temp->ticket.priority << "\n";
            cout << "Status: " << temp->ticket.status << "\n";
            cout << "Technician: " << temp->ticket.technician << "\n";
            cout << "-----------------------------\n";
            temp = temp->next;
        }
    }

    // (permenant records saving mechanism)Save all ticket fields to a text file, separated by '|'
    void saveData() {
        ofstream file("tickets.txt");

        if (!file) {
            cout << "Unable to open tickets.txt for saving.\n";
            return;
        }

        Node* temp = head;
        while (temp != nullptr) {
            Ticket& t = temp->ticket;
            file << t.ticketID << '|'
                 << t.employeeName << '|'
                 << t.department << '|'
                 << t.issueTitle << '|'
                 << t.description << '|'
                 << t.category << '|'
                 << t.priority << '|'
                 << t.status << '|'
                 << t.technician << '\n';
            temp = temp->next;
        }

        cout << "Ticket data saved to tickets.txt.\n";
    }

    // Load saved tickets and rebuild the BST and waiting queue
    void loadData(TreeNode*& root, class TicketQueue& waitingQueue, int& nextID);
};

// Queue
class TicketQueue {
private:
    QueueNode* front;
    QueueNode* rear;

public:
    TicketQueue() : front(nullptr), rear(nullptr) {}

    void enqueue(Ticket* t) {
        if (t == nullptr) return;

        // Only Open/Waiting tickets belong in the waiting queue.
        if (t->status != "Open" && t->status != "Waiting") return;

        // Avoid putting the same ticket into the queue more than once.
        QueueNode* check = front;
        while (check != nullptr) {
            if (check->data->ticketID == t->ticketID) return;
            check = check->next;
        }

        QueueNode* newNode = new QueueNode(t);

        if (rear == nullptr) {
            front = rear = newNode;
            return;
        }

        rear->next = newNode;
        rear = newNode;
    }

    void processnext() {
        if (front == nullptr) {
            cout << "No tickets waiting.\n";
            return;
        }

        QueueNode* temp = front;
        cout << "\nProcessing Ticket ID: " << temp->data->ticketID << '\n';
        cout << "Employee: " << temp->data->employeeName << '\n';
        cout << "Issue: " << temp->data->issueTitle << '\n';

        if (temp->data->status == "Open" || temp->data->status == "Waiting") {
            temp->data->status = "In Progress";
        }

        front = front->next;
        if (front == nullptr) {
            rear = nullptr;
        }

        delete temp;
        cout << "Ticket moved to In Progress.\n";
    }

    void removeTicket(int id) {
        QueueNode* temp = front;
        QueueNode* previous = nullptr;

        while (temp != nullptr) {
            if (temp->data->ticketID == id) {
                if (previous == nullptr) {
                    front = temp->next;
                } else {
                    previous->next = temp->next;
                }

                if (temp == rear) {
                    rear = previous;
                }

                delete temp;
                return;
            }

            previous = temp;
            temp = temp->next;
        }
    }

    void displayQueue() {
        if (front == nullptr) {
            cout << "Waiting queue is empty.\n";
            return;
        }

        QueueNode* temp = front;
        cout << "\n===== WAITING QUEUE =====\n";

        while (temp != nullptr) {
            cout << "Ticket ID: " << temp->data->ticketID
                 << " | Employee: " << temp->data->employeeName
                 << " | Priority: " << temp->data->priority << '\n';
            temp = temp->next;
        }
    }
};

// Undo stack
class UndoStack {
private:
    StackNode* top;

public:
    UndoStack() : top(nullptr) {}

    void push(change c) {
        StackNode* newNode = new StackNode(c);
        newNode->next = top;
        top = newNode;
    }

    bool pop(change& c) {
        if (top == nullptr) {
            return false;
        }

        StackNode* temp = top;
        c = temp->data;
        top = top->next;
        delete temp;
        return true;
    }

    bool isEmpty() {
        return top == nullptr;
    }
};

// Now TicketQueue is fully defined, so loading can rebuild the index and queue.
void TicketList::loadData(TreeNode*& root, TicketQueue& waitingQueue, int& nextID) {
    ifstream file("tickets.txt");

    if (!file) {
        cout << "No saved data found. Starting with an empty system.\n";
        return;
    }

    string line;
    int loaded = 0;

    while (getline(file, line)) {
        stringstream ss(line);
        string part;
        vector<string> fields;

        while (getline(ss, part, '|')) {
            fields.push_back(part);
        }

        if (fields.size() != 9) continue;

        try {
            Ticket t;
            t.ticketID = stoi(fields[0]);
            t.employeeName = fields[1];
            t.department = fields[2];
            t.issueTitle = fields[3];
            t.description = fields[4];
            t.category = fields[5];
            t.priority = fields[6];
            t.status = fields[7];
            t.technician = fields[8];

            // Avoid inserting the same ID twice in the current list.
            if (findticket(t.ticketID) != nullptr) continue;

            Ticket* savedTicket = addticket(t);
            root = insertBST(root, savedTicket);
            waitingQueue.enqueue(savedTicket);

            if (t.ticketID >= nextID) nextID = t.ticketID + 1;
            loaded++;
        } catch (...) {
            // Ignore malformed records and continue loading other records.
        }
    }

    cout << loaded << " ticket(s) loaded.\n";
}

void showTicket(Ticket* t) {
    if (t == nullptr) {
        cout << "Ticket not found.\n";
        return;
    }

    cout << "\nTicket ID: " << t->ticketID << '\n';
    cout << "Employee Name: " << t->employeeName << '\n';
    cout << "Department: " << t->department << '\n';
    cout << "Issue Title: " << t->issueTitle << '\n';
    cout << "Description: " << t->description << '\n';
    cout << "Category: " << t->category << '\n';
    cout << "Priority: " << t->priority << '\n';
    cout << "Status: " << t->status << '\n';
    cout << "Technician: " << t->technician << '\n';
}

void createTicket(TicketList& tickets, TreeNode*& root, TicketQueue& waitingQueue,
                  int& nextID, vector<change>& history) {
    Ticket t;
    t.ticketID = nextID++;

    cout << "\n===== CREATE SUPPORT TICKET =====\n";
    cout << "Enter employee name: ";
    getline(cin >> ws, t.employeeName);
    cout << "Enter department: ";
    getline(cin, t.department);
    cout << "Enter issue title: ";
    getline(cin, t.issueTitle);
    cout << "Enter issue description: ";
    getline(cin, t.description);

    int input;
    cout << "\nSelect category:\n"
         << "1. Hardware\n2. Software\n3. Network\n4. Account / Security\n"
         << "Enter choice: ";
    cin >> input;

    switch (input) {
        case 1: t.category = "Hardware"; break;
        case 2: t.category = "Software"; break;
        case 3: t.category = "Network"; break;
        case 4: t.category = "Account / Security"; break;
        default: t.category = "Other"; break;
    }

    cout << "\nSelect priority:\n1. Low\n2. Medium\n3. High\n4. Critical\n"
         << "Enter choice: ";
    cin >> input;

    switch (input) {
        case 1: t.priority = "Low"; break;
        case 2: t.priority = "Medium"; break;
        case 3: t.priority = "High"; break;
        case 4: t.priority = "Critical"; break;
        default: t.priority = "Medium"; break;
    }

    t.status = "Open";
    t.technician = "Unassigned";

    Ticket* savedTicket = tickets.addticket(t);
    root = insertBST(root, savedTicket);
    waitingQueue.enqueue(savedTicket);

    history.push_back({t.ticketID, "created", "Ticket created"});
    cout << "\nTicket created successfully. Ticket ID: " << t.ticketID << '\n';
}

void updateTicketStatus(TicketList& tickets, TicketQueue& waitingQueue,
                        UndoStack& undo, vector<change>& history) {
    int id, input;
    cout << "Enter ticket ID: ";
    cin >> id;

    Ticket* t = tickets.findticket(id);
    if (t == nullptr) {
        cout << "Ticket not found.\n";
        return;
    }

    cout << "Current status: " << t->status << '\n';
    cout << "1. Open\n2. Waiting\n3. In Progress\n4. Resolved\n5. Closed\n";
    cout << "Choose new status: ";
    cin >> input;

    string newStatus;
    switch (input) {
        case 1: newStatus = "Open"; break;
        case 2: newStatus = "Waiting"; break;
        case 3: newStatus = "In Progress"; break;
        case 4: newStatus = "Resolved"; break;
        case 5: newStatus = "Closed"; break;
        default:
            cout << "Invalid status choice.\n";
            return;
    }

    change c = {id, "status", t->status};
    undo.push(c);
    history.push_back(c);
    t->status = newStatus;

    if (t->status == "Open" || t->status == "Waiting") {
        waitingQueue.enqueue(t);
    } else {
        waitingQueue.removeTicket(t->ticketID);
    }

    cout << "Ticket status updated to " << t->status << ".\n";
}

void assignTechnician(TicketList& tickets, UndoStack& undo, vector<change>& history) {
    int id;
    cout << "Enter ticket ID: ";
    cin >> id;

    Ticket* t = tickets.findticket(id);
    if (t == nullptr) {
        cout << "Ticket not found.\n";
        return;
    }

    cout << "Current technician: " << t->technician << '\n';
    cout << "Enter technician name: ";
    string technician;
    getline(cin >> ws, technician);

    change c = {id, "technician", t->technician};
    undo.push(c);
    history.push_back(c);
    t->technician = technician;

    cout << "Technician assigned successfully.\n";
}

void undoLastChange(TicketList& tickets, TicketQueue& waitingQueue,
                    UndoStack& undo, vector<change>& history) {
    change c;
    if (!undo.pop(c)) {
        cout << "Nothing to undo.\n";
        return;
    }

    Ticket* t = tickets.findticket(c.ticketID);
    if (t == nullptr) {
        cout << "Ticket not found; cannot undo this change.\n";
        return;
    }

    if (c.field == "status") {
        string currentValue = t->status;
        t->status = c.oldvalue;
        if (t->status == "Open" || t->status == "Waiting") {
            waitingQueue.enqueue(t);
        } else {
            waitingQueue.removeTicket(t->ticketID);
        }
        history.push_back({c.ticketID, "undo status", currentValue});
        cout << "Status restored to " << t->status << ".\n";
    } else if (c.field == "technician") {
        string currentValue = t->technician;
        t->technician = c.oldvalue;
        history.push_back({c.ticketID, "undo technician", currentValue});
        cout << "Technician restored to " << t->technician << ".\n";
    } else {
        cout << "This change type cannot be undone.\n";
    }
}

void viewTicketHistory(const vector<change>& history) {
    if (history.empty()) {
        cout << "No ticket history available.\n";
        return;
    }

    cout << "\n===== TICKET HISTORY =====\n";
    for (const change& c : history) {
        cout << "Ticket ID: " << c.ticketID
             << " | Action: " << c.field
             << " | Previous/details: " << c.oldvalue << '\n';
    }
}

int main() {
    TicketList tickets;
    TicketQueue waitingQueue;
    
     UndoStack undo;
    
     TreeNode* root = nullptr;
    vector<change> history;
    
    int nextID = 1001;

    int input = 0;

     tickets.loadData(root, waitingQueue, nextID);

    do {
        cout << "\n====================================================\n";
        cout << "      IT SUPPORT TICKET MANAGEMENT SYSTEM\n";
        cout << "====================================================\n";
        cout << "                     DASHBOARD\n";
        cout << "----------------------------------------------------\n";
        cout << " 1.  Create Ticket for Employee\n";
        cout << " 2.  Display All Tickets\n";
        cout << " 3.  Search Ticket by ID\n";
        cout << " 4.  View Waiting Queue\n";
        cout << " 5.  Process Next Waiting Ticket\n";
        cout << " 6.  Update Ticket Status\n";
        cout << " 7.  Assign Technician\n";
        cout << " 8.  Undo Last Change\n";
        cout << " 9.  View Ticket History\n";
        cout << " 10. Save Data\n";
        cout << " 11. Exit\n";
        cout << "----------------------------------------------------\n";
        cout << "Enter choice: ";
        cin>>input;

        switch (input) {
            case 1:
                createTicket(tickets, root, waitingQueue, nextID, history);
                break;
            case 2:
                tickets.displaytickets();
                break;
            case 3: {
                int id;
                cout << "Enter ticket ID to search: ";
                cin >> id;
                TreeNode* result = searchBST(root, id);
                if (result != nullptr) showTicket(result->data);
                else cout << "Ticket not found.\n";
                break;
            }
            case 4:
                waitingQueue.displayQueue();
                break;
            case 5:
                waitingQueue.processnext();
                break;
            case 6:
                updateTicketStatus(tickets, waitingQueue, undo, history);
                break;
            case 7:
                assignTechnician(tickets, undo, history);
                break;
            case 8:
                undoLastChange(tickets, waitingQueue, undo, history);
                break;
            case 9:
                viewTicketHistory(history);
                break;
            case 10:
                tickets.saveData();
                break;
            case 11:
                tickets.saveData();
                cout << "Exiting program. Goodbye!\n";
                break;
            default:
                cout << "\n!! Invalid input, please try again !!\n";
                break;
        }
    } while (input != 11);

    return 0;
}
