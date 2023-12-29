#ifndef CROSS_LINK_HEADER
#define CROSS_LINK_HEADER 

#include <cassert>

namespace mmm{
    class CrossLink{
        public:
        struct Node {
            Node *lhs, *rhs, *ups, *dwn;
            int x;
            int y;
            Node(int x, int y) : x(x), y(y) {
                lhs = rhs = ups = dwn = nullptr;
            }
        };
        int s;
        Node **edgeX;
        Node **edgeY;
        CrossLink(int size) : s(size){
            edgeX = new Node *[size];
            edgeY = new Node *[size];
            for (int i = 0; i < size; ++i){
                edgeX[i] = edgeY[i] = nullptr;
            }
        }

        ~CrossLink(){
            for (int i = 0; i < s; ++i){
                Node *node = edgeX[i];
                 while (node != nullptr) {
                    Node *next = node->dwn;
                    delete node;
                    node = next;
                }
            }
            delete[] edgeX;
            delete[] edgeY;
        }
        inline int size() const { return s; }
        bool insert(int x, int y) {

            Node *p = edgeX[x];
            Node *node = new Node(x, y);
            if (p == nullptr) {
                edgeX[x] = node;
            }
            else {
                while (p != nullptr) {
                    if (p->y > y) {
                        node->dwn = edgeX[x];
                        edgeX[x]->ups = node;
                        edgeX[x] = node;
                        break;
                    }
                    else if (p->y == y) {
                        delete node;
                        return true;
                    }
                    else {
                        if (p->dwn == nullptr) {
                            p->dwn = node;
                            node->ups = p;
                            break;
                        }
                        else if (p->dwn->y > y) {
                            node->dwn = p->dwn;
                            node->ups = p;
                            p->dwn->ups = node;
                            p->dwn = node;
                            break;
                        }
                        else {
                            p = p->dwn;
                        }
                    }
                }
            }
            p = edgeY[y];
            if (p == nullptr) {
                edgeY[y] = node;
            }
            else {
                while (p != nullptr) {
                    if (p->x > x) {
                        node->rhs = edgeY[y];
                        edgeY[y]->lhs = node;
                        edgeY[y] = node;
                        break;
                    }
                    else if (p->x == x) {
                        return true;
                    }
                    else {
                        if (p->rhs == nullptr) {
                            p->rhs = node;
                            node->lhs = p;
                            break;
                        }
                        else if (p->rhs->x > x) {
                            node->rhs = p->rhs;
                            node->lhs = p;
                            p->rhs->lhs = node;
                            p->rhs = node;
                            break;
                        }
                        else {
                            p = p->rhs;
                        }
                    }
                }
            }
            return false;
        }
        bool find(int x, int y) {
            Node *p = edgeX[x];
            if (p == nullptr) {
                return false;
            }
            else {
                while (p != nullptr) {
                    if (p->y > y) {
                        return false;
                    }
                    else if (p->y == y) {
                        return true;
                    }
                    else {
                        if (p->dwn == nullptr) {
                            return false;
                        }
                        else if (p->dwn->y > y) {
                            return false;
                        }
                        else {
                            p = p->dwn;
                        }
                    }
                }
            }
            return false;
        }

        void erase(int x, int y) {
            Node *p = edgeX[x];
            while (p != nullptr) {
                if (p->y > y) {
                    break;
                }
                else if (p->y == y) {

                    if (p->lhs != nullptr) {
                        p->lhs->rhs = p->rhs;
                    }
                    else {
                        assert(p == edgeY[y]);
                        edgeY[y] = p->rhs;
                    }

                    if (p->rhs != nullptr) {
                        p->rhs->lhs = p->lhs;
                    }

                    if (p->ups != nullptr) {
                        p->ups->dwn = p->dwn;
                    }
                    else {
                        assert(p == edgeX[x]);
                        edgeX[x] = p->dwn;
                    }

                    if (p->dwn != nullptr) {
                        p->dwn->ups = p->ups;
                    }

                    delete p;
                    break;
                }
                else {
                    if (p->dwn == nullptr) {
                        break;
                    }
                    else if (p->dwn->y > y) {
                        break;
                    }
                    else {
                        p = p->dwn;
                    }
                }
            }
        }

    };
}
#endif