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
        size_t elem_amount = 0;
        size_t length_dataset_;
        size_t currentPage;

        std::list<PageT> cache_;

        using ListIt = typename std::list<PageT>::iterator;
        std::unordered_map <KeyT, ListIt> hash_;
        int* prediction_;

        bool full() const {return (elem_amount == sz_);}

        auto far_entry (int* prediction_, std::unordered_map<KeyT, ListIt> hash_) const {
            auto it = hash_.begin();
            auto result = it;
            size_t length_to_farrest = 0;

            if (currentPage + 1 == length_dataset_ - 1) {
                return it;
            }

            for (it = hash_.begin(); it != hash_.end(); ++it) {
                int current_length = 0;

                for (int i = currentPage + 1 ; i < length_dataset_; i++) {
                    if (it->first == prediction_[i]) {
                        current_length = i - currentPage;
                        break;
                    }
                }

                current_length = (current_length == 0) ? sz_  :  current_length;

                if (current_length > length_to_farrest) {
                    length_to_farrest = current_length;
                    result = it;
                }
            }

            return result;
        }

        void output_hash () {
            for (auto it = hash_.begin(); it != hash_.end(); ++it) {
                std::cout << it->first << " ";
            }
            std::cout << std::endl;

        }

        void output_list () {
            std::cout << "LIST:";
            for (const auto& elem: cache_) {
                std::cout << elem << " ";
            }
            std::cout << std::endl;
        }

    public:
        perfect_cache_t (int* prediction, size_t sz, size_t length_dataset) : prediction_(prediction), sz_(sz), length_dataset_(length_dataset), currentPage(0) {};

        template <typename F>
        bool lookup_update (KeyT key, F slow_get_page) {
            auto it = hash_.find (key);

            if (it == hash_.end()) {
                if (full ()) {
                    auto farrest_entry = far_entry(prediction_, hash_)->first;
                    auto it_list = hash_[farrest_entry];

                    cache_.erase (it_list);
                    hash_.erase (farrest_entry);

                    elem_amount--;
                }

                elem_amount++;
                cache_.push_front (slow_get_page(key));
                hash_[key] = cache_.begin();
                currentPage++;

                return false;
            } else {
                currentPage++;

                return true;
            }
        }

        size_t count_hits () {
            size_t total_hits = 0;
            currentPage = 0;

            for (int i = 0; i < length_dataset_; i++) {
                total_hits += lookup_update (prediction_[i], slow_get_page);
            }

            return total_hits;
        }

};

#endif
