#include <cassert>
#include <iostream>
#include <vector>
#include <cmath>
#include "VectorSearchEngine/MemoryPool.h"
#include "VectorSearchEngine/VectorStorage.h"
#include "VectorSearchEngine/VectorMath.h"
#include "VectorSearchEngine/VectorIndex.h"



int main() {
    std::cout << "Running unit tests..." << std::endl;
    
    // ============================
    // Test MemoryPool Module
    // ============================
    {
        std::size_t blockSize = 16;
        std::size_t blockCount = 10;
        VectorSearchEngine::MemoryPool pool(blockSize, blockCount);
        
        
        void* p1 = pool.allocate();
        void* p2 = pool.allocate();
        assert(p1 != nullptr);
        assert(p2 != nullptr);
        assert(p1 != p2);
        
       
        pool.deallocate(p1);
        pool.deallocate(p2);
        
        
        std::vector<void*> pointers;
        for (std::size_t i = 0; i < blockCount; ++i) {
            pointers.push_back(pool.allocate());
        }
        try {
            pool.allocate();
            
            assert(false);
        } catch (const std::runtime_error& ) {
            
        }
        
        for (auto p : pointers) {
            pool.deallocate(p);
        }
    }
    
    // ============================
    // Test VectorMath Module
    // ============================
    {
        std::vector<float> a = {1.0f, 2.0f, 3.0f};
        std::vector<float> b = {4.0f, 5.0f, 6.0f};
        
        float dot = VectorSearchEngine::dotProduct(a.data(), b.data(), a.size());
        // dot = 1*4 + 2*5 + 3*6 = 32
        assert(std::fabs(dot - 32.0f) < 1e-5);
        
        float cosine = VectorSearchEngine::cosineSimilarity(a.data(), b.data(), a.size());
        
        assert(std::fabs(cosine - 0.97463185f) < 1e-5);
        
        float euclidean = VectorSearchEngine::euclideanDistance(a.data(), b.data(), a.size());
        // sqrt((3^2)+(3^2)+(3^2)) = sqrt(27) ≈ 5.19615
        assert(std::fabs(euclidean - 5.19615f) < 1e-4);
    }
    
    // ============================
    // Test VectorStorage and VectorIndex Modules
    // ============================
    {
        const std::size_t vectorDim = 3;
        const std::size_t capacity = 10;
        VectorSearchEngine::VectorStorage storage(vectorDim, capacity);
        
       
        std::vector<float> vec1 = {1.0f, 0.0f, 0.0f};
        std::vector<float> vec2 = {0.0f, 1.0f, 0.0f};
        std::size_t idx1 = storage.addVector(vec1);
        std::size_t idx2 = storage.addVector(vec2);
        assert(idx1 == 0);
        assert(idx2 == 1);
        assert(storage.size() == 2);
        
        
        const float* retVec1 = storage.getVector(idx1);
        assert(std::fabs(retVec1[0] - 1.0f) < 1e-5);
        
       
        std::vector<float> vec1_updated = {0.5f, 0.4f, 0.0f};
        storage.updateVector(idx1, vec1_updated);
        retVec1 = storage.getVector(idx1);
        assert(std::fabs(retVec1[0] - 0.5f) < 1e-5);
        
        
        storage.deleteVector(idx2);
        try {
            storage.getVector(idx2);
            assert(false); 
        } catch (const std::runtime_error& ) {
            
        }
        
      
        std::vector<float> vec3 = {1.0f, 1.0f, 0.0f};
        std::size_t idx3 = storage.addVector(vec3);
        assert(idx3 == 2);
        
        
        assert(storage.getVectorDim() == vectorDim);
        
        
        VectorSearchEngine::VectorIndex index(&storage);
        std::vector<float> query = {1.0f, 1.0f, 0.0f};
        auto results = index.search(query.data(), 2);
        
       
        assert(!results.empty());
        assert(results[0].index == 2);
        assert(std::fabs(results[0].score - 1.0f) < 1e-5);
    }
    
    std::cout << "All tests passed successfully." << std::endl;
    return 0;
}
