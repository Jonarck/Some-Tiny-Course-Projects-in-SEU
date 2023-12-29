#include "Vector.h"
#include <cassert>
#include <math.h>

namespace mmm{
    Vector operator + (const Vector &a, const Vector &b){
        assert(a.size() == b.size());
        Vector c(a.size());
        for(int i = 0; i < a.size();i++){
            c[i] = a[i] + b[i];
        }
        return c;
    }
    Vector operator - (const Vector &a, const Vector &b) {
        assert(a.size() == b.size());
        Vector c(a.size());
        for(int i = 0; i < a.size();i++){
            c[i] = a[i] - b[i];
        }
        return c;
    }
    Vector operator * (const double &a, const Vector &b){
        Vector c(b.size());
        for(int i = 0; i < b.size();i++){
            c[i] = a * b[i];
        }
        return c;
    }
    Vector operator / (const Vector &a, const double &b){
        assert(b != 0);
        Vector c(a.size());
        for(int i = 0; i < a.size();i++){
            c[i] = a[i] / b;
        }
        return c;
    }
    double norm(const Vector &v){
        double t = 0;
        for(int i = 0; i < v.size();i++){
            t += v[i] * v[i];
        }
        return sqrt(t);
    }
    Vector crossProduct(const Vector &a, const Vector &b){
        assert(a.size() == 3&& b.size() == 3);
        Vector c(3);
        c[0] = a[0] * b[2] - a[2] * b[1];
        c[1] = a[2] * b[0] - a[0] * b[2];
        c[2] = a[0] * b[1] - a[1] * b[0];
        return c;
    }
    double innerProduct(const Vector &a, const Vector &b){
        assert(a.size() == b.size());
        double c = 0;
        for(int i = 0; i < a.size();i++){
            c += a[i] * b[i];
        }
        return c;
    }
}