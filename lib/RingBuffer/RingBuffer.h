#pragma once

#include <Arduino.h> // Needed for size_t and other Arduino types

template <typename T, size_t Capacity>
class RingBuffer {
  private:
    T buffer[Capacity];
    size_t head;  // Write index
    size_t tail;  // Read index
    size_t count; // Current number of elements

  public:
    // Constructor
    RingBuffer() : head(0), tail(0), count(0) {}

    // Add an element. Returns true if successful, false if buffer is full.
    bool push(const T& item) {
        if (isFull()) {
            return false;
        }
        
        buffer[head] = item;
        head = (head + 1) % Capacity;
        count++;
        return true;
    }

    // Remove an element. The value is stored in 'item'.
    // Returns true if successful, false if buffer is empty.
    bool pop(T& item) {
        if (isEmpty()) {
            return false;
        }

        item = buffer[tail];
        tail = (tail + 1) % Capacity;
        count--;
        return true;
    }

    // Peek at the next item without removing it
    T peek() const {
        return buffer[tail];
    }

    T* getWriteSlot() {
        if (isFull()) return nullptr;
        return &buffer[head];
    }

    void commit() {
        if (isFull()) return; // Safety check
        head = (head + 1) % Capacity;
        count++;
    }

    // Utility methods
    bool isFull() const { return count == Capacity; }
    bool isEmpty() const { return count == 0; }
    size_t size() const { return count; }
    size_t capacity() const { return Capacity; }
    
    // Clear the buffer
    void clear() {
        head = 0;
        tail = 0;
        count = 0;
    }
};