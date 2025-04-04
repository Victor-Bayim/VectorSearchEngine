#include "VectorSearchEngine/MemoryPool.h"

namespace VectorSearchEngine {

    MemoryPool::MemoryPool(std::size_t blockSize, std::size_t blockCount)
        : blockSize_(blockSize), blockCount_(blockCount)
    {
        
        pool_ = new char[blockSize_ * blockCount_];
        used_.resize(blockCount_, false);
    }

    MemoryPool::~MemoryPool() {
        
        delete[] pool_;
    }

    void* MemoryPool::allocate() {
       
        for (std::size_t i = 0; i < blockCount_; ++i) {
            if (!used_[i]) {
                used_[i] = true;
               
                return pool_ + i * blockSize_;
            }
        }
       
        throw std::runtime_error("MemoryPool: No free blocks available.");
    }

    void MemoryPool::deallocate(void* ptr) {
        
        std::ptrdiff_t offset = static_cast<char*>(ptr) - pool_;
        if (offset < 0 || static_cast<std::size_t>(offset) >= blockSize_ * blockCount_) {
            throw std::runtime_error("MemoryPool: Invalid pointer deallocation.");
        }
        
        std::size_t index = offset / blockSize_;
        if (!used_[index]) {
            throw std::runtime_error("MemoryPool: Double deallocation detected.");
        }
       
        used_[index] = false;
    }

} // namespace VectorSearchEngine
