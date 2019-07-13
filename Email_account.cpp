#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <ctime>
#include <chrono>
using namespace std;

template<typename T> class Vec;
class Name;
class Message;

template<typename T>
class Vec {
public:
    Vec() {_capacity = 0; _size = 0; _vec = NULL;}
    Vec(int n)
    {
        if (n <= 0){
            _capacity = 0;
            _size = 0;
            _vec = NULL;
        }
        if (n > 0){
            _capacity = n;
            _size = n;
            allocate();
        }
    }
    Vec(int n, const T &a)
    {
        if (n <= 0){
            _capacity = 0;
            _size = 0;
            _vec = NULL;
        }
        if (n > 0) {
            _capacity = n;
            _size = n;
            allocate();
            for (int i = 0; i < _capacity; ++i)
                _vec[i] = a;
        }
    }
    Vec(const Vec &orig){
        _capacity = orig._size;
        _size = orig._size;
        allocate();
        for (int i = 0; i < _size; i++){
            _vec[i] = orig._vec[i];
        }
    }
    Vec& operator= (const Vec &rhs){
        if (this == &rhs)
            return *this;
        _capacity = rhs._size;
        _size = rhs._size;
        release();
        allocate();
        for (int i = 0; i < _size; ++i)
            _vec[i] = rhs._vec[i];
        return *this;
    }
    ~Vec() { release(); }

    int capacity() const { return _capacity; }
    int size() const { return _size; }

    T front() const{
        if ( _size <= 0 ){
            T default_obj;
            return default_obj;
        }
        if ( _size > 0 )
            return _vec[0];
    }
    T back() const{
        if ( _size <=0 ){
            T default_obj;
            return default_obj;
        }
        if ( _size > 0 )
            return _vec[_size-1];
    }

    void clear()
    {
        _size = 0;
    }
    void pop_back()
    {
        if (_size > 0)
            _size = _size - 1;
    }

    void erase(int n){
        if (n>=0 && n < _size){
            for (int i=n; i < _size-2; i++){
                _vec[i] = _vec[i+1];
            }
            _size = _size-1;
        }
        else
            return;
    }

    void push_back(const T &a)
    {
        if (_size < _capacity){
            _vec[_size] = a;
            _size = _size + 1;
        }

        if (_size == _capacity){
            if (_capacity == 0)
                _capacity = 1;
            else if (_capacity > 0)
                _capacity = _capacity * 2;
            T *oldvec = _vec;
            allocate();
            if (oldvec != NULL){
                for (int i = 0; i < _size; ++i)
                    _vec[i] = oldvec[i];
                delete [] oldvec;
            }
            _vec[_size] = a;
            _size = _size + 1;
        }
    }

    T* at(int n){
        if (n >= 0 && n < _size){
            T *pvec = &_vec[n];
            return pvec;
        }
        if (n < 0 || n >= _size){
            T *pnull = NULL;
            return pnull;
        }
    }

    T& operator[] (int n){
        return _vec[n];
    }

    const T& operator [](int n) const{
        if (n >= 0 && n < _size)
            return _vec[n];
        if (n < 0 || n >= _size){
            T *pnull = NULL;
            return *pnull;
        }
    }

private:
    void allocate()
    {
        if (_capacity <= 0)
            _vec = NULL;
        if (_capacity > 0)
            _vec = new T[_capacity];
    }
    void release()
    {
        if (_vec != NULL){
            delete [] _vec;
        }
        _vec = NULL;
    }

    int _capacity;
    int _size;
    T * _vec;
};


class Name{
public:
    Name(){}
    Name(string s) { set(s); }

    void set(string s){
        const string _domain = "@qc.cuny.edu";
        istringstream iss(s);
        iss >> _name;
        if (_name == ""){
            _address = " ";
        }
        else
            _address = _name + _domain;
    }

    string name() const { return _name; }
    string address() const { return _address; }
private:
    string _name;
    string _address;
};

bool operator==(const Name &n1, const Name &n2){
    return ( n1.address() == n2.address() );
}

bool operator<(const Name &n1, const Name &n2){
    return ( n1.address() < n2.address() );
}


class Message {
public:
    Message() { _date = 0; };
    Message(string f){
        _from.set(f);
        _date = 0;
    }
    Message(string f, string t){
        _from.set(f);
        _to.set(t);
        _date = 0;
    }
    Message(string f, string t, string s){
        _subject = s;
        _from.set(f);
        _to.set(t);
        _date = 0;
    }
    Message(string f, string t, string s, string txt){
        _text = txt;
        _subject = s;
        _from.set(f);
        _to.set(t);
        _date = 0;
    }

    Message(const Message &orig){
        _from = orig.from();
        _to = orig.to();
        _subject = orig.subject();
        _text = orig.text();
        _date = orig._date;
    }

    const Name& from() const { return _from; }
    const Name& to() const { return _to; }
    string subject() const { return _subject; }
    string text() const { return _text; }

    string date() const{
        if(_date > 0)
            return std::ctime(&_date);
        else
            return "";
    }
    void send() { setDate(); }

    void setRecipient(string t) { _to.set(t); }
    void setSubject(string s) { _subject = s; }
    void setText(string txt) { _text = txt; }


    void prependText(string t){
        _text = t + " " + _text;
    }
    void appendText(string t){
        _text = _text + " " + t;
    }
    void print() const{
        cout << "From: " << _from.name() << "   <" << _from.address() << ">" << endl;
        cout << "To: " << _to.name() << "   <" << _to.address() << ">" << endl;
        cout << "Subject: " << _subject << endl;
        cout << "Date: " << date() << endl;
        cout << _text << endl;
        cout << endl;
    }

    ostream &operator<< (ostream &os){
        os << "From: " << _from.name() << "   <" << _from.address() << ">" << endl;
        os << "To: " << _to.name() << "   <" << _to.address() << ">" << endl;
        os << "Subject: " << _subject << endl;
        os << "Date: " << date() << endl;
        os << _text << endl;
    }

private:
    void setDate(){
        auto t_now = std::chrono::system_clock::now();
        _date = std::chrono::system_clock::to_time_t(t_now);
    }

    Name _from;
    Name _to;
    string _subject;
    string _text;
    time_t _date;
};


class BaseFolder;
class EmailAccount;
class Drafts;
class ISP;

class BaseFolder{
public:
    virtual ~BaseFolder();

    int size() const { return _msg.size(); }

    void display() const;
    void erase(int n){
        if ( n < 1 || n >= _msg.size() )
            return;
        if ( n >= 1 && n < _msg.size() ){
            delete _msg[n];
            _msg.erase(n);
        }
    }
    void forward(int n) const;
    void print(int n) const{
        if ( n<1 || n>=_msg.size() )
            return;
        if ( n>=1 && n<_msg.size() ){
            const Message *a = _msg[n];
            a->print();
        }
    }
    void receive (const Message *m) { _msg.push_back(m); }
    void reply(int n) const;

protected:
    BaseFolder(EmailAccount *ac) : _ac(ac){
        _msg.push_back(NULL);
    }

    virtual string type() const {};
    virtual const Name& tofrom(const Message *m) const {};

    Vec<const Message*> _msg;
    EmailAccount *_ac;

private:
    BaseFolder( const BaseFolder &orig){
        int length = orig._msg.size();
        for (int i = 0; i < length; i++)
            this->_msg[i] = new Message;
        for (int i = 0; i < length; i++)
            this->_msg[i] = orig._msg[i];
    }
};


class EmailAccount{
public:
    EmailAccount(string s);
    ~EmailAccount();

    const Name& owner() const { return _owner; }
    Drafts& drafts() { return *_drafts; }
    BaseFolder& in() { return *_in; }
    BaseFolder& out() { return *_out; }

    void send(Message *m);
    void receive(Message *m);
    void insert(Message *m);

private:
    EmailAccount(EmailAccount &orig);

    Name _owner;
    Drafts *_drafts;
    BaseFolder * _in;
    BaseFolder * _out;
};

BaseFolder::~BaseFolder(){
    for (int i=0; i < _msg.size(); i++)
        delete _msg[i];
}

void BaseFolder::display() const {
    cout << _ac->owner().name() << " " << type() << endl;
    if (_msg.size() <= 1){
        cout << "no messages to display" << endl;
    }
    for (int i =1; i < _msg.size(); ++i){
        const Message *m = _msg[i];
        const Name &tmp = tofrom(m);
        cout << i << tmp.name() << m->subject();
    }
}

void BaseFolder::forward(int n) const {
    if ( n < 1 || n >= _msg.size() )
        return;
    if ( n>=1 && n < _msg.size() ){
        const Message *m = _msg[n];
        string fwd_subject = "Fwd: " + m->subject();
        Message *ptr = new Message(_ac->owner().name(), "", fwd_subject, m->text());
        _ac->insert( ptr );
    }
}

void BaseFolder::reply(int n) const {
    if ( n < 1 || n >= _msg.size() )
        return;
    if ( n >= 1 && n < _msg.size() ){
        const Message *m = _msg[n];
        string subject = "Re: " + m->subject();
        const Name&tmp = tofrom(m);
        Message *ptr = new Message(_ac->owner().name(), tmp.name(), subject, m->text());
        _ac->insert( ptr );
    }
}

class Inbox : public BaseFolder{
public:
    Inbox(EmailAccount *ac);
protected:
    virtual string type() const { return "Inbox"; }
    virtual const Name& tofrom(const Message *m) const { return m->from(); }
};

class Outbox : public BaseFolder{
public:
    Outbox(EmailAccount *ac);
protected:
    virtual string type() const { return "Outbox"; }
    virtual const Name& tofrom(const Message *m) const { return m->to(); }
};

Inbox::Inbox(EmailAccount *ac): BaseFolder(ac){}
Outbox::Outbox(EmailAccount *ac): BaseFolder(ac){}

class ISP{
public:
    static void addAccount(EmailAccount *e){
       _accounts[e->owner()]=e;
    }

    static void send(Message *m){
        EmailAccount *ac;
        ac = _accounts[m->to()];
        if (ac == NULL){
            cout << "Delivery failed, to recipient: " << m->to().name() << endl;
            return;
        }
        Message *clone = new Message(*m);
        ac->receive(clone);
        return;
    }

private:
    ISP() {};
    static map<Name, EmailAccount*> _accounts;
};

map<Name, EmailAccount*> ISP::_accounts = {};

void EmailAccount::receive(Message *m){
    _in->receive(m);
}

void EmailAccount::send(Message *m){
    _out->receive(m);
    ISP::send(m);
}


class Drafts{
public:
    Drafts(EmailAccount *ac){
        _ac = ac;
        _newKey = 0;
        _drafts[0] = NULL;
    }
    ~Drafts(){
        map<int, Message*>::iterator m_it;
        for (m_it = _drafts.begin(); m_it != _drafts.end(); ++m_it)
            delete m_it->second;
    }

    void display() const{
        cout << _ac->owner().name() << " drafts: " << endl;
        if (_drafts.size() <= 1)
            cout << "no messages to display" << endl;
        map<int, Message*>::const_iterator mit;
        for (mit = _drafts.begin(); mit != _drafts.end(); ++mit){
            if (mit->first == 0) continue;
            Message *ptr = mit->second;
            cout << mit->first << ptr->to().name() << ptr->subject();
        }
    }
    void send(int n){
        Message *ptr = _drafts[n];
        if (ptr == NULL){
            cout << "Message not found";
            return;
        }
        if (ptr->to().address() == ""){
            cout << "You must specify a recipient" << endl;
            return;
        }
        ptr->send();
        _drafts.erase(n);
        _ac->send( ptr );
    }
    void erase(int n){
        delete _drafts[n];
        _drafts.erase(n);
    }

    Message* addDraft(){
        int key = newKey();
        Message *ptr = new Message( _ac->owner().name() );
        _drafts[key] = ptr;
        return ptr;
    }
    Message* addDraft(Message *m){
        int key = newKey();
        _drafts[key] = m;
        return m;
    }
    Message* getDraft(int n) {
        if (_drafts.count(n) == 0) {
            Message *pnull = NULL;
            return pnull;
        }
        return _drafts[n];
    }
    Message* operator[] (int n) { return _drafts[n]; }

private:
    Drafts(Drafts &orig){
        map<int, Message*>::iterator mit;
        for (mit = this->_drafts.begin(); mit != this->_drafts.end(); ++mit)
            mit->second = new Message;
        for (mit = orig._drafts.begin(); mit != orig._drafts.end(); ++mit){
            int val = mit->first;
            this->_drafts[val] = mit->second;
        }
    }

    int _newKey;
    int newKey() { return ++_newKey; }

    map<int, Message*> _drafts;
    EmailAccount *_ac;
};

EmailAccount::EmailAccount(string s){
    _owner.set(s);
    _drafts = new Drafts(this);
    _in = new Inbox(this);
    _out = new Outbox(this);
    ISP::addAccount(this);
}

void EmailAccount::insert(Message *m){
    _drafts->addDraft(m);
}

EmailAccount::EmailAccount(EmailAccount &orig){
    _owner = orig._owner;
//    EmailAccount *e = &orig;
//    Drafts *dnew = new Drafts(e);
//    _drafts = dnew;
//    BaseFolder *innew = new Inbox(e);
//    _in = innew;
//    BaseFolder *outnnew = new Outbox(e);
//    _out = outnew;
    _drafts = orig._drafts;
    _in = orig._in;
    _out = orig._out;
}

EmailAccount::~EmailAccount(){
    delete _drafts;
    delete _in;
    delete _out;
}


int main(){
    string from="fromaccount";
    string to="toaccount";
    string subject="sending an email";
    string body="hello world!!";
    Message newMsg(from,to, subject, body);
    newMsg.send();
    Vec<Message> myMessages;
    myMessages.push_back(newMsg);
    myMessages.push_back(newMsg);
    myMessages[0].print();
    myMessages.erase(0);
    myMessages[1].print();
    cout << myMessages.size() << " " << myMessages.capacity() << endl;
    Name n1, n2;
    n1.set("bbb");
    n2.set("");
    if (n1 == n2)
        cout << "yes\n";
    else
        cout << "no\n";
    EmailAccount e("Sakib");
    e.drafts().send(0);
    return 0;
}
