#include <cassert>
#include <algorithm>
#include "Face.h"

namespace mmm{
    Edge Face::against(int v) const {
        assert(v == v1 || v == v2 || v == v3);
        if(v == v1){
            return Edge(v2,v3);
        }
        else if(v == v2){
            return Edge(v3,v1);
        }
        else{
            return Edge(v1,v2);
        }
    }

    void Face::reverse() {
        std::swap(v1,v2);
    }

    void Face::replace(int v, int vNew) {
        assert(v == v1 || v == v2 || v == v3);
        if(v == v1) {
            v1 = vNew;
        }
        else if(v == v2) {
            v2 = vNew;
        }
        else{
            v3 = vNew;
        }
    }

}