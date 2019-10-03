# C-programs
# producerConsumer

ProducerConsumer is a program using worker Threads to get character strings from an input file,
put the characters into a vector after removing empty spaces, sort the characters in ascending order
and print each string of characters to an output file separating each character by a comma.

# Requirement software

Ubuntu 18.04,
input file with up to 100 character string item per line and not to exceed 10000 lines of items.

# Installation
  The CMakeLists.txt creates a directory named, bin, where the executable is located. To make the
  executable file:
  make a directory for the project in an open terminal in Ubuntu 18.04 and enter commands:
  
  	$mkdir ProducerConsumerProject
  put the producerconsumer.cpp program an CMakeLists.txt in the new directory
  enter the following commands:
  
  	$cmake -H. -Bbuild
	$cmake --build build -- -j3
# Run Program
  producerConsumer takes three command line arguments:
  argument 1 is the input file name with the items to be sorted
  argument 2 is the output file name with the sorted output 
  argument 3 is the sort method: t = tree sort, m = merge sort

  next command to run producerConsumer with arguments:

	$./bin/producerConsumer inputFilename ouputFilename Sortmethod

  The Sortmethod is 't' or 'm'
# Tree Sort
  Tree sort has an average time complexity of O(n log n). Worst case senario is O(n^2),
  which only occurs in a list that is already sorted. 

# Merge Sort
  Merge Sort has an average, best and worst time complexity of O(n log n).The compromise for this is in memory, which is O(n).
