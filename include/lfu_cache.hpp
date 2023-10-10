#ifndef LFU_CACHE_HPP
#define LFU_CACHE_HPP

#include <iostream>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <climits>

int slow_get_page(int key) {return key;}

void print_umap (std::unordered_map<int, int> m) {
        std::for_each(m.begin(),
                m.end(),
                [](const std::pair<int, int> &p) {
                    std::cout << "{" << p.first << ": " << p.second << "}\n";
                });
        std::cout << "\n" << std::endl;
}

void print_map (std::map<int, int> m) {
    for (const auto& [key, value] : m)
        std::cout << '[' << key << "] = " << value << "; " << std::endl;

    std::cout << "\n" << std::endl;
}

template <typename KeyT, typename PageT, typename freq_t = int>
class cache_t {
    private:
        size_t sz_;
        size_t elem_amount = 0;

        std::unordered_map<KeyT, PageT> cache_;

        std::map<KeyT, freq_t> freq_;                                   //компаратор нужен

    public:
        cache_t (size_t sz) : sz_(sz) {};                               // конструктор

        bool full() const {return (elem_amount == sz_);}

        auto find_less_freq (std::unordered_map<KeyT, PageT> cache_, std::map<KeyT, freq_t> freq_) {
            auto it = cache_.begin();
            auto result = it;
            freq_t min_freq = freq_[it->first];

            for (it = cache_.begin(); it != cache_.end(); ++it) {
                if (min_freq >= freq_[it->first]) {
                    result = it;
                }
            }

            return result;
        }

        template <typename F>
        bool lookup_update(KeyT key, F slow_get_page) {
            auto it = cache_.find(key);

            if (it == cache_.end()) {
                if (full()) {
                    auto least_freq = find_less_freq (cache_, freq_)->first;

                    cache_.erase (least_freq);          //problem
                    elem_amount--;
                }

                elem_amount++;
                if (freq_.find(key) == freq_.end()) {
                    freq_.emplace (key, 1);
                } else {
                    freq_[key] += 1;
                }

                cache_.insert ({key, slow_get_page (key)});
                
print_umap (cache_);
                return false;
            }
            
            freq_[key] += 1;
print_umap (cache_);
printf ("hit\n\n");
            return true;
        }
};

#endif