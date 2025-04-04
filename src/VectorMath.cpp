#include "VectorSearchEngine/VectorMath.h"
#include <cmath>

namespace VectorSearchEngine {

float dotProduct(const float* vec1, const float* vec2, std::size_t dim) {
    float dot = 0.0f;
    for (std::size_t i = 0; i < dim; ++i) {
        dot += vec1[i] * vec2[i];
    }
    return dot;
}

float cosineSimilarity(const float* vec1, const float* vec2, std::size_t dim) {
    float dot = dotProduct(vec1, vec2, dim);
    float norm1 = 0.0f;
    float norm2 = 0.0f;
    for (std::size_t i = 0; i < dim; ++i) {
        norm1 += vec1[i] * vec1[i];
        norm2 += vec2[i] * vec2[i];
    }
   
    if (norm1 == 0.0f || norm2 == 0.0f) {
        return 0.0f;
    }
    return dot / (std::sqrt(norm1) * std::sqrt(norm2));
}

float euclideanDistance(const float* vec1, const float* vec2, std::size_t dim) {
    float sum = 0.0f;
    for (std::size_t i = 0; i < dim; ++i) {
        float diff = vec1[i] - vec2[i];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}

} // namespace VectorSearchEngine
