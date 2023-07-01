# LU-Decomposioion-of-a-Matrix
A sequential, Pthreads and OpenMP implementation of a program that performs Lower Upper (LU) decomposition on a matrix
# Sequential
The sequential program has been developed and parallelized using both pthread and openMP implementations. All the required 2D arrays in the program have been represented in 1D form to provide ease in caching. The arrays have been stored in row major format so that each thread works in consecutive parts of the same storage and false sharing problems are averted. Some operations do require column operations and may result in some instances of cache misses and false sharing; however, these are unavoidable. It is also to be noted that these codes were tested on an Ubuntu virtual machine, hence they require more time to run than if they had run directly on the host operating system. The total time of execution may not match the time printed by the program as only the part of the program where initializations, correctness checking (norm calculations) and printing are not done has been timed. These programs were run on ubuntu with 4 CPU cores using VirtualBox for n=7000. For both the codes, debugging statements have been commented out and not removed as these can help analyze the program. The programs output the value of time the moment the parallelized section of the code is completely executed but end only after verifying and printing the norm to verify correctness.
# OpenMP
OpenMP section: Only the outer loop has been parallelized as the collapse clause se in openMP used for nested loops provides additional overhead while causing correctness issues(max may be overwritten) which makes the code slower than if only the outer loop is parallelized(this is true only for a very large value of n as in this case threads do a somewhat equal amount of work).Locks have also been used whenever the matrices have been modified to preserve correctness of the program. Using a printf statement to determine the iteration running, it was determined that openMP was using guided scheduling (e.g.: - for n=7000 and t=2, the series starting at 1 and 3500 were observed). This makes the first thread the bottleneck as the amount of work keeps getting smaller for higher values of k.

Threads	Time(s)	Speedup(OMP)	Efficiency(S/4)
0			-0.08291(Extrapolated)
1	921	1	0.25
2	395	2.331646	0.582911
4	368	2.502717	0.625679
8	385	2.392208	0.598052
16	389	2.367609	0.591902

 
Data analysis: The time for execution required for the sequential code was 921 seconds. Just 2 cores give a time of 395 seconds. The reason for the speedup being slightly higher than 2 (S=2.33) for N=2 may be due to the reason that the virtual machine handles code execution differently for programs with and without openmp. The time required at 4 threads is minimum as expected as at this point, number of threads = number of cores. Beyond this point, increasing the number of threads does not decrease the time as any additional threads are executed by one of those four cores themselves and addition of more threads causes additional needless overhead (thread creation and destruction is a somewhat demanding task for the processor).

# Pthreads
Pthreads Section: The sequential code had to be modified heavily for the Pthread implementation when compared to the OpenMP implementation. The time of execution is much lower for implementation with pthreads than for openmp. A global structure variable has been used in this implementation for ease of access throughout the code and is available to all threads for any manipulation. While using global variables is generally not recommended, this makes the code extremely simple in comparison to if all the data had been passed to the threads as an argument. This also makes the code much simpler as pthreads can only take one argument and hence, we don’t need to pass a pointer to a structure object to each thread. 







Threads	Time(s)	Speedup (Pthread)	Efficiency(S/4)
0			-0.2675(Extrapolated)
1	921	1	0.25
2	300	3.07	0.7675
4	288	3.197917	0.799479
8	299	3.080268	0.770067
16	303	3.039604	0.759901

 


Data Analysis: Time for execution of sequential code was 921 seconds. The pthread implementation was faster than both the openmp and the sequential implementations. . The time required at 4 threads is minimum as expected as at this point, number of threads = number of cores. Beyond this point, increasing the number of threads does not decrease the time as any additional threads are executed by one of those four cores themselves and addition of more threads causes additional needless overhead which increases with increasing number of threads.


