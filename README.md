#TopHashtagsWithFibonacciHeaps
#Project Description
Implemented a method to get the most popular hashtags appeared on social media such as Facebook or Twitter. For the scope of this project, hashtags will be given from an input file. Basic idea for the implementation is to use a max priority  data structure and to find the most popular hashtags from that.
#Data Structures
Basic idea for the implementation is to use a max priority structure to find out the most popular hashtags. The following data structures were used:
Max Fibonacci heap: used to keep track of the frequencies of hashtags as a max priority queue.
1.	Amortized cost for Increase key operation is O (1). This allowed faster performance if there will be a large number of hashtags appearing in the stream and we might need to perform increase key operation many times.
2.	Amortized cost for Remove Max operation is O (log n), where n is the no. of nodes in the heap. 
Hash table: The key for the hash table is the hashtag, and the value is the pointer to the corresponding node in the Fibonacci heap.
#Execution
The “makefile” document creates an executable file named hashtagcounter. Program requires the input file name as an argument.
Following is an example execution of the program that read from an input file named file_name.

./hashtagcounter input_file_name output_file_name
