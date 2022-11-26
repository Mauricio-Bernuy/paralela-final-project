# paralela-final-project
```
git clean -f && git checkout -- . && git pull
```

## SERIAL
### compile
```bash
g++ BrokenTriangleSerial.cpp RngStream.cpp -o BrokenTriangleSerial 
```

### run
```bash
# set N
./BrokenTriangleSerial 10000000 

# print only results 
./BrokenTriangleSerial 10000000 RES_ONLY

# one liner
g++ BrokenTriangleSerial.cpp RngStream.cpp -o BrokenTriangleSerial && time ./BrokenTriangleSerial 10000000 RES_ONLY
```

## OMP
### compile
```bash
g++ -fopenmp BrokenTriangleOMP.cpp RngStream.cpp -o BrokenTriangleOMP 
```

### run
```bash
# set p
./BrokenTriangleOMP 8 

# set N
./BrokenTriangleOMP 8 1000000

# print only results 
./BrokenTriangleOMP 8 1000000 RES_ONLY

# one liner
g++ -fopenmp BrokenTriangleOMP.cpp RngStream.cpp -o BrokenTriangleOMP && time ./BrokenTriangleOMP 8 1000000 RES_ONLY
```

## Python Test Scripts
```bash
python3 testerSerial.py > serialResults.txt
python3 testerOMP.py > OMPResults.txt
```

## Khipu Run
```bash

```