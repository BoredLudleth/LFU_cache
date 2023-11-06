#ifndef PERFECT_CACHE_HPP
#define PERFECT_CACHE_HPP

#include <iostream>
#include <list>
#include <unordered_map>

int slow_get_page(int key);

template <typename KeyT, typename PageT>
class perfect_cache_t {
    private:
        size_t sz_;
        size_t elem_amount = 0;
        size_t currentPage = 0;

        size_t length_dataset_;
        int* dataset_;

        std::unordered_map <KeyT, PageT> cache_;

        std::unordered_map <KeyT, size_t> entry_;

        void output_hash () const {
            std::cout << "================"<< std::endl;

            std::cout << "CACHE:"<< std::endl;
            for (auto it = cache_.begin(); it != cache_.end(); ++it) {
                std::cout << it->first << " ";
            }
            std::cout << std::endl;

            std::cout << "ENTRY:"<< std::endl;
            for (auto it = entry_.begin(); it != entry_.end(); ++it) {
                std::cout << it->second << " ";
            }
            std::cout << std::endl;

            std::cout << "================"<< std::endl;
        }

        size_t next_entry (KeyT key) const {
            size_t result = currentPage;

            for (size_t i = currentPage + 1; i < length_dataset_; i++) {
                if (dataset_[i] == key) {
                    return i;
                }
            }

            return length_dataset_;
        }

        KeyT far_entry (KeyT key) const {
            KeyT result = 0;

            size_t max_len_to_next = 0;

            size_t key_len = next_entry(key);

            for (auto it = entry_.begin(); it != entry_.end(); ++it) {
                if (it->second > max_len_to_next) {
                    result = it->first;
                    max_len_to_next = it->second;
                }
            }

            result = (key_len >= max_len_to_next) ? key : result;

            return result;
        }

        bool full() const {return (elem_amount == sz_);}

    public:
        perfect_cache_t (int* dataset, size_t sz, size_t length_dataset) : dataset_(dataset), sz_(sz), length_dataset_(length_dataset), currentPage(0) {};

        template <typename F>
        bool lookup_update (KeyT key, F slow_get_page) {
            auto it = cache_.find (key);

            if (it == cache_.end()) {
                if (full ()) {

                    const KeyT farrest = far_entry(key);

                    if (farrest == key) {
                        currentPage++;
                        return false;
                    }
                
                    cache_.erase (farrest);
                    entry_.erase (farrest);


                    elem_amount--;
                }

                elem_amount++;

                cache_[key] = slow_get_page(key);
                entry_[key] = next_entry (key);

                currentPage++;

                return false;
            } else {
                entry_[key] = next_entry (key);
                currentPage++;
                return true;
            }
        }
};

#endif