#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>
#include <sstream>

// Strategy
class SortStrategy {
public:
    virtual void sort(std::vector<int>& data) = 0;
    virtual ~SortStrategy() {}
};

class BubbleSort : public SortStrategy {
public:
    void sort(std::vector<int>& data) override {
        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < data.size() - i - 1; ++j) {
                if (data[j] > data[j + 1]) {
                    std::swap(data[j], data[j + 1]);
                }
            }
        }
    }
};

class QuickSort : public SortStrategy {
public:
    void sort(std::vector<int>& data) override {
        quicksort(data, 0, data.size() - 1);
    }

private:
    void quicksort(std::vector<int>& data, int low, int high) {
        if (low < high) {
            int pi = partition(data, low, high);
            quicksort(data, low, pi - 1);
            quicksort(data, pi + 1, high);
        }
    }

    int partition(std::vector<int>& data, int low, int high) {
        int pivot = data[high];
        int i = low - 1;
        for (int j = low; j < high; ++j) {
            if (data[j] < pivot) {
                std::swap(data[++i], data[j]);
            }
        }
        std::swap(data[i + 1], data[high]);
        return i + 1;
    }
};

// Template Method
class SortTemplate {
public:
    virtual ~SortTemplate() {}
    void sort(std::vector<int>& data) {
        preSort(data);
        doSort(data);
        postSort(data);
    }

protected:
    virtual void preSort(std::vector<int>& data) {}
    virtual void doSort(std::vector<int>& data) = 0;
    virtual void postSort(std::vector<int>& data) {}
};

class BubbleSortTemplate : public SortTemplate {
protected:
    void doSort(std::vector<int>& data) override {
        BubbleSort().sort(data);
    }
};

class QuickSortTemplate : public SortTemplate {
protected:
    void doSort(std::vector<int>& data) override {
        QuickSort().sort(data);
    }
};

// Composite
class SortComponent {
public:
    virtual void sort(std::vector<int>& data) = 0;
    virtual ~SortComponent() {}
};

class SimpleSort : public SortComponent {
    std::unique_ptr<SortStrategy> strategy;
public:
    SimpleSort(std::unique_ptr<SortStrategy> strat) : strategy(std::move(strat)) {}
    void sort(std::vector<int>& data) override {
        strategy->sort(data);
    }
};

class CompositeSort : public SortComponent {
    std::vector<std::unique_ptr<SortComponent>> children;
public:
    void add(std::unique_ptr<SortComponent> component) {
        children.push_back(std::move(component));
    }
    void sort(std::vector<int>& data) override {
        for (auto& child : children) {
            child->sort(data);
        }
    }
};

// Adapter
class ThirdPartySort {
public:
    void externalSort(std::vector<int>& data) {
        std::sort(data.begin(), data.end());
    }
};

class SortAdapter : public SortStrategy {
    ThirdPartySort thirdPartySorter;
public:
    void sort(std::vector<int>& data) override {
        thirdPartySorter.externalSort(data);
    }
};

// Iterator
class Iterator {
public:
    virtual bool hasNext() = 0;
    virtual int next() = 0;
    virtual ~Iterator() {}
};

class VectorIterator : public Iterator {
    std::vector<int>& data;
    size_t index;
public:
    VectorIterator(std::vector<int>& vec) : data(vec), index(0) {}
    bool hasNext() override {
        return index < data.size();
    }
    int next() override {
        return data[index++];
    }
};

// Builder
class SortBuilder {
    std::unique_ptr<SortStrategy> strategy;
public:
    SortBuilder& setBubbleSort() {
        strategy = std::make_unique<BubbleSort>();
        return *this;
    }
    SortBuilder& setQuickSort() {
        strategy = std::make_unique<QuickSort>();
        return *this;
    }
    std::unique_ptr<SortStrategy> build() {
        return std::move(strategy);
    }
};

// Command
class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() {}
};

class SortCommand : public Command {
    std::unique_ptr<SortStrategy> sorter;
    std::vector<int> data;
public:
    SortCommand(std::unique_ptr<SortStrategy> strat, std::vector<int> dat)
        : sorter(std::move(strat)), data(std::move(dat)) {}
    void execute() override {
        sorter->sort(data);
        for (int num : data) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
};

// Memento
class Memento {
    std::vector<int> state;
public:
    Memento(std::vector<int> s) : state(std::move(s)) {}
    std::vector<int> getState() const {
        return state;
    }
};

class Originator {
    std::vector<int> state;
public:
    void setState(std::vector<int> s) {
        state = std::move(s);
    }
    Memento saveStateToMemento() {
        return Memento(state);
    }
    void getStateFromMemento(const Memento& memento) {
        state = memento.getState();
    }
};

// Facade
class SortFacade {
    std::unique_ptr<SortStrategy> bubbleSort;
    std::unique_ptr<SortStrategy> quickSort;
public:
    SortFacade() {
        bubbleSort = std::make_unique<BubbleSort>();
        quickSort = std::make_unique<QuickSort>();
    }
    void sortWithBubble(std::vector<int>& data) {
        bubbleSort->sort(data);
    }
    void sortWithQuick(std::vector<int>& data) {
        quickSort->sort(data);
    }
};

// Observer
class Observer {
public:
    virtual void update() = 0;
    virtual ~Observer() {}
};

class Subject {
    std::vector<std::unique_ptr<Observer>> observers;
public:
    void addObserver(std::unique_ptr<Observer> observer) {
        observers.push_back(std::move(observer));
    }
    void notify() {
        for (auto& observer : observers) {
            observer->update();
        }
    }
};

class SortObserver : public Observer {
    void update() override {
        std::cout << "Sorting completed.\n";
    }
};

int main() {
    std::vector<int> data;
    std::string input;
    std::cout << "Enter numbers to sort (separated by spaces): ";
    std::getline(std::cin, input);
    std::istringstream iss(input);
    int number;
    while (iss >> number) {
        data.push_back(number);
    }

    int choice;
    std::cout << "Choose sorting algorithm: 1 for Bubble Sort, 2 for Quick Sort: ";
    std::cin >> choice;

    auto start = std::chrono::high_resolution_clock::now();
    if (choice == 1) {
        BubbleSort().sort(data);
    } else if (choice == 2) {
        QuickSort().sort(data);
    } else {
        std::cout << "Invalid choice.\n";
        return 1;
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Sorted Data: ";
    for (int num : data) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time taken: " << duration << " microseconds" << std::endl;

    return 0;
}
