#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "VectorSearchEngine/VectorStorage.h"
#include "VectorSearchEngine/VectorIndex.h"



int main() {
    std::cout << "=== Vector Search Engine CLI ===" << std::endl;
    
    
    const std::size_t vectorDim = 3;   
    const std::size_t capacity = 100;    

    
    VectorSearchEngine::VectorStorage storage(vectorDim, capacity);
    VectorSearchEngine::VectorIndex index(&storage);

    
    while (true) {
        std::cout << "\nAvailable commands: add, search, update, delete, save, load, exit" << std::endl;
        std::cout << "Enter command: ";
        std::string command;
        std::cin >> command;
        
        if (command == "exit") {
            break;
        } else if (command == "add") {
            
            std::vector<float> vec(vectorDim);
            std::cout << "Enter " << vectorDim << " float values separated by spaces: ";
            for (std::size_t i = 0; i < vectorDim; ++i) {
                std::cin >> vec[i];
            }
            try {
                std::size_t idx = storage.addVector(vec);
                std::cout << "Vector added at index " << idx << "." << std::endl;
            } catch (const std::exception& ex) {
                std::cout << "Error adding vector: " << ex.what() << std::endl;
            }
        } else if (command == "search") {
           
            std::vector<float> query(vectorDim);
            std::cout << "Enter query vector (" << vectorDim << " float values): ";
            for (std::size_t i = 0; i < vectorDim; ++i) {
                std::cin >> query[i];
            }
            std::size_t topK;
            std::cout << "Enter number of top results (K): ";
            std::cin >> topK;
            try {
                auto results = index.search(query.data(), topK);
                std::cout << "Search Results:" << std::endl;
                for (const auto& res : results) {
                    std::cout << "Index: " << res.index << ", Score: " << res.score << std::endl;
                }
            } catch (const std::exception& ex) {
                std::cout << "Error during search: " << ex.what() << std::endl;
            }
        } else if (command == "update") {
            
            std::size_t idx;
            std::cout << "Enter index of vector to update: ";
            std::cin >> idx;
            std::vector<float> vec(vectorDim);
            std::cout << "Enter new vector values (" << vectorDim << " floats): ";
            for (std::size_t i = 0; i < vectorDim; ++i) {
                std::cin >> vec[i];
            }
            try {
                storage.updateVector(idx, vec);
                std::cout << "Vector at index " << idx << " updated." << std::endl;
            } catch (const std::exception& ex) {
                std::cout << "Error updating vector: " << ex.what() << std::endl;
            }
        } else if (command == "delete") {
            
            std::size_t idx;
            std::cout << "Enter index of vector to delete: ";
            std::cin >> idx;
            try {
                storage.deleteVector(idx);
                std::cout << "Vector at index " << idx << " deleted." << std::endl;
            } catch (const std::exception& ex) {
                std::cout << "Error deleting vector: " << ex.what() << std::endl;
            }
        } else if (command == "save") {
            
            std::string filename;
            std::cout << "Enter filename to save data: ";
            std::cin >> filename;
            if (storage.saveToFile(filename)) {
                std::cout << "Data saved to " << filename << "." << std::endl;
            } else {
                std::cout << "Failed to save data." << std::endl;
            }
        } else if (command == "load") {
            
            std::string filename;
            std::cout << "Enter filename to load data: ";
            std::cin >> filename;
            if (storage.loadFromFile(filename)) {
                std::cout << "Data loaded from " << filename << "." << std::endl;
            } else {
                std::cout << "Failed to load data." << std::endl;
            }
        } else {
            std::cout << "Unknown command. Please try again." << std::endl;
        }
    }
    
    std::cout << "Exiting CLI. Goodbye!" << std::endl;
    return 0;
}
