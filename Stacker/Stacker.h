#ifndef Stacker_h
#define Stacker_h
// Seams best one
template <class type>
struct node{
    type* data; // stored type data
    node<type>* next; // connection to next node
};

template <class type>
class Stacker{
    node<type>* ptr; // pointer to first element of a stack
    node<type>* hdl; // handle using to add or remove nodes
    unsigned long long stacksize; // size of a stack
public:
    // Stacker<type> stack(new type());
    // data that means null
    Stacker(type *null_data){
        this->ptr = new node<type>;
        this->ptr->data = null_data;
        this->ptr->next = this->ptr;
        this->stacksize = 0;
    }

    ~Stacker(){
        this->clear();
        delete this->ptr->data; // rm null_data
        delete this->ptr;       // rm ptr
    }

    // stack.push(type_data)
    void push(const type& data){
        this->hdl = new node<type>;
        this->hdl->data = new type(data);
        this->hdl->next = this->ptr->next;
        this->ptr->next = this->hdl;
        
        this->stacksize++;
        this->hdl = nullptr; // keep clean
    }

    // stack.pop()
    void pop(){
        if(this->isEmpty()) return;

        this->hdl = this->ptr->next;
        this->ptr->next = this->hdl->next;
        delete this->hdl->data;
        delete this->hdl;

        this->stacksize--;
        this->hdl = nullptr; // keep clean
    }

    void clear(){
        while(!this->isEmpty()) this->pop();
    }

    // const type* = stack.top()
    const type* top() const{
        if(this->isEmpty()) return this->ptr->data; // stacksize == 0
        else return this->ptr->next->data; // stacksize > 0 
    }

    const unsigned long long& size() const{
        return this->stacksize;
    }

    const bool isEmpty() const{
        if(this->ptr->next == this->ptr) return true;
        else return false;
    }
};

#endif