#include "CPPGen.h"


//SimpleGen::SimpleGen(string filename):out(filename,ofstream::out|fstream::in|ofstream::trunc)
//{
//}


SimpleGen::~SimpleGen()
{

}


#define G_CLASS_B(className) ("class " + className + "{" + "\n")

#define G_PUBLIC_B() ("public:\n")

#define G_CLASS_E() ("};\n\n")

#define G_MEMBER(type,name)("static const " + type + " " + name + ";\n")

#define G_PRETREATMENT(className) \
        ("#ifndef __"+className+"_H_\n"+\
        "#define __"+className+"_H_\n")
#define G_PRETREATMENT_E() ("#endif\n")

void inline SimpleGen::levelFormat(int level)
{
        while(level--)out<<"\t";
}


void SimpleGen::genPretreatment(string className,int level)
{
        //Turn to big
        levelFormat(level);
        out<<G_PRETREATMENT(className);
}

void SimpleGen::genPretreatmentEnd(int level)
{
        levelFormat(level);
        out<<G_PRETREATMENT_E();
}

void SimpleGen::genClass(string className,int level)
{
        levelFormat(level);
        out<<G_CLASS_B(className);
        levelFormat(level);
        out<<G_PUBLIC_B();
}

void SimpleGen::genMember(string type,string name,int level)
{
        levelFormat(level);
        out<<G_MEMBER(type,name);
}

void SimpleGen::genClassEnd(int level)
{
        levelFormat(level);
        out<<G_CLASS_E();
}

/*
 * 
 *  template
 */





const string layer_id_1::resource_id_1 = "/home/test";
const string layer_id_1::resource_id_2 = "/home/test";
const string layer_id_1::resource_id_3 = "/home/test";
const string layer_id_1::resource_id_4 = "/home/test";
const string layer_id_1::resource_id_5 = "/home/test";


