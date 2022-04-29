#pragma once
#include <vector>
////
////#ifndef PERLIN_H
////#define PERLIN_H
//
//double fade(double t);
//double lerp(double t, double a, double b);
//double grad(int hash, double x, double y, double z);
//double perlin_noise(float x, float y, std::vector<int>& p);
//
//// tests
////// something is wrong with this vector return value
//std::vector<int> myFunction(int value, double bullshit) { 
//	std::vector<int> r{ 2,3,4,4 };
//	return r;
//}
//
//void get_Permutation_Vector(std::vector<int>* pVectorPointer);
//
//
//
////#endif // end PERLIN_H if



// fade(t)= t^3 (10 + t* (t* 6 - 15))
double fade(double t) {

    return t * t * t * (t * (t * 6 - 15) + 10);
};

// linear interpolation between a and b: For polishing 
// t = [0,1] is the persentage of the distance bewteen a and b
double lerp(double t, double a, double b) {
    return a + (b - a) * t;
}


// 
double grad(int hash, double x, double y, double z) {
    int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
    double u = h < 8 ? x : y,                 // INTO 12 GRADIENT DIRECTIONS.
        v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}


// generate Perlin noise    
// Parameter: 
// p is (psyodo) "random" value
double perlin_noise(float x, float y, std::vector<int>& p) {
    int z = 0.5;

    // x,y,z abrunden und mit 1111 1111 verunden == modulo 255
    int X = (int)floor(x) & 255,                  // FIND UNIT CUBE THAT
        Y = (int)floor(y) & 255,                  // CONTAINS POINT.
        Z = (int)floor(z) & 255;

    // Kommastellen
    x -= floor(x);                                // FIND RELATIVE X,Y,Z
    y -= floor(y);                                // OF POINT IN CUBE.
    z -= floor(z);

    double u = fade(x),                             // COMPUTE FADE CURVES
        v = fade(y),                                // FOR EACH OF X,Y,Z.
        w = fade(z);

    int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z,      // HASH COORDINATES OF
        B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;      // THE 8 CUBE CORNERS,

    // return perlin noise of x and y 
    return lerp(w,
        lerp(v, lerp(u, grad(p[AA], x, y, z),  // AND ADD
            grad(p[BA], x - 1, y, z)), // BLENDED
            lerp(u, grad(p[AB], x, y - 1, z),  // RESULTS
                grad(p[BB], x - 1, y - 1, z))),// FROM  8
        lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),  // CORNERS
            grad(p[BA + 1], x - 1, y, z - 1)), // OF CUBE
            lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
                grad(p[BB + 1], x - 1, y - 1, z - 1))));
}
//
//void get_Permutation_Vector(std::vector<int>* permVectorPointer) {
//    permVectorPointer->clear();
//
//    std::vector<int> permutation = { 151,160,137,91,90,15,
//    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
//    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
//    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
//    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
//    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
//    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
//    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
//    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
//    129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
//    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
//    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
//    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
//    };
//
//    for (int j = 0; j < 2; j++)
//        for (int i = 0; i < 256; i++) {
//            permVectorPointer->push_back(permutation[i]);
//        }
//}


 //initialise 2x permutation of a hard coded 256 int vector
//std::vector<int> PERLIN_H::kansei() {
 

std::vector<int> getPermutationVector() {
    std::vector<int> p;

    std::vector<int> permutation = { 151,160,137,91,90,15,
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
    129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
    };

    for (int j = 0; j < 2; j++)
        for (int i = 0; i < 256; i++) {
            p.push_back(permutation[i]);
        }

    return p;
}

