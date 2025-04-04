#ifndef VECTOR_MATH_H
#define VECTOR_MATH_H

#include <cstddef>

namespace VectorSearchEngine {

    
    float dotProduct(const float* vec1, const float* vec2, std::size_t dim);

   
    float cosineSimilarity(const float* vec1, const float* vec2, std::size_t dim);

   
    float euclideanDistance(const float* vec1, const float* vec2, std::size_t dim);
    
} // namespace VectorSearchEngine

#endif // VECTOR_MATH_H
