# DecisionTree

This is a simple decision tree using C++ to develop.

### Sorting
This project uses Heap Sort to sort the data and find the best splitting point. There are several ways to separate the data, including using min-comfusing index or just randoming pick a spot.
Check `MLopr.cpp` to see the function.

#### Confusion Rate

Confusion rate is the essential key to build the decision tree, and there are several ways to evaluate the confusion rate, for example, gini index.
In this project, the confusion rate is defined as

confusion(a, b) = 2*min(a/(a+b), b/(a+b))

where a and b stand for the "YES" and "NO" in a set of data.

To minimize the confusion rate after branching, the total confusion rate after branching need to be calculated:

total_confusion(c, d, e, f) = (c+d)/(c+d+e+f)*confusion(c, d) + (e+f)/(c+d+e+f)*confusion(e, f)

where c, e is the "YES" in two branches and d, f is the "NO" in two branches


For a well developed tree, the confusion rate in each level need to be minimized.

### Result

After running the program, it will produce a txt file, inside is cpp function using nested if/else operations. For example, if using `self.train` file as training data and set `EPS` = 0.5, the output file will be

```cpp=
int tree_predict(double* attr) {
    if (attr[1] < 0.000000) {
        return -1;
    }
    else {
        return +1;
    }
}
```


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

If you need to try, there is two example training file inside. First one is `heart.train`, which is also the example file of the `LIBSVM` library. The second file `self.train` is a much smaller file created by me, which can be used to debug the code.
