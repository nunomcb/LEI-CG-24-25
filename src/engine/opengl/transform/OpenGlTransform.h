#ifndef CG_OPENGLTRANSFORM_H
#define CG_OPENGLTRANSFORM_H

class OpenGlTransform {
public:
    virtual ~OpenGlTransform() = default;

    virtual void apply() = 0;
};

#endif //CG_OPENGLTRANSFORM_H
