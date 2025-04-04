#include "VectorSearchEngine/VectorIndex.h"
#include "VectorSearchEngine/VectorMath.h"
#include <algorithm>

namespace VectorSearchEngine {

    VectorIndex::VectorIndex(VectorStorage* storage)
        : storage_(storage)
    {
    }

    std::vector<SearchResult> VectorIndex::search(const float* queryVec, std::size_t topK) const {
        std::vector<SearchResult> results;
        std::size_t totalVectors = storage_->size();
        std::size_t dim = storage_->getVectorDim();

        // 遍历所有存储的向量
        for (std::size_t i = 0; i < totalVectors; ++i) {
            try {
                // 尝试获取向量数据，如果该位置已被删除，会抛出异常
                const float* vec = storage_->getVector(i);
                float score = cosineSimilarity(queryVec, vec, dim);
                results.push_back({ i, score });
            }
            catch (const std::runtime_error&) {
                // 如果 getVector 抛出异常，表示此索引处无有效向量，直接跳过
                continue;
            }
        }

        // 对结果按相似度降序排序（相似度越高越好）
        std::sort(results.begin(), results.end(), [](const SearchResult& a, const SearchResult& b) {
            return a.score > b.score;
            });

        // 截取前 topK 个结果
        if (results.size() > topK) {
            results.resize(topK);
        }

        return results;
    }

} // namespace VectorSearchEngine
