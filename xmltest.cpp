#include <iostream>
#include <deque>
#include <vector>
#include <list>
#include <utility>
using namespace std;
#include "XmlParser.h"
#include "CPPGen.h"
#include "tinyxml.h"


TiXmlElement * g_pRoot=NULL;


int simpleFind(const string * arry,string s,int size)
{
        for(int i=0;i<size;i++){
                if(arry[i] == s)return 1;
        }
        return 0;
}

//it is class member
const string ClassKey[]={
        "works",
        "layer",
        "sub",
        "subsub",
        "resource",
};

const string MemberKey[]={
//        "resource",
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



/*  存放域名字符串 */
list< pair<string,int> > keydomin;


list<TiXmlElement *> pparent;


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

        dv.push_back(temp);
        //cout<<pEle->Value()<<"::";

        TiXmlAttribute* first = attr;

        attr = attr->Next();

        //TiXmlNode *parent = pEle->Parent();

        

        while (attr) {
                //cout << attr->Name() << " " << attr->Value() << endl;
        TiXmlElement * p = pEle;
        while(p!=g_pRoot){
                pparent.push_back(p);
                p=p->Parent()->ToElement();
        }

        while(!pparent.empty()){
                TiXmlElement * cur = pparent.back();
                TiXmlAttribute * cat = cur->FirstAttribute();
                cout<<cur->Value()<<"_"<<cat->Name()<<"_"<<cat->Value()<<"::";
                pparent.pop_back();
        }
                
                cout<<attr->Name()<<" = "<<"\""<<attr->Value()<<"\""<<endl;
                xmlItem * temp1 = new xmlItem("NULL",attr->Name(),attr->Value(),level);
                dv.push_back(temp1);
                attr = attr->Next();
        }
        ParseXmlText(pEle,level);
        

        
        //cout << endl;
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
        g_pRoot = pRoot;

    
        ParseXmlText(pWork,0);
    
    
        //Create file 
        string hfile=pRoot->Value();
        string cfile=pRoot->Value();
        hfile+=".h";
        cfile+=".cpp";
        
        SimpleGen h(hfile);
        SimpleGen c(cfile);

        /*  生成root类 */
        h.genPretreatment(pRoot->Value());
        h.genClass(pRoot->Value());
        TiXmlAttribute * attri = pRoot->FirstAttribute();
        while(attri){
                h.genMember("string",attri->Name(),1);
                attri = attri->Next();
        }
        
       
        

        

        int current_class_level=0;
        int pre_level = 0;

                        
        int cen=0xFF;

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
#ifdef _DEBUG
                        cout<<"pre:"<<pre_level<<","<<"the:"<<the->level<<endl;
#endif
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

                        //if(!keydomin.empty() && keydomin.back().second >= the->level)keydomin.pop_back();
                        keydomin.push_back(make_pair(cid,the->level));

                }


               
                if("NULL" == the->key && the->level == pre_level){
                        h.genMember("string",the->name,the->level + 1);

                        //cout<<"name:"<<the->name<<","<<"level:"<<the->level<<endl;
                        //Here to process the cfile
                        string value = the->value;
                        string domin;

                        

#ifdef _DEBUG
#undef _DEBUG
                        cout<<"----------"<<endl;
                        cout<< the->key<<endl;
                        cout<<the->name<<endl;
                        cout<<the->value<<endl;
                        cout<<"----------"<<endl;
                        cout<<endl;
#endif


                }

                if(simpleFind(MemberKey,the->key,sizeof(MemberKey)/sizeof(string))){
                        string mid = the->key+"_"+the->name+"_"+the->value;
                        h.genMember("string",mid,the->level);
//#define _DEBUG
#ifdef _DEBUG
#undef _DEBUG

                        cout<<"----------"<<endl;
                        cout<< the->key<<endl;
                        cout<<the->name<<endl;
                        cout<<the->value<<endl;
                        cout<<"----------"<<endl;
                        cout<<endl;
#endif

                }


                //cout<<the->key<<" "<<the->name<<" "<<the->value<<" "<<the->level<<endl;
                //cout<<the->key+"_"+the->name+"_"+the->value<<"\t"<<the->level<<endl;
                delete the;
                //iv.pop_back();
                dv.pop_front();
        }

        //All closed gen {
        while(!dep.empty()){
                current_class_level = dep.back();
                h.genClassEnd(current_class_level);
                dep.pop_back();
        }
        h.genClassEnd();
        h.genPretreatmentEnd();


        
#ifdef _DEBUG
        cout<<"++++++++++++++++++++++++++++++++++++++++"<<endl;
        while(!keydomin.empty()){
                pair<string,int> domin = keydomin.front();
                //cout<<domin.first<<","<<domin.second<<endl;
                cout<<domin.second<<",";
                keydomin.pop_front();
        }
        cout<<"++++++++++++++++++++++++++++++++++++++++"<<endl;
#endif







        //SimpleGen sim("Test.h");
        //sim.genPretreatment("Test");

        //sim.genClass("Test");
        //sim.genMember("string","resource_id_1",1);
        //sim.genClassEnd();

        //sim.genPretreatmentEnd();

}

