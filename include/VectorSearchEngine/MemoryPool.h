#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <cstddef>
#include <vector>
#include <stdexcept>

namespace VectorSearchEngine {

    class MemoryPool {
    public:
        
        MemoryPool(std::size_t blockSize, std::size_t blockCount);

        
        ~MemoryPool();

      
        void* allocate();

     
        void deallocate(void* ptr);

    private:
        std::size_t blockSize_;   
        std::size_t blockCount_;  
        char* pool_;              
        std::vector<bool> used_;  
    };

} // namespace VectorSearchEngine

#endif // MEMORY_POOL_H
