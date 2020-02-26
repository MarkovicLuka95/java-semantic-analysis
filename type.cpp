#include "type.hpp"



extern map<string, vector<ClassVariable*>> table;


Class::~Class(){
    for(unsigned i= 0; i< _v.size(); i++){
        delete _v[i];
    }
}

bool Class::typechack() const{
    bool tacna_klasa = true;
    for (unsigned i= 0; i< _v.size(); i++){
        if(_v[i]->typechack(_name,_tablica_funkcije)==false){
            tacna_klasa=false;
        }
    }
    if(tacna_klasa)
        return true;
    return false;
}

bool Class_Variable_Declaration::typechack(string class_name, map<string, vector<Variable*>> tablica_funkcije) const{

    map<string, vector<ClassVariable*>>::iterator tmp;

    if(_type != "int" && _type != "double" && _type != "string" && _type != "void"){
        tmp=table.find(_type);
        if(tmp==table.end()){
            cout << "Klasa " << _type <<" nije definisana" << endl;
            return false;
        }
    }

    return true;
}




Function::~Function(){
    for(unsigned i=0; i<_I.size(); i++){
        delete _I[i];
    }

}

bool Function::typechack(string class_name, map<string, vector<Variable*>> tablica_funkcije) const{
    bool semanticki_tacna_fja = true;
    for (unsigned i= 0; i<_I.size(); i++){
        if(_I[i]->typechack(class_name,_name, tablica_funkcije)==false){
            semanticki_tacna_fja = false;
        }
    }
    if(semanticki_tacna_fja)
        return true;
    return false;
}

bool Izraz_Iskaz::typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const{
    if(_e->typechack(class_name, function_name, tablica_funkcije)=="false")
        return false;
    return true;
}

bool Iskaz_Dodela::typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const{

    string e_type= _e->typechack(class_name, function_name, tablica_funkcije);

    if(e_type=="false")
        return false;



    for(unsigned i=0; i<tablica_funkcije[function_name].size(); i++){

        if(_name == tablica_funkcije[function_name][i]->getName()){
            if(tablica_funkcije[function_name][i]->getType()==e_type)
                return true;
            else
                if((tablica_funkcije[function_name][i]->getType()== "double" && e_type == "int") || (tablica_funkcije[function_name][i]->getType()== "int" && e_type == "double"))
                    return true;
                else{
                    cout << "u funkciji " << function_name << endl;
                    cout << _name << " = ";
                    _e->print();
                    cout << endl;
                    cout << "promenljiva " << _name << " nije typa " << e_type << " vec " <<
                    tablica_funkcije[function_name][i]->getType() << endl;
                    return false;
                }
        }

    }



    for(unsigned i=0; i<table[class_name].size(); i++){

        if(_name == table[class_name][i]->getName() && table[class_name][i]->getIndikator()==0){
            if(table[class_name][i]->getType()==e_type){

                return true;
            }
            else
                if((table[class_name][i]->getType()== "double" && e_type == "int") ||
                    (table[class_name][i]->getType()== "int" && e_type == "double")){
                    return true;
                }
                else{
                    cout << "u funkciji " << function_name << endl;
                    cout << _name << " = ";
                    _e->print();
                    cout << endl;
                    cout << "promenljiva " << _name << " nije typa " << e_type << " vec " <<
                    table[class_name][i]->getType() << endl;
                    return false;
                }
        }
    }

    cout << "u funkciji " << function_name << endl;
    cout << "promenljiva " << _name << " nije deklarisana\n" << endl;



    return false;
}


bool Deklaracija::typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const{
    return true;
}


bool DeklaracijaIDodela::typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const{
    string e_type = _e->typechack(class_name, function_name, tablica_funkcije);
    if(_type == e_type)
        return true;

    if((_type=="double" && e_type=="int") || (_type=="int" && e_type=="double"))
        return true;

    cout << "u funkciji " << function_name << endl;
    cout << _type << ' ' << _name << '=';
    _e->print();
    cout << endl;
    cout << _type <<" i " << e_type << " nisu kompatiblini\n"<< endl;

    return false;
}

string Variable::typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const{

    return _type;
}

string Function_call::typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const{
    for (unsigned i=0; i<table[class_name].size(); i++){
        if(_name == table[class_name][i]->getName() &&
            table[class_name][i]->getIndikator()==1){
                return table[class_name][i]->getName();
            }

    }

    cout << "Funkcija " << _name << " nije deklarisana"<< endl;
    return "Not decleard";

}

string Metod_call::typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const{

    map<string, vector<ClassVariable*>>::iterator tmp;
    string pom_class_name;


    for(unsigned i=0; i<tablica_funkcije[function_name].size(); i++){
        if(_name == tablica_funkcije[function_name][i]->getName()){
            pom_class_name=tablica_funkcije[function_name][i]->getType();
            tmp=table.find(pom_class_name);
            if(tmp==table.end()){
                cout << "Klasa " << pom_class_name << " nije definisana" << endl;
                return "Not decleard";
            }
            else {
                for (unsigned i=0; i<table[pom_class_name].size(); i++){
                    if(_method_name==table[pom_class_name][i]->getName()
                        && 1==table[pom_class_name][i]->getIndikator()){
                        if(table[pom_class_name][i]->getMod()=="public"){
                            return table[pom_class_name][i]->getType();

                        }
                        cout << "Metod " << _method_name << " je privatan"<<endl;
                        return "private variable";
                    }
                }
                cout << "Metod " << _method_name << " nije deklarisan u klasi " << pom_class_name << endl;
                return "Not decleard";
            }
        }
    }

    for(unsigned i=0; i<table[class_name].size(); i++){
        if(_name == table[class_name][i]->getName()){
            pom_class_name=table[class_name][i]->getType();
            tmp=table.find(pom_class_name);
            if(tmp==table.end()){
                cout << "Klasa " << pom_class_name << " nije definisana" << endl;
                return "Not decleard";
            }
            else {
                for (unsigned i=0; i<table[pom_class_name].size(); i++){
                    if(_method_name==table[pom_class_name][i]->getName()
                        && 1==table[pom_class_name][i]->getIndikator()){
                        if(table[pom_class_name][i]->getMod()=="public"){
                            return table[pom_class_name][i]->getType();

                        }
                        cout << "Metod " << _method_name << " je privatan"<<endl;
                        return "private variable";
                    }

                }
                cout << "Metod " << _method_name << " nije deklarisan u klasi " << pom_class_name << endl;
                return "Not decleard";
            }
        }
    }







    return "Not decleard";

}

string Class_attribut_call::typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const{

    map<string, vector<ClassVariable*>>::iterator tmp;
    string pom_class_name;


    for(unsigned i=0; i<tablica_funkcije[function_name].size(); i++){
        if(_name == tablica_funkcije[function_name][i]->getName()){
            pom_class_name=tablica_funkcije[function_name][i]->getType();
            tmp=table.find(pom_class_name);
            if(tmp==table.end()){
                cout << "Klasa " << pom_class_name << " nije definisana" << endl;
                return "Not decleard";
            }
            else {
                for (unsigned i=0; i<table[pom_class_name].size(); i++){
                    if(_att_name==table[pom_class_name][i]->getName()
                        && 0==table[pom_class_name][i]->getIndikator()){
                        if(table[pom_class_name][i]->getMod()=="public"){
                            return table[pom_class_name][i]->getType();

                        }
                        cout << "Atribut " << _att_name << " je privatan"<<endl;
                        return "private variable";
                    }
                }
                cout << "Atribut " << _att_name << " nije deklarisan u klasi " << pom_class_name << endl;
                return "Not decleard";
            }
        }
    }

    for(unsigned i=0; i<table[class_name].size(); i++){
        if(_name == table[class_name][i]->getName()){
            pom_class_name=table[class_name][i]->getType();
            tmp=table.find(pom_class_name);
            if(tmp==table.end()){
                cout << "Klasa " << pom_class_name << " nije definisana" << endl;
                return "Not decleard";
            }
            else {
                for (unsigned i=0; i<table[pom_class_name].size(); i++){
                    if(_att_name==table[pom_class_name][i]->getName()
                        && 0==table[pom_class_name][i]->getIndikator()){
                        if(table[pom_class_name][i]->getMod()=="public"){
                            return table[pom_class_name][i]->getType();

                        }
                        cout << "Atribut " << _att_name << " je privatan"<<endl;
                        return "private Variable";
                    }

                }
                cout << "Atribut " << _att_name << " nije deklarisan u klasi " << pom_class_name << endl;
                return "Not decleard";
            }
        }
    }
    return "Not decleard";

}

BinOp::~BinOp(){
    delete _e1;
    delete _e2;
}

string Add::typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const{
    string e1_type= _e1->typechack(class_name, function_name, tablica_funkcije);
    string e2_type= _e2->typechack(class_name, function_name, tablica_funkcije);
    if(e1_type==e2_type)
        return e1_type;

    if(((e1_type=="int" && e2_type == "double") || (e1_type == "double" && e2_type=="int")))
        return "double";


    return "Not decleard";
}

string Constant::typechack(string class_name, string function_name, map<string, vector<Variable*>> tablica_funkcije) const{
     return _type;
}

void Variable::print() const{
    cout << _name;
};

void Function_call::print() const{
    cout << _name << '(' << ')';
};

void Metod_call::print() const{
    cout << _name <<'.' << _method_name <<'(' << ')';
};

void Class_attribut_call::print() const{
    cout << _name <<'.' << _att_name;
};

void Add::print() const{
    _e1->print();
    cout << " + ";
    _e2->print();
};

void Double::print() const{
    cout << _d;
};

void Integer::print() const{
    cout << _num;
};

void String::print() const{
    cout << _val;
};
