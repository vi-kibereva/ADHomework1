V4|5:1:100|S4\
For the first part of assigment I made three different solutions which use different data structures.
1. Stores students in std::vector, so all three operations are O(n). But is slightly more memory efficient than the other two.
2. Stores students in std::set. They are sorted by group, so even thought all operations are still O(n), but finding group with the most students and the highes average rating is faster, because it doesn't require any additional unordered maps for storing counts.\
   Is less memory efficient, because students are not stored in a contiguous block of memory, so there is a need for storing additional pointers.
3. Stores students in std::unordered_map by email, but also keeps two additional unordered_maps for number of students in a group and average rating of a group\
   Both finding group with most people and with the highest average rating is O(number of groups), and changing a group by email is O(1), but with some additional operation for keeping additional maps up to date. The memory usage is higher because of storing additional data.(But still O(n))
I also implemented sorting using bstsort, which works slower that std::sort, because it doesn't operate on contiguous block of memory, which would be optimal for CPU performance.\
There are plots for performance of both tasks and also for memory usage, which can be found in sorting_performance.png, memory_usage.png and benchmark_performance.png\
To generate them:
```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
./Benchmark
./Benchmark_sorting
```
At this point you will have performance data in memory_results.csv, benchmark_result.csv, and sorting_results.csv
To visualize them:
```
cd ..
pip install -r requirements.txt
python3 visualization.py
python3 plot_sorting.py
python3 plot_memory.py
```
The results of memory measuring are inaccurate for small sample sizes, because then the data is stored in the memory allocated in advance\

