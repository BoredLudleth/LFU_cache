#ifndef PERFECT_CACHE_HPP
#define PERFECT_CACHE_HPP

#include <iostream>
#include <list>
#include <unordered_map>
#include <algorithm>
#include <climits>

int slow_get_page(int key);

template <typename KeyT, typename PageT>
class perfect_cache_t {
    private:
        size_t sz_;
        size_t elem_amount;
        size_t length_dataset_;
        size_t current_dataset_elem;

        std::list<PageT> cache_;

        using ListIt = typename std::list<PageT>::iterator;
        std::unordered_map <KeyT, ListIt> hash_;

        int* prediction_;

        bool full() const {return (elem_amount == sz_);}

        auto far_entry (int* prediction_, std::unordered_map<KeyT, ListIt> hash_) const {
            auto it = hash_.begin();
            auto result = it;
            size_t length_to_farrest = 0;

            for (it = hash_.begin(); it != hash_.end(); ++it) {
                int current_length = 0;

                for (int i = current_dataset_elem + 1; i < length_dataset_; i++) {
                    if (it->first == prediction_[i]) {
                        current_length = i - current_dataset_elem;
                        break;
                    }
                }

                current_length == (current_length == 0) ? sz_ :  current_length;

                if (current_length > length_to_farrest) {
                    length_to_farrest = current_length;
                    result = it;
                }
            }

            return result;
        }

    public:
        perfect_cache_t (int* prediction, size_t sz, size_t length_dataset) : prediction_(prediction), sz_(sz), length_dataset_(length_dataset), current_dataset_elem(0) {};

        template <typename F>
        bool lookup_update (KeyT key, F slow_get_page) {
            auto it = hash_.find (key);

            if (it == hash_.end()) {
                if (full ()) {
                    auto farrest_entry = far_entry(prediction_, hash_);

                    cache_.erase ((*farrest_entry).second);          //problem
                    hash_.erase (farrest_entry);

                    elem_amount--;
                }

                cache_.push_front (slow_get_page(key));
                hash_[key] = cache_.begin();
                current_dataset_elem++;

                return false;
            } else {
                current_dataset_elem++;

                return true;
            }
        }

        size_t count_hits () {
            size_t total_hits = 0;

            for (int i = 0; i < length_dataset_; i++) {
                total_hits += lookup_update (prediction_[i], slow_get_page);
            }

            return total_hits;
        }
};

#endif
