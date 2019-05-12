#ifndef OBJECT_H
#define OBJECT_H

#include "objectype.h"
#include <iostream>

class Object {
protected:
    Object(ObjectType x) : type(x) {}
    Object() : type(ObjectType::TNonType) {}

public:
    virtual ~Object() {}

    const ObjectType type;
};

#endif // OBJECT_H
