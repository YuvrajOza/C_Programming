# include<bits/stdc++.h>

using namespace std;



//  User defined data type('like int') for ticket information collection

struct ticket{
    int tid;
    string ename;
    string tname;
    string dept;
    string issuetitle;
    string issuedesc;
    string priority;
    string status;
    string technician;
    string category;
};


// structure based on node by using data type ticket

struct Node{
    ticket tick;

    Node * next;

    Node(ticket T) : tick(T) , next(nullptr) {} 
};

// undo stack & ticket history

struct change{
    int tid;
    string field;
    string oldvalue;
};

struct StackNode{
    change c;
    StackNode *next;

    StackNode(change x) : c(x) , next(nullptr){}
};

struct QueueNode{
    ticket *data;
    QueueNode *next;
    QueueNode(ticket *t) : data(t), next(nullptr){} 
};

// BST structure for searching ticket

struct TreeNode{
    ticket *data;
    TreeNode *left;
    TreeNode *right;

    TreeNode(ticket *x) : data(x) , left(nullptr),right(nullptr){}

};

// insert BST function

TreeNode * insertBST(TreeNode *root, ticket *t){
    if(root ==nullptr) return new TreeNode(t);
    TreeNode *temp = root;
    if(t->tid < root->data->tid)  root->left = insertBST(root->left,t);
    else if(t->tid > root->data->tid)  root->right = insertBST(root->right,t);

    return root;

}

// search BST function

TreeNode * searchBST(TreeNode *root,int id){
    if(root==nullptr && (root->data->tid == id)) return root;

    if(id< root->data->tid) return searchBST(root->left, id);
    
    return searchBST(root->right, id);

}

// LL based t-list

class ticketlist{
    private:
    Node * head;

    public:
    ticketlist() : head(nullptr){}

    ticket *addticket(ticket t){
        Node * newnode = new Node(t);
        
        if(head==nullptr) head = newnode;

        else {
            Node *temp = head;
            while(temp->next !=NULL){
                temp = temp->next;
            }
            temp->next = newnode;
        }
        return &newnode->tick;
    }
    

    ticket *findticket(int id){
        Node *temp = head;
        
        while(temp!=NULL){
            if(temp->tick.tid == id){
                return &temp->tick;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    void display(){
        if(head==nullptr){
            cout<<"\nNo tickets available!!\n";
         return;
        }
        Node *temp =head;
        while(temp!=NULL){
            cout<<"Ticket ID : "<<temp->tick.tid<<'\n';
            cout<<"Employee Name : "<<temp->tick.ename<<'\n';
            cout<<"Department : "<<temp->tick.dept<<'\n';
            cout<<"Issue Title : "<<temp->tick.issuetitle<<'\n';
            cout<<"Description : "<<temp->tick.issuedesc<<'\n';
            cout<<"Category : "<<temp->tick.category<<'\n';
            cout<<"Priority : "<<temp->tick.priority<<'\n';
            cout<<"Status : "<<temp->tick.status<<'\n';
            cout<<"Technician : "<<temp->tick.technician<<'\n';
            cout<<"-----------------------------\n";
            temp = temp->next;
        }
    }

     // (permenant records saving mechanism)Save all ticket fields to a text file, separated by '|'
    
     void savedata(){
        ofstream file("tickets.txt");
        if(!file){
            cout<<"\nError to read/open file_tickets.txt !!\n";
            return;
        }
        Node *temp = head;
        while(temp!=NULL){
            ticket &t = temp->tick;
            file<<t.tid<<'|'<<t.ename<<'|'<<t.dept<<'|'<<t.issuetitle<<'|'
            <<t.issuedesc<<'|'<<t.category<<'|'<<t.technician<<'|'<<t.priority<<'|'
            <<t.status<<'|';
            temp=temp->next;
        }
        cout<<"Ticket data saved to tickets.txt.\n";
     }

    //  data load function
    void loaddata(TreeNode*root,class ticketqueue & waitingqueue,int & nextid);
};


// Queue
class ticketqueue{
    private:
    QueueNode*front;
    QueueNode*rear;

    public:
    ticketqueue(): front(nullptr), rear(nullptr){}

    void enqueue(ticket *t){
        if(t==nullptr) return;

        // only open/waiting tickets belong in the waiting queue.
        if(t->status !="open" && t->status != "waiting") return;

        // avoid putting the same ticket into the queue more than once.
        QueueNode *chk = front;
        while(chk!=nullptr){
            if(chk->data->tid == t->tid) return;
            chk = chk->next;
        }

        QueueNode * newnode =  new QueueNode(t);
        if(rear==nullptr){
            front = rear = newnode;
            return;
        }
        rear->next = newnode;
        rear = newnode;
    }

};



int main(){
    
return 0;
}
