#ifndef VB_RINGBUFFER_HPP
#define VB_RINGBUFFER_HPP

#include <cstdint>

namespace vb {

template<typename _type, uint8_t _2_size>
class RingBuffer {
public:
    void push_front(_type& obj) {
        head_idx = (head_idx - 1) & MASK;
        data[head_idx] = obj;
    }
    _type pop_front() {
        if (length() == 0) {
            throw std::out_of_range("Cannot pop empty ringbuffer");
        }
        std::size_t prev_loc = head_idx;
        head_idx = (head_idx + 1) & MASK;
        return data[prev_loc];
    }

    void push_back(_type& obj) {
        data[tail_idx] = obj;
        tail_idx = (tail_idx + 1) & MASK;
    }
    _type pop_back() {
        if (length() == 0) {
            throw std::out_of_range("Cannot pop empty ringbuffer");
        }
        tail_idx = (tail_idx - 1) & MASK;
        return data[tail_idx];
    }

    const _type& peek_at(std::size_t index) const {
        std::size_t idx = index + head_idx;
        idx &= MASK;
        return data[idx];
    }

    _type& operator[](std::size_t index) {
        std::size_t idx = index + head_idx;
        idx &= MASK;
        return data[idx];
    }

    std::size_t length() {
        return (tail_idx - head_idx) & MASK;
    }
private:
    static constexpr std::size_t MASK = (1<<_2_size - 1);
    static constexpr std::size_t SIZE = 1<<_2_size;

    _type data[SIZE];
    std::size_t head_idx;
    std::size_t tail_idx;
};

}

#endif