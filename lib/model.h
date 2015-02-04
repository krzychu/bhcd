#ifndef _BHCD_MODEL_H_
#define _BHCD_MODEL_H_

#include "split.h"
#include "likelihood.h"
#include "statistics.h"

namespace BHCD
{
    struct Model
    {
        const Likelihood& internal;
        const Likelihood& external;
        const Split& split;

        Model(const Likelihood* _internal, const Likelihood* _external, const Split* _split) :
            internal(*_internal), external(*_external), split(*_split)
        {}

        Model(const Model& model) = delete;
        Model& operator = (const Model& other) = delete;

        ~Model()
        {
            delete &internal;
            delete &external;
            delete &split;
        }
    };
}

#endif
