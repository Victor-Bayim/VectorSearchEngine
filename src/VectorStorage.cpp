#include "VectorSearchEngine/VectorStorage.h"
#include <cstring>   
#include <fstream>

namespace VectorSearchEngine {

VectorStorage::VectorStorage(std::size_t vectorDim, std::size_t capacity)
    : vectorDim_(vectorDim),
      capacity_(capacity),
    
      pool_(vectorDim * sizeof(float), capacity),
      currentSize_(0)
{
    dataPointers_.resize(capacity, nullptr);
}

VectorStorage::~VectorStorage() {
    
    for (std::size_t i = 0; i < currentSize_; ++i) {
        if (dataPointers_[i] != nullptr) {
            pool_.deallocate(dataPointers_[i]);
        }
    }
}

std::size_t VectorStorage::addVector(const std::vector<float>& vec) {
    if (vec.size() != vectorDim_) {
        throw std::runtime_error("VectorStorage: Vector dimension mismatch.");
    }
    if (currentSize_ >= capacity_) {
        throw std::runtime_error("VectorStorage: Capacity exceeded.");
    }
    void* mem = pool_.allocate();
    std::memcpy(mem, vec.data(), vectorDim_ * sizeof(float));
    dataPointers_[currentSize_] = mem;
    return currentSize_++;
}

const float* VectorStorage::getVector(std::size_t index) const {
    if (index >= currentSize_ || dataPointers_[index] == nullptr) {
        throw std::runtime_error("VectorStorage: Invalid index.");
    }
    return static_cast<const float*>(dataPointers_[index]);
}

void VectorStorage::updateVector(std::size_t index, const std::vector<float>& vec) {
    if (index >= currentSize_ || dataPointers_[index] == nullptr) {
        throw std::runtime_error("VectorStorage: Invalid index.");
    }
    if (vec.size() != vectorDim_) {
        throw std::runtime_error("VectorStorage: Vector dimension mismatch.");
    }
    std::memcpy(dataPointers_[index], vec.data(), vectorDim_ * sizeof(float));
}

void VectorStorage::deleteVector(std::size_t index) {
    if (index >= currentSize_ || dataPointers_[index] == nullptr) {
        throw std::runtime_error("VectorStorage: Invalid index.");
    }
    
    pool_.deallocate(dataPointers_[index]);
    dataPointers_[index] = nullptr;
    
}

std::size_t VectorStorage::size() const {
    return currentSize_;
}

bool VectorStorage::saveToFile(const std::string& filename) const {
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs) {
        return false;
    }
    
    ofs.write(reinterpret_cast<const char*>(&currentSize_), sizeof(currentSize_));
    ofs.write(reinterpret_cast<const char*>(&vectorDim_), sizeof(vectorDim_));
    
    for (std::size_t i = 0; i < currentSize_; ++i) {
        const float* vecData = static_cast<const float*>(dataPointers_[i]);
        ofs.write(reinterpret_cast<const char*>(vecData), vectorDim_ * sizeof(float));
    }
    return ofs.good();
}

bool VectorStorage::loadFromFile(const std::string& filename) {
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) {
        return false;
    }
    std::size_t numVectors;
    std::size_t fileVectorDim;
    ifs.read(reinterpret_cast<char*>(&numVectors), sizeof(numVectors));
    ifs.read(reinterpret_cast<char*>(&fileVectorDim), sizeof(fileVectorDim));
    if (fileVectorDim != vectorDim_ || numVectors > capacity_) {
        return false;
    }
    
    for (std::size_t i = 0; i < currentSize_; ++i) {
        if (dataPointers_[i] != nullptr) {
            pool_.deallocate(dataPointers_[i]);
            dataPointers_[i] = nullptr;
        }
    }
    currentSize_ = 0;
   
    for (std::size_t i = 0; i < numVectors; ++i) {
        void* mem = pool_.allocate();
        ifs.read(reinterpret_cast<char*>(mem), vectorDim_ * sizeof(float));
        dataPointers_[currentSize_++] = mem;
    }
    return ifs.good();
}

} // namespace VectorSearchEngine
