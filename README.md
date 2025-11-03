V4|5:1:100|S4
For the first part of assigment I made three different solutions which use different datasetuctures.
1. Stores students in std::vector, so all three operations are O(n). But is slightly more memory efficient than the other two.
2. Stores students in std::set. They are sorted by group, so even thought all operations are still O(n), but finding group with the most students and the highes average rating is faster, because it doesn't require any additonal unordered maps for storing counts.
   Is less memory efficient, because students are not stored in a contigiuos block of memory, so there is a need for storing additional pointers.
3. Stores students in std::unordered_map by email, but also keeps two addtional unordered_maps for number of students in a group and average rating of a group
   Both finding group with most people and with highest average rating is O(number of groups), and changing a group by emial is O(1), but with some additional operation for keeping additional maps up to date.
I also impelemented sorting using bstsort.
