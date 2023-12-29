#ifndef EDGE_HEADER
#define EDGE_HEADER

namespace mmm {
    struct Edge {
        int v1;
        int v2;
        Edge(int v1 = -1, int v2 = -1) : v1(v1), v2(v2) {}

        bool operator < (const Edge &e2) const {
            if (v1 < e2.v1) {
                return true;
            }
            else if (v1 == e2.v1) {
                return v2 < e2.v2;
            }
            else {
                return false;
            }
        }
    };
}

#endif
