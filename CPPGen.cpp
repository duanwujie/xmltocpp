#include "CPPGen.h"


//SimpleGen::SimpleGen(string filename):out(filename,ofstream::out|fstream::in|ofstream::trunc)
//{
//}


SimpleGen::~SimpleGen()
{

}


#define G_CLASS_B(className) ("class " + className + "{" + "\n"+ "public:" + "\n")
#define G_CLASS_E() ("};\n")
#define G_MEMBER(type,name)("\tstatic const " + type + " " + name + ";\n")
#define G_PRETREATMENT(className) \
        ("#ifndef __"+className+"_H_\n"+\
        "#define __"+className+"_H_\n")
#define G_PRETREATMENT_E() ("#endif\n")
void SimpleGen::genPretreatment(string className)
{
        //Turn to big
        out<<G_PRETREATMENT(className);
}

void SimpleGen::genPretreatmentEnd()
{
        out<<G_PRETREATMENT_E();
}

void SimpleGen::genClass(string className)
{
        out<<G_CLASS_B(className);
}

void SimpleGen::genMember(string type,string name)
{
        out<<G_MEMBER(type,name);
}

void SimpleGen::genClassEnd()
{
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


