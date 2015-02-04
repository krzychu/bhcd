#ifndef _BHCD_ENGINE_H_
#define _BHCD_ENGINE_H_

#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "likelihood.h"
#include "split.h"
#include "model.h"
#include "network.h"

namespace BHCD
{
    class Engine   
    {
        private:
            Model model;

        public:
            Engine(const Engine& other) = delete;
            Engine& operator=(const Engine& other) = delete;

            Engine(Likelihood* _internal, Likelihood* _external, Split* _split) :
                model(_internal, _external, _split)
            {}

            std::map<std::string, std::string> infer_hierarchy(std::ostream& log, const Network& network) const;
    };
}

#endif
