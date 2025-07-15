#include <vector>
#include <stdexcept>
#include <iostream>

#pragma once

class BitVector{
private:
    std::vector<bool> bits_;
    size_t size_;

public:
    BitVector() {}
    BitVector(size_t size): bits_(size, false), size_(size) {}
    BitVector(std::vector<bool> bits): bits_(bits), size_(bits.size()) {}
    BitVector(const BitVector& bit_vector): bits_(bit_vector.bits_), size_(bit_vector.size_) {}

    BitVector& operator=(const BitVector& bit_vector) {
        if(this != &bit_vector){
            bits_ = bit_vector.bits_;
            size_ = bit_vector.size_;
        }
        return *this;
    }

    bool operator[](size_t index) const{
        if (index >= size_) {
            throw std::out_of_range("Operator[] index out of range!");
        }
        return bits_[index];
    }

    void Set(size_t index, bool value){
        if (index >= size_) {
            throw std::out_of_range("Set ndex out of range!");
        }
        bits_[index] = value;
    }

    size_t GetSize() { 
        return size_;
    }

    void Set(std::vector<bool> bits) {
        bits_ = bits; 
        size_ = bits.size();
    }

    // OR
    BitVector operator|(const BitVector& bit_vector) const{
        if(size_ != bit_vector.size_) throw std::length_error("Sizes must be the same!");
        BitVector returnable(size_);
        for(int i = 0; i < size_; ++i){
            returnable.bits_[i] = bits_[i] || bit_vector[i];
        }
        return returnable;
    }

    // AND
    BitVector operator&(const BitVector& bit_vector) const{
        if(size_ != bit_vector.size_) throw std::length_error("Sizes must be the same!");
        BitVector returnable(size_);
        for(int i = 0; i < size_; ++i){
            returnable.bits_[i] = bits_[i] && bit_vector[i];
        }
        return returnable;
    }

    // negate
    BitVector operator~() const{
        BitVector returnable(size_);
        for(int i = 0; i < size_; ++i){
            returnable.bits_[i] = !bits_[i];
        }
        return returnable;
    }

    BitVector Substract(const BitVector& bit_vector) const{
        if(size_ != bit_vector.size_) throw std::length_error("Sizes must be the same!");
        BitVector returnable(size_);
        for(int i = 0; i < size_; ++i){
            returnable.bits_[i] = bits_[i] && !bit_vector.bits_[i];
        }
        return returnable;
    }
    
    // ==
    bool operator==(const BitVector& bit_vector){
        return size_ == bit_vector.size_ && bits_ == bit_vector.bits_;
    }

    // !=
    bool operator!=(const BitVector& bit_vector){
        return size_ != bit_vector.size_ || bits_ != bit_vector.bits_;
    }

    void Print(){
        for(size_t i = 0; i < size_; ++i){
            std::cout << (bits_[i] ? "1" : "0");
        }
    }
};
