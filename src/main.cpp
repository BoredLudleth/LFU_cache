#include "lfu_cache.hpp"
#include "perfect_cache.hpp"

#include <time.h>
#include <fstream>
#include <string>

int main () {
    std::cout << "==============================================" << std::endl;

    unsigned int start_preparation = clock ();
    size_t m;          //size
    size_t n;          //number of pages
    
    std::cin >> m >> n;
    cache_t<int, int> c{m};

    int* cache_buff = new int[n];

    if (cache_buff == nullptr) {
        std::cout << "Problem with memory allocation" << std::endl;
        return 1;
    }

    for (int i = 0; i < n; i++) {
        std::cin >> cache_buff[i]; 
    }

    unsigned int end_preparation = clock ();

    perfect_cache_t<int, int> perfecto{cache_buff, m, n};

    std::cout << "Perfect cache: " << perfecto.count_hits() << " hits" << std::endl; 

    unsigned int end_perfect_cache = clock ();

    size_t total_hits = 0;

    for (int i = 0; i < n; i++) {
        total_hits += c.lookup_update (cache_buff[i], slow_get_page);
    }

    std::cout << "LFU-cache: " << total_hits << " hits" << std::endl;

    unsigned int end_lfu = clock ();

    delete[] cache_buff;

    unsigned int end_time = clock();

    float search_time_perfect_cache = (float) (end_perfect_cache - start_preparation + end_time - end_lfu) / ((float) CLOCKS_PER_SEC);
    float search_time_lfu_cache     = (float) (end_preparation - start_preparation + end_time - end_perfect_cache) / ((float) CLOCKS_PER_SEC);

    std::cout << "Time for perfect cache " << search_time_perfect_cache << " sec"<< std::endl;
    std::cout << "Time for LFU cache " << search_time_lfu_cache << " sec" << std::endl;
    std::cout << "==============================================" << std::endl;

    return 0;
}
