#include <iostream>
#include <limits>
#include <map>
#include <vector>

using std::pair;
using std::vector;

// Count the number of non-negative integer elements in the union of range
// buckets input as a list of tuples. For three range buckets, [2,4], [8,9],
// and [3,6], the function should return 7 as shown on the number line below
//
//     +=+=+       +=+      <-- [2,4] and [8,9]
// 0-1-2-3-4-5-6-7-8-9-10-...
//       +=+=+=+            <-- [3,6]
//
//     +=+=+=+=+   +=+      <-- 7 total elements in range
//
size_t count_elements_in_ranges(vector<pair<int, int>> ranges)
{
    // sort the list from lowest to highest to make future processing easier
    // using a bubble sort starting at the end and working backwards
    // (leverage size_t rolling over from 0 to max rather than going negative)
    for (size_t i = ranges.size() - 1; i != std::numeric_limits<size_t>::max(); i--)
    {
        // make sure each pair is ordered low,high instead of high,low
        // so we can sort based on the first element
        pair<int, int> range = ranges.at(i);
        if (range.first > range.second)
        {
            int temp = range.first;
            range.first = range.second;
            range.second = temp;
            ranges.at(i) = range;
        }

        // compare this element to everything above it and bubble it up
        size_t j = i;
        while (j < ranges.size() - 1 && ranges.at(j).first > ranges.at(j + 1).first)
        {
            range = ranges.at(j);
            ranges.at(j) = ranges.at(j + 1);
            ranges.at(j + 1) = range;
            j++;
        }
    }

    // initialize a list of switches between out of range and in range, seed with first range bucket
    vector<int> switches = {ranges.at(0).first, ranges.at(0).second};

    // loop through 2nd through final ranges and process
    for (size_t i = 1; i < ranges.size(); i++)
    {
        pair<int, int> range = ranges.at(i);
        // each range must start after the one before it, so only need to check if the
        // end of the range is larger than the final switch from in range to out of range
        if (range.second > switches.back())
        {
            if (range.first <= switches.back() + 1)
            {
                // new range overlaps existing final range bucket, just extend end
                switches.back() = range.second;
            }
            else
            {
                // new range doesn't overlap existing range bucket, add new bucket
                switches.push_back(range.first);
                switches.push_back(range.second);
            }
        }
    }

    // sum up the total number of elements in range
    // (add one for each range bucket because 2..4 has 3 elements, not 2)
    size_t count = 0;
    for (size_t i = 0; i < switches.size(); i += 2)
        count += switches.at(i + 1) - switches.at(i) + 1;

    return count;
}

int main()
{
    pair<int, int> range1(2, 4);
    pair<int, int> range2(3, 6);
    pair<int, int> range3(8, 9);
    vector<pair<int, int>> ranges = {range1, range3, range2};
    std::cout << count_elements_in_ranges(ranges) << std::endl;

    return 0;
}
