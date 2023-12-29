#ifndef VECTOR_HEADER
#define VECTOR_HEADER

#include <algorithm>

namespace mmm{
    class Vector{
        double a[4];
        int s;
    public:
        Vector(){
            s = 0;
        }
        Vector(int n){
            s = n;
        }
        Vector(double x, double y, double z){
            s = 3;
            a[0] = x; 
            a[1] = y; 
            a[2] = z;
        }
        Vector(int n, double x){
            s = n;
            for(int i = 0; i < n; i++){
                a[i] = x;
            }
        }
        void swap(Vector &b){
            for(int i = 0; i < s; i++){
                std::swap(a[i], b.a[i]);
            }
        }
        int size() const {return s;}
        void clear(){s = 0;}
        double& operator [] (int x) {return a[x]; }
        const double& operator [] (int x) const {return a[x]; }
        void push_back(double x){ a[s++] = x; }
        void pop_back() { s--; }
    };
    Vector operator + (const Vector &a, const Vector &b);
    Vector operator - (const Vector &a, const Vector &b);
    Vector operator * (const Vector &a, const Vector &b);
    Vector operator / (const Vector &a, const double &b);

    double norm(const Vector &v);
    Vector crossProduct(const Vector &a, const Vector &b);
    double innerProduct(const Vector &a, const Vector &b);

}

#endif