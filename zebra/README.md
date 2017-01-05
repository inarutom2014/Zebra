##Zebra: light-weight Offline Rendering Library
[English Version of README](./README.en.md)

###Zebra是一个轻量级的图形学离线渲染库，它来源于 [Giraffe](https://www.github.com/UncP/Giraffe)

***

###目标
* 实现各种求解渲染方程的算法
* 进行必要的调整来配合 [Elephant](https://www.github.com/UncP/Elephant) 进行图形学与机器学习相结合的实践

###如果你感兴趣
`g++ -std=c++11 -O3 -fopenmp zebra.cpp -o Zebra`  
`./Zebra box pt 1 (1代表采样数,可以为任意大于0的值)`  
`./Zebra rainbow lt 8(pt,lt代表算法)`  
`因为Zebra有自己的追踪语言，所以你可以在以.Zebra为后缀的文件中对场景参数进行修改而不用重新编译`  

###已实现
* 路径追踪
* 路径追踪（直接光照）
* Light Tracer

####Rainbow
![](./image/rainbow.bmp)

####Cornell Box
![](./image/box.bmp)
