# HUST-ML-kNN

CS1807 Chenhui Mo, Implementation of the Machine Learning course of Huazhong University of Science and Technology

All in C++, Python refused

# Release 1.0:

The basical implementation of the kNN algorithm. The speed is 7-9 photos per second on mnist.

# Release 1.1:

Use priority queue to accelerate finding the wanted distance, the time complexity is O(size * log_2 k), the origin is O(size * k).

The speed is 10-12 photos per second on mnist.

# Release 1.2:

Rebuild the project and Optimize the cache. The speed is 15 photos per second on mnist.

# usage:

Data style： the first is the class it belongs to, followed by the coordinates of the points corresponding to the hyperplane

for example: [belongs, x1, x2, x3, x4, ..., xn]

You can download the file named kNN.h and include it into your project. Initialize it as a global variable because it uses a buffer to read data, which occupies 2^20 byte so you will get a stackoverflow.

Then call kNN::Work to run the main function. in it, you should input the path of the dataset and the path of the testset. After that, you may leave for a cup of coffee.

