##Zebra: light-weight Offline Rendering Library 
[中文版README](./README.md)

###Zebra is a light-weight CG Offline Rendering Library, it originates from [Giraffe](https://www.github.com/UncP/Giraffe)

***

###Goal
* Implement all kinds of algorithms that solve rendering equation 
* Be adjusted properly to cooperate with [Elephant](https://www.github.com/UncP/Elephant) to carry out some experiments between Computer Graphics and Maching Learning.


###If you are interested
`g++ -std=c++11 -O3 -fopenmp zebra.cpp -o Zebra`  
`./Zebra box pt 1 (1 means sample number, could be any positive number)`  
`./Zebra rainbow lt 8 (pt,lt represents algorithms)`  

###Implemented
* Pure Path Tracing
* Path Tracing with Direct Illumination
* Light Tracer

####Rainbow
![](./image/rainbow.bmp)

####Cornell Box
![](./image/box.bmp)

