#ifndef VECTOR_STORAGE_H
#define VECTOR_STORAGE_H

#include "VectorSearchEngine/MemoryPool.h"
#include <vector>
#include <string>
#include <stdexcept>
#include <cstddef>

namespace VectorSearchEngine {

    class VectorStorage {
    public:
       
        VectorStorage(std::size_t vectorDim, std::size_t capacity);

        
        ~VectorStorage();

       
        std::size_t addVector(const std::vector<float>& vec);

        
        const float* getVector(std::size_t index) const;

        
        void updateVector(std::size_t index, const std::vector<float>& vec);

        
        void deleteVector(std::size_t index);

        
        std::size_t size() const;

        
        bool saveToFile(const std::string& filename) const;

        
        bool loadFromFile(const std::string& filename);

        
        std::size_t getVectorDim() const { return vectorDim_; }

    private:
        std::size_t vectorDim_;          
        std::size_t capacity_;           
        MemoryPool pool_;                
        std::vector<void*> dataPointers_; 
        std::size_t currentSize_;        
    };

} // namespace VectorSearchEngine

#endif // VECTOR_STORAGE_H
