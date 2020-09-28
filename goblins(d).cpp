#include <iostream>
using namespace  std;
struct Node{
    Node* next;
    int data;
    Node(Node* next1, int data1){
        next = next1;
        data = data1;
    }
};

struct queue{
    Node* tail = NULL;
    Node* mid = NULL;
    Node* head = NULL;
    int size = 0;
    void push_back(int x){
        if(head == NULL && tail == NULL){
            Node* node = new Node(tail, x);
            head = node;
            mid = node;
            tail = node;
        }
        else{
            Node* node = new Node(NULL, x);
            tail->next = node;
            tail = node;
        }
        if(size%2==0 && size!=0){
            mid = mid->next;
        }
        size++;
    }
    int front(){
        if (size!=0){
            return head->data;
        }
    }
    void push_mid(int x){
        if(head == NULL && tail == NULL){
            Node* node = new Node(tail, x);
            head = node;
            mid = node;
            tail = node;
        }
        else{
            Node* node = new Node(mid->next, x);
            mid->next = node;
            if(size%2==0){
                mid = mid->next;
            }
        }
        size++;
    }
    void pop(){
        if(size){
            Node* p = head->next;
            delete head;
            head = p;
            if(size%2==0){
                mid = mid->next;
            }
            size--;
        }
    }
};



int main() {
    int n;
    queue q;
    cin >> n;
    for (int i=0; i<n; i++){
        char s;
        cin >> s;
        if(s == '+'){
            int c;
            cin >> c;
            q.push_back(c);
        }
        if(s == '-'){
            cout << q.front() << endl;
            q.pop();
        }
        if(s == '*'){
            int c;
            cin >> c;
            q.push_mid(c);
        }
    }
    return 0;
}
