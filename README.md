# DecisionTree

This is a simple decision tree using C++ to develop.

### Sorting
This project uses Heap Sort to sort the data and find the best splitting point. There are several ways to separate the data, including using min-comfusing index or just randoming pick a spot.
Check `MLopr.cpp` to see the function.

#### Confusion Rate

The confusion rate in binary (true/false question) is defined as

$$
confusion(a, b)=2min(\frac{a}{a+b},\frac{b}{a+b})
$$

for a mixture of aTbF

### run the decision tree
First, you need to prepare your data in `LIBSVM` sparse form, check [LIBSVM](https://www.csie.ntu.edu.tw/~cjlin/libsvm/) for reference.

First, build the program by

```
make
```

Second, run the code by

```
./tree </location/of/training/file> EPS
```

There are two inputs to this program.

First one is the LIBSVM sparse training data location.

Second is EPS, which is the confusion rate of the decision tree. It can be set between 0~1, both extreme values are not recommended.
