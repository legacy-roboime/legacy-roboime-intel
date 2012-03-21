#pragma once
#include "NxField.h"

class NxAllFields
{
public:
	NxArray<NxField> fields;
public:
	NxAllFields(void);
	~NxAllFields(void);
	NxField getFieldByScene(int indexScene);
};
