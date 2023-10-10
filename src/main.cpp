#include "lfu_cache.hpp"

int main () {
    int m;          //size
    int n;          //number of pages
    
    std::cin >> m >> n;

    cache_t<int, int> c{m};

    int* cache_buff = new int[n];

    for (int i = 0; i < n; i++) {
        std::cin >> cache_buff[i]; 
    }

    int total_hits = 0;

    for (int i = 0; i < n; i++) {
        total_hits += c.lookup_update (cache_buff[i], slow_get_page);
        // printf ("%d\n", total_hits);
    }

    std::cout << "Total hits " << total_hits << std::endl; 

    delete[] cache_buff;

    return 0;
}
