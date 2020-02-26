#ifndef __TYPE_HPP__
#define __TYPE_HPP__


#include <string>
#include <vector>
#include <iostream>
#include <cstdbool>
#include <map>

using namespace std;

class Class; 
class Iskaz;
class Izraz;
class Naredba;
class Variable;
class Class_Variable_Declaration;

class Class {

public:
    Class()
    {}

    Class(string s, vector<Naredba*> v, map<string, vector<Variable*>> tablica_funkcije)
        :_name(s), _v(v), _tablica_funkcije(tablica_funkcije)
    {}
    bool typechack() const;
    ~Class();

    string getName(){
        return _name;
    }

private:
    string _name;
    vector<Naredba*> _v;
    map<string, vector<Variable*>> _tablica_funkcije;

};

class Naredba{
public:
    virtual bool typechack(string class_name, map<string, vector<Variable*>> tablica_funkcije) const = 0;
    virtual ~Naredba(){

    }
};

class Class_Variable_Declaration : public Naredba {
public:
    Class_Variable_Declaration(string mod, string type, vector<string> names)
        :_mod(mod), _type(type), _names(names)
    {}
    bool typechack(string class_name, map<string, vector<Variable*>> tablica_funkcije) const;

    string getType(){
        return _type;
    }

    string getMod(){
        return _mod;
    }


private:
    string _mod, _type;
    vector<string> _names;

};

class Function : public Naredba{
public:
    Function(string mod, string type, string name, vector<Iskaz*> I)
        :_mod(mod), _type(type), _name(name), _I(I)
    {}
    ~Function();
    bool typechack(string class_name, map<string, vector<Variable*>> tablica_funkcije) const;

    string getType(){
        return _type;
    }

    string getName(){
        return _name;
    }
    string getMod(){
        return _mod;
    }

private:
    string _mod, _type;
    string _name;
    vector<Iskaz*> _I;

};



class Iskaz{
public:
    virtual bool typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const =0;
    virtual ~Iskaz(){

    }
};


class Izraz_Iskaz : public Iskaz{
public:
    Izraz_Iskaz(Izraz *e)
        :_e(e)
    {}
    bool typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const;

private:
    Izraz *_e;
};

class Iskaz_Dodela : public Iskaz{
public:
    Iskaz_Dodela(string s, Izraz *e)
        :_name(s),_e(e)
    {}
    bool typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const;

private:
    string _name;
    Izraz *_e;

};

class Deklaracija : public Iskaz{
public:
    Deklaracija(string type, string name)
        :_type(type),_name(name)
    {}
    bool typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const;

private:
    string _type;
    string _name;
};


class DeklaracijaIDodela : public Iskaz{
public:
    DeklaracijaIDodela(string type, string name, Izraz *e)
        :_type(type),_name(name), _e(e)
    {}
    bool typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const;

private:
    string _type, _name;
    Izraz *_e;

};

class Izraz{
public:
    virtual string typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const =0;
    virtual ~Izraz(){

    }
    virtual void print() const =0;
};

class Variable : public Izraz{
public:
    Variable(string name, string type)
        :_name(name),_type(type)
    {}
     string typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const;

     void print() const;

    string getName(){
         return _name;
    }

    string getType(){
         return _type;
    }
private:
    string _name;
    string _type;
};



class Function_call : public Izraz{
public:
    Function_call(string name)
        :_name(name)
    {}
     string typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const;

     void print() const;
private:
    string _name;
};


class Metod_call : public Izraz{
public:
    Metod_call(string name, string metod_name)
        :_name(name), _method_name(metod_name)
    {}
     string typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const;

     void print() const;
private:
    string _name;
    string _method_name;
};

class Class_attribut_call : public Izraz{
public:
    Class_attribut_call(string name, string att_name)
        :_name(name), _att_name(att_name)
    {}
     string typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const;

     void print() const;
private:
    string _name;
    string _att_name;
};


class BinOp: public Izraz{
public:
    BinOp(Izraz *e1, Izraz *e2)
        :_e1(e1), _e2(e2)
    {}
    ~BinOp();
protected:
    Izraz *_e1;
    Izraz *_e2;
private:
    BinOp(const BinOp& z);
    BinOp& operator=(const BinOp& z);
};

class Add : public BinOp{
public:
    Add(Izraz *e1, Izraz *e2)
        :BinOp(e1,e2)
        {}
     string typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const;

     void print() const;
};





class Constant : public Izraz{
public:
    Constant(string s)
        :_type(s)
    {}
    string typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const;


protected:
    string _type;
};

class Integer : public Constant{
public:
    Integer(string s, int num)
        :Constant(s),_num(num)
    {}

    void print() const;

private:
    int _num;
};

class Double : public Constant{
public:
    Double(string s,double d)
        :Constant(s), _d(d)
    {}

    void print() const;

private:
    double _d;
};

class String : public Constant{
public:
    String(string s,string val)
        :Constant(s), _val(val)
    {}

    void print() const;

private:
    string _val;
};



class ClassVariable{
public:
    ClassVariable(){

    }
    ClassVariable(string mod, Variable *V, int indikator)
        :_mod(mod), _V(V),_indikator(indikator)
    {}

    string getName(){
        return _V->getName();
    }

    string getType(){
        return _V->getType();
    }

    string getMod(){
        return _mod;
    }

    int getIndikator(){
        return _indikator;
    }

private:
    string _mod;
    Variable *_V;
    int _indikator;
};



#endif
