#include "NxAllFields.h"

NxAllFields::NxAllFields(void)
{
}

NxAllFields::~NxAllFields(void)
{
}

NxField NxAllFields::getFieldByScene(int indexScene){
	for(int i=0; i<fields.size(); i++){
		if(fields[i].indexScene==indexScene)
			return fields[i];
	}

}
