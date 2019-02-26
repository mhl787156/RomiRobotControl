#ifndef Utils_H
#define Utils_H

template <typename T> class Pair{
    public:
    Pair(T _x, T _y):x(_x), y(_y){};
    T x, y;
};

template<typename T> class Triplet{
    public:
    Triplet(T _x, T _y, T _z):x(_x), y(_y), z(_z){};
    T x, y, z;
};

// Inlined to ensure no multiple definition errors
inline int sign(float i) {
    if (i > 0) {
        return 1;
    } else if (i == 0) {
        return 0;
    } else {
        return -1;
    }
}

#endif