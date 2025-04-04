#ifndef VECTOR_INDEX_H
#define VECTOR_INDEX_H

#include "VectorStorage.h"
#include <vector>
#include <cstddef>

namespace VectorSearchEngine {

    
    struct SearchResult {
        std::size_t index;
        float score;
    };

   
    class VectorIndex {
    public:
        
        explicit VectorIndex(VectorStorage* storage);

        
        std::vector<SearchResult> search(const float* queryVec, std::size_t topK) const;

    private:
        VectorStorage* storage_; 
    };

} // namespace VectorSearchEngine

#endif // VECTOR_INDEX_H
