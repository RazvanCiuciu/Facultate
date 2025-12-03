#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <stdexcept>
#include <string>

// Global mutex for synchronizing access to the shared resource among threads
std::mutex global_mutex; 

// ----------------------------------------------------------------------
// HeavyResource Class: Simulates a resource that requires explicit release.
// Illustrates the need for RAII (Item 13).
// ----------------------------------------------------------------------

class HeavyResource {
private:
    std::string name;
public:
    HeavyResource(const std::string& n) : name(n) {
        std::cout << "[Resource] Constructor: " << name << " opened.\n";
    }
    ~HeavyResource() {
        std::cout << "[Resource] Destructor: " << name << " closed and released.\n";
    }
    // const function is generally safe for concurrent read access
    void performOperation() const {
        std::cout << "[Resource] Operating on " << name << "...\n";
    }
    
    // Item 14: Deletes copy operations to simulate a unique/non-copyable resource
    HeavyResource(const HeavyResource&) = delete;
    HeavyResource& operator=(const HeavyResource&) = delete;
};

// ----------------------------------------------------------------------
// Resource Management Demos (Item 13)
// ----------------------------------------------------------------------

void unique_ptr_demo() {
    std::cout << "\n--- UNIQUE_PTR DEMO ---\n";
    // std::unique_ptr ensures exclusive ownership and automatic cleanup (RAII)
    std::unique_ptr<HeavyResource> unique_resource(new HeavyResource("Config_Unique"));
    
    try {
        unique_resource->performOperation();
        // If an exception were thrown here, the resource would still be released.
    } catch (const std::exception& e) {
        std::cerr << "Error caught: " << e.what() << "\n";
    }
    
    // The resource is AUTOMATICALLY released here when unique_resource goes out of scope.
    std::cout << "Unique Resource goes out of scope.\n";
}

void shared_ptr_management(std::shared_ptr<HeavyResource> ptr) {
    // Receiving a copy of the shared_ptr increments the reference count temporarily.
    ptr->performOperation();
    std::cout << "  (SharedPtr demo) Temporary reference count: " << ptr.use_count() << "\n";
    // The count is decremented when the local ptr goes out of scope.
}


// ----------------------------------------------------------------------
// Concurrency and Resource Sharing (Item 14)
// ----------------------------------------------------------------------

void access_resource_with_mutex(std::shared_ptr<HeavyResource> ptr, const std::string& thread_name) {
    // Synchronized access (mandatory for modifications, safe for reads)
    std::lock_guard<std::mutex> lock(global_mutex); // Locks the mutex
    
    std::cout << "[" << thread_name << "] [LOCKED] Accessed resource safely.\n";
    ptr->performOperation();
    std::cout << "[" << thread_name << "] [UNLOCKED] Finished access.\n";
    
    // The mutex is automatically unlocked by lock_guard when it goes out of scope (RAII in action)
}

void access_resource_without_mutex(std::shared_ptr<HeavyResource> ptr, const std::string& thread_name) {
    // Concurrent access WITHOUT synchronization (DANGEROUS for any modification)
    std::cout << "[" << thread_name << "] [WARNING] Starting resource access WITHOUT Mutex.\n";
    ptr->performOperation();
    std::cout << "[" << thread_name << "] [WARNING] Finished access WITHOUT Mutex.\n";
}

void thread_demo() {
    std::cout << "\n--- THREADS & SHARED_PTR DEMO ---\n";

    // Resource shared among threads (Item 14)
    std::shared_ptr<HeavyResource> shared_db_resource(new HeavyResource("Critical_Database"));

    std::cout << "\n-- Running with Mutex (Safe Access) --\n";
    // Each thread gets a copy of the shared_ptr
    std::thread t1(access_resource_with_mutex, shared_db_resource, "Thread 1 (Mutex)");
    std::thread t2(access_resource_with_mutex, shared_db_resource, "Thread 2 (Mutex)");
    
    t1.join();
    t2.join();

    std::cout << "\n-- Running Without Mutex (Concurrent Access) --\n";
    // Demonstrate concurrent access without sync. Note: The output order will be non-deterministic.
    std::thread t3(access_resource_without_mutex, shared_db_resource, "Thread 3 (No Mutex)");
    std::thread t4(access_resource_without_mutex, shared_db_resource, "Thread 4 (No Mutex)");
    
    t3.join();
    t4.join();
    
    // shared_db_resource will be destroyed when it goes out of scope in main.
}

// ----------------------------------------------------------------------
// Main Function
// ----------------------------------------------------------------------

int main() {
    std::cout << "--- C++ Resource Management Project (Effective C++) ---\n";
    
    // 1. UNIQUE_PTR DEMO (Item 13)
    unique_ptr_demo(); 
    
    // 2. SHARED_PTR COPYING DEMO (Item 13 & 14)
    std::cout << "\n--- SHARED_PTR COPYING DEMO ---\n";
    std::shared_ptr<HeavyResource> main_shared_ptr(new HeavyResource("Main_Log"));
    
    std::cout << "Initial reference count: " << main_shared_ptr.use_count() << "\n";
    
    // Call function which receives a copy of the shared_ptr
    shared_ptr_management(main_shared_ptr); 
    
    std::cout << "Reference count after shared_ptr_management: " << main_shared_ptr.use_count() << "\n";

    // 3. THREADS & MUTEX DEMO (Item 14)
    thread_demo();

    std::cout << "\nAll threads have finished.\n";
    
    // The remaining resources will be destroyed here, thanks to the smart pointers.
    std::cout << "Program exiting. Destructors will be called now.\n";
    
    return 0;
}