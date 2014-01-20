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
  //  while ( attributeOfStudent ) {
  //    std::cout << attributeOfStudent->Name() << " : " << attributeOfStudent->Value() << std::endl;
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
	//ShowNodeInfo(pWork);
    //parseElement(pWork);
    //while(!iv.empty()){
    while(!dv.empty()){
            //xmlItem * the = iv.back();
            xmlItem * the = dv.front();
            //cout<<the->key<<" "<<the->name<<" "<<the->value<<" "<<the->level<<endl;
            cout<<the->key+"_"+the->name+"_"+the->value<<"\t"<<the->level<<endl;
            delete the;
            //iv.pop_back();
            dv.pop_front();

    }

    SimpleGen sim("Test.h");
    sim.genPretreatment("Test");
    sim.genClass("Test");
    sim.genMember("string","resource_id_1");
    sim.genClassEnd();
    sim.genPretreatmentEnd();

}

