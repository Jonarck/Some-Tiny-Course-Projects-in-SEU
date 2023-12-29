#ifndef FACE_HEADER
#define FACE_HEADER 

#include "Edge.h"

namespace mmm{
    class Face{
        public:
            int v1,v2,v3;
            Face(int v1, int v2, int v3):v1(v1), v2(v2),v3(v3){};
            ~Face(){};
            Edge against(int v) const;
            void reverse();
            void replace(int v, int vNew);
    };
}
#endif // FACE_HEADER