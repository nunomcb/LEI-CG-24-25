#ifndef CG_MODELSERIALISER_H
#define CG_MODELSERIALISER_H

#include <istream>
#include <ostream>

#include "../Model.h"

class ModelSerialiser {
public:
    virtual void serialise(Model& model, std::ostream& out) = 0;
    virtual Model deserialise(std::istream& in) = 0;
};

#endif //CG_MODELSERIALISER_H
