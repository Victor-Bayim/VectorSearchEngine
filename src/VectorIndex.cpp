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

        // �������д洢������
        for (std::size_t i = 0; i < totalVectors; ++i) {
            try {
                // ���Ի�ȡ�������ݣ������λ���ѱ�ɾ�������׳��쳣
                const float* vec = storage_->getVector(i);
                float score = cosineSimilarity(queryVec, vec, dim);
                results.push_back({ i, score });
            }
            catch (const std::runtime_error&) {
                // ��� getVector �׳��쳣����ʾ������������Ч������ֱ������
                continue;
            }
        }

        // �Խ�������ƶȽ����������ƶ�Խ��Խ�ã�
        std::sort(results.begin(), results.end(), [](const SearchResult& a, const SearchResult& b) {
            return a.score > b.score;
            });

        // ��ȡǰ topK �����
        if (results.size() > topK) {
            results.resize(topK);
        }

        return results;
    }

} // namespace VectorSearchEngine
