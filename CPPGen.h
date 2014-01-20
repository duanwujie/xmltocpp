#ifndef __CPPGEN_H_
#define __CPPGEN_H_

#include <iostream>
#include <fstream>
using namespace std;


class SimpleGen
{
        ofstream  out;
public:
        //SimpleGen(string filename);
        SimpleGen(string filename):out(filename.c_str(),ofstream::out|ofstream::trunc){}
        ~SimpleGen();

        /* class gen function */
        void genClass(string className);
        void genClassEnd();

        /*  member gen function  */
        void genMember(string type,string name);

        void genPretreatment(string className);
        void genPretreatmentEnd();
 
};



class layer_id_1 {
public:
       static const string resource_id_1;
       static const string resource_id_2;
       static const string resource_id_3;
       static const string resource_id_4;
       static const string resource_id_5;
       class sub_id_1{
       public:
               static const string resource_id_1;
       };
               
};


//const string layer_id_1::sub_id_1::resource_id_1="/home/test";

#endif
