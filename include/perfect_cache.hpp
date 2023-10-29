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

        std::unordered_map <KeyT, PageT> hash_;
        std::unordered_map <KeyT, size_t> cur_lens_;

        int* prediction_;

        bool full() const {return (elem_amount == sz_);}

        void output_hash () const {
            for (auto it = hash_.begin(); it != hash_.end(); ++it) {
                std::cout << it->first << " ";
            }
            std::cout << std::endl;

        }

        size_t check_entry (KeyT key) const {
            if (currentPage + 1 >= length_dataset_) {
                return length_dataset_;
            }

            for (size_t i = currentPage + 1; i < length_dataset_; ++i) {
                if (prediction_[i] == key) {
                    return i - currentPage;
                }
            }

            return length_dataset_;
        }

        KeyT far_entry (KeyT key) const {
            auto it = cur_lens_.begin();
            size_t result = it->first;
            size_t max_len_to_next = 0;

            size_t key_len = check_entry (key);

            for (it = cur_lens_.begin(); it != cur_lens_.end(); ++it) {
                if (it->second > max_len_to_next) {
                    result = it->first;
                    max_len_to_next = it->second;
                }
            }

            result = (key_len >= max_len_to_next) ? key : result;

            return result;
        }

        void decrise_lens () {
            for (auto it = cur_lens_.begin(); it != cur_lens_.end(); ++it) {
                if (it->second != length_dataset_)
                    it->second -= 1;
            }
        }

    public:
        perfect_cache_t (int* prediction, size_t sz, size_t length_dataset) : prediction_(prediction), sz_(sz), length_dataset_(length_dataset), currentPage(0) {};

        template <typename F>
        bool lookup_update (KeyT key, F slow_get_page) {
            auto it = hash_.find (key);

            if (it == hash_.end()) {
                if (full ()) {

                    const KeyT farrest = far_entry(key);

                    if (farrest == key) {
                        currentPage++;
                        decrise_lens();
                        return false;
                    }
                
                    hash_.erase (farrest);
                    cur_lens_.erase (farrest);


                    elem_amount--;
                }

                elem_amount++;

                hash_[key] = slow_get_page(key);
                cur_lens_[key] = check_entry (key);

                decrise_lens();

                currentPage++;

                return false;
            } else {
                cur_lens_[key] = check_entry (key);
                decrise_lens();
                currentPage++;
                return true;
            }
        }

        size_t count_hits () {
            size_t total_hits = 0;
            currentPage = 0;

            for (int i = 0; i < length_dataset_; i++) {
                total_hits += lookup_update (prediction_[i], slow_get_page);
                // output_hash();
            }

            return total_hits;
        }
};

#endif
