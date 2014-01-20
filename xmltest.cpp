#include <iostream>
#include <deque>
#include <vector>
using namespace std;
#include "XmlParser.h"
#include "CPPGen.h"
#include "tinyxml.h"

                
const string KEY_LAYER="layer";
const string KEY_RESOURCE="resource";
const string KEY_SUB="sub";
const string KEY_NULL="NULL";

const int OPENED = 1;
const int CLOSED = 0;



int simpleFind(const string * arry,string s,int size)
{
        for(int i=0;i<size;i++){
                if(arry[i] == s)return 1;
        }
        return 0;
}

//it is class member
const string ClassKey[]={
        "work",
        "layer",
        "sub",
};

const string MemberKey[]={
        "resource",
};


class xmlItem{
public:
        string key;
        string name;
        string value;
        int    level;
        xmlItem(string key,string name,string value,int level){
                this->name = name;
                this->value = value;
                this->key = key;
                this->level = level;
        }

};

vector<xmlItem *>  iv;
deque<xmlItem *>  dv;
deque<int> dep;




bool ParseXmlText(TiXmlElement* pEleRoot,int level)
{
    if (NULL == pEleRoot)
    {
        return false;
    }
    TiXmlElement* pEle = NULL;


    bool theone=true;
    for (pEle = pEleRoot->FirstChildElement(); pEle; pEle = pEle->NextSiblingElement())
    {
        if(theone){
                theone = false;
                level++;
        }
        TiXmlAttribute* attr = pEle->FirstAttribute();  //获得student的name属性
        xmlItem * temp = new xmlItem(pEle->Value(),attr->Name(),attr->Value(),level);
        iv.push_back(temp);
        dv.push_back(temp);
        //cout<<pEle->Value()<<" ";
        attr = attr->Next();
        while (attr) {
          //cout << attr->Name() << " " << attr->Value() << endl;
          xmlItem * temp1 = new xmlItem("NULL",attr->Name(),attr->Value(),level);
            

          iv.push_back(temp1);
          dv.push_back(temp1);
          attr = attr->Next();
        }
        ParseXmlText(pEle,level);
        

        
        cout << endl;
    }


    return true;
}


int main()
{

  //TiXmlDocument* myDocument = new TiXmlDocument();
  //myDocument->LoadFile("test6.xml");

  //TiXmlElement* rootElement = myDocument->RootElement();  //Class
  //TiXmlElement* studentsElement = rootElement->FirstChildElement();  //Students
  //TiXmlElement* studentElement =  studentsElement->FirstChildElement();  //Students
  //while ( studentElement ) {
  //  TiXmlAttribute* attributeOfStudent = studentElement->FirstAttribute();  //获得student的name属性
  //  while ( attributeOfStudent ) { //    std::cout << attributeOfStudent->Name() << " : " << attributeOfStudent->Value() << std::endl;
  //    attributeOfStudent = attributeOfStudent->Next();
  //  }
  //  TiXmlElement* phoneElement = studentElement->FirstChildElement();//获得student的phone元素
  //  std::cout << "phone" << " : " << phoneElement->GetText() << std::endl;
  //  TiXmlElement* addressElement = phoneElement->NextSiblingElement();
  //  std::cout << "address" << " : " << phoneElement->GetText() << std::endl;
  //  studentElement = studentElement->NextSiblingElement();
  //}
  //return 0;
  
//        XmlParser xmlrole;
//        std::string xmlfile="test.xml";
//        xmlrole.init(xmlfile);
//        xmlrole.DoParser();

	TiXmlDocument aDoc("test.xml");

	if ( ! aDoc.LoadFile() )
	{
		perror("打开test.xml失败:");
		return -1;
	}

	TiXmlElement *pRoot = aDoc.RootElement();
	TiXmlElement *pWork = pRoot;

    
        ParseXmlText(pWork,0);
    
    
        //Create file 
        string hfile=pRoot->Value();
        string cfile=pRoot->Value();
        hfile+=".h";
        cfile+=".cpp";
        
        SimpleGen h(hfile);
        SimpleGen c(cfile);

        int current_class_level=0;
        int close_state = OPENED;
        int pre_level = 0;

        while(!dv.empty()){
        //while(!iv.empty()){
                //xmlItem * the = iv.back();
                xmlItem * the = dv.front();
 
                if(!dep.empty()){
                        pre_level = dep.back();
                }



                //Gernerate the class begin
                if(simpleFind(ClassKey,the->key,sizeof(ClassKey)/sizeof(string))){
                        string cid = the->key+"_"+the->name+"_"+the->value;
                        //dep.push_back(the->level);

                        //Get the press level
                        
                        cout<<"pre:"<<pre_level<<","<<"the:"<<the->level<<endl;
                        if(the->level > pre_level){
                                //该类属于上一层次的子类,因此上一层的类并没有处理完
                                dep.push_back(the->level);
                        }
                        else if(the->level == pre_level){
                                //处理完同一级的类了
                                h.genClassEnd(the->level);
                        }
                        else if(the->level < pre_level){
                                h.genClassEnd(pre_level);
                                h.genClassEnd(pre_level-1);
                                dep.pop_back();
                                //当前类处理完了
                        }
                        h.genClass(cid,the->level);

                        //if(close_state == CLOSED){
                        //        //h.genClassEnd(the->level-1);
                        //        close_state = OPENED;
                        //}else{
                        //        close_state = CLOSED;
                        //}
                }
               
                if("NULL" == the->key && the->level == pre_level){
                        h.genMember("string",the->name,the->level + 1);
                        //Here to process the cfile

                }

                if(simpleFind(MemberKey,the->key,sizeof(MemberKey)/sizeof(string))){
                        string mid = the->key+"_"+the->name+"_"+the->value;
                        h.genMember("string",mid,the->level);
#ifdef _DEBUG
                        cout<<"----------"<<endl;
                        cout<< the->key<<endl;
                        cout<<the->name<<endl;
                        cout<<the->value<<endl;
                        cout<<"----------"<<endl;
                        cout<<endl;
#endif


                }


                cout<<the->key<<" "<<the->name<<" "<<the->value<<" "<<the->level<<endl;
                //cout<<the->key+"_"+the->name+"_"+the->value<<"\t"<<the->level<<endl;
                delete the;
                //iv.pop_back();
                dv.pop_front();
        }
        while(!dep.empty()){
                current_class_level = dep.back();
                h.genClassEnd(current_class_level);
                dep.pop_back();
        }
        


        //SimpleGen sim("Test.h");
        //sim.genPretreatment("Test");

        //sim.genClass("Test");
        //sim.genMember("string","resource_id_1",1);
        //sim.genClassEnd();

        //sim.genPretreatmentEnd();

}

