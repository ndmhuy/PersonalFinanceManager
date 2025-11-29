template <typename T>
class ArrayList
{
private:
    T *data;
    size_t size;
    size_t capacity;
    
    
public:
    ArrayList() : size(0), capacity(16) { data = new T[capacity]; }
    
    ArrayList(size_t initCap) : size(0) {
        capacity = (initCap == 0) ? 16 : initCap;
        data = new T[capacity];
    }
    
    ArrayList(const T* other, size_t size) : size(size) {
        capacity = size * 1.5;
        data = new T[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = other[i];
        }
    }
    
    ArrayList(const ArrayList* other) : size(other.size), capacity(other.capacity) {
        data = new T[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }
    
    T operator[](size_t index) { return data[index]; }
    const T operator[](size_t index) const { return data[index]; }
    
    void Add(const T& element) {
        if (size == capacity) {
            capacity *= 2;
            T *newData = new T[capacity];
            
            for (int i = 0; i < size; ++i) {
                newData[i] = data[i];
            }
            
            delete[] data;
            data = newData;
        }
        
        data[size++] = element;
    }
    
    void RemoveAt(int index) {
        T* newData = new T[size - 1];
        int i = 0, j = 0;
        while (i < size) {
            if (i == index) ++i;
            else newData[j++] = data[i++];
        }
        
        delete[] data;
        data = newData;
    }
    
    void Remove(const T& element) {
        int index = IndexOf(element);
        if (index >= 0) RemoveAt(index);
    }
    
    int IndexOf(const T& element) {
        for (int i = 0; i < size; ++i)
            if (data[i] == element) return i;
        return -1;
    }
};
