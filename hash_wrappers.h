//
// Created by amrulla on 02.03.2021.
//

#ifndef THESIS_WORK_HASH_WRAPPERS_H
#define THESIS_WORK_HASH_WRAPPERS_H

#include <string>

class BadHash {
public:
    size_t operator()(const std::string& str) const;
};

//----------- CityHashes ----------

class CityHash32Wrapper {
public:
    size_t operator()(const std::string& str) const;
};

class CityHash64Wrapper {
public:
    size_t operator()(const std::string& str) const;
};

class CityHash64WithSeedWrapper {
public:
    size_t operator()(const std::string& str) const;
};

class CityHash64WithSeedsWrapper {
public:
    size_t operator()(const std::string& str) const;
};

class CityHash128Wrapper {
public:
    size_t operator()(const std::string& str) const;
};

class CityHash128WithSeedWrapper {
public:
    size_t operator()(const std::string& str) const;
};

//----------- FarmHashes ----------

class FarmHash32Wrapper {
public:
    size_t operator()(const std::string& str) const;
};

class FarmHash32WithSeedWrapper {
public:
    size_t operator()(const std::string& str) const;
};

class FarmHash64Wrapper {
public:
    size_t operator()(const std::string& str) const;
};

class FarmHash64WithSeedWrapper {
public:
    size_t operator()(const std::string& str) const;
};


class FarmHash64WithSeedsWrapper {
public:
    size_t operator()(const std::string& str) const;
};

class FarmHash128Wrapper {
public:
    size_t operator()(const std::string& str) const;
};

class FarmHash128WithSeedWrapper {
public:
    size_t operator()(const std::string& str) const;
};

//------------ xxHashes -----------

class xxHash32Wrapper {
public:
    size_t operator()(const std::string& str) const;
};

class xxHash64Wrapper {
public:
    size_t operator()(const std::string& str) const;
};

class xxHash128Wrapper {
public:
    size_t operator()(const std::string& str) const;
};

class XXH3_64bitsWrapper {
public:
    size_t operator()(const std::string& str) const;
};

class XXH3_64bits_withSeedWrapper {
public:
    size_t operator()(const std::string& str) const;
};

/*
class xxH3WithSecretWrapper {
public:
    size_t operator()(const std::string& str) const;
};*/

class XXH3_128bitsWrapper {
public:
    size_t operator()(const std::string& str) const;
};

class XXH3_128bits_withSeedWrapper {
public:
    size_t operator()(const std::string& str) const;
};

//---------- MurmurHashes ---------

class MurmurHash1Wrapper {
public:
    size_t operator()(const std::string& str) const;
};

class MurmurHash1AlignedWrapper {
public:
    size_t operator()(const std::string& str) const;
};

class MurmurHash2Wrapper {
public:
    size_t operator()(const std::string& str) const;
};

class MurmurHash64AWrapper {
public:
    size_t operator()(const std::string& str) const;
};

class MurmurHash64BWrapper {
public:
    size_t operator()(const std::string& str) const;
};

class MurmurHash3_x64_128Wrapper {
public:
    size_t operator()(const std::string& str) const;
};



#endif //THESIS_WORK_HASH_WRAPPERS_H
