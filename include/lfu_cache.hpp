#ifndef LFU_CACHE_HPP
#define LFU_CACHE_HPP

#include <map>
#include <unordered_map>
#include <algorithm>

int slow_get_page(int key) {return key;}

template <typename KeyT, typename PageT, typename FreqT = int, typename LastEntryT = int>
class cache_t {
    private:
        size_t sz_;
        size_t elem_amount = 0;
        size_t currentPage = 0;

        std::unordered_map<KeyT, PageT> cache_;
        std::map<KeyT, std::pair<FreqT, LastEntryT> > freq_;

        bool full() const {return (elem_amount == sz_);}

        auto find_less_freq (std::unordered_map<KeyT, PageT> cache_, std::map<KeyT, std::pair<FreqT, LastEntryT>> freq_) const {
            auto it = cache_.begin();
            auto result = it;
            FreqT min_freq = freq_[it->first].first;

            for (it = cache_.begin(); it != cache_.end(); ++it) {
                if (min_freq > freq_[it->first].first) {
                    result = it;
                } else if (min_freq == freq_[it->first].first) {
                    result = (freq_[result->first].second > freq_[it->first].second) ? it : result;
                }
            }

            return result;
        }

    public:
        cache_t (size_t sz) : sz_(sz) {};

        template <typename F>
        bool lookup_update (KeyT key, F slow_get_page) {
            auto it = cache_.find(key);

            if (it == cache_.end()) {
                if (full()) {
                    auto least_freq = find_less_freq (cache_, freq_)->first;

                    cache_.erase (least_freq);
                    elem_amount--;
                }

                elem_amount++;
                if (freq_.find(key) == freq_.end()) {
                    freq_.emplace (key, std::pair{1, currentPage});
                } else {
                    freq_[key].first += 1;
                    freq_[key].second = currentPage;
                }

                cache_.insert ({key, slow_get_page (key)});
                currentPage++;
                return false;
            }
            
            freq_[key].first += 1;
            freq_[key].second = currentPage;
            currentPage++;
        
            return true;
        }
};

#endif
