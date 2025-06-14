#ifndef CG_OBJSERIALISER_H
#define CG_OBJSERIALISER_H

#include "ModelSerialiser.h"
#include "model/Model.h"

class ObjSerialiser: public ModelSerialiser {
public:
    void serialise(Model& model, std::ostream& out_stream) override;
    Model deserialise(std::istream& in) override;
};

#endif //CG_OBJSERIALISER_H
