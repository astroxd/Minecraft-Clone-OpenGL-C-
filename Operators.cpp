//#include <iostream>
//
//struct AllocationMetrics {
//	uint32_t TotalAllocated = 0;
//	uint32_t TotalFreed = 0;
//
//	uint32_t CurrentUsage() { return TotalAllocated - TotalFreed; }
//};
//
//
//static AllocationMetrics s_AllocationMetrics;
//
//static void PrintMemoryUsage() {
//	std::cout << "Memory usage: " << s_AllocationMetrics.CurrentUsage() << std::endl;
//}
//
//
//void* operator new(size_t size) {
//	s_AllocationMetrics.TotalAllocated += size;
//	return malloc(size);
//}
//
//void operator delete(void* memory, size_t size) {
//	s_AllocationMetrics.TotalFreed += size;
//	free(memory);
//}
