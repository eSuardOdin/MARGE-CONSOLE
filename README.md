# MARGE-CONSOLE



## Testing the CPU

### R TYPE

ADD: 
![alt text](./img/add.png)
Test:
- `ADD x1(1), x1(1), x2($FFFFFFFF) => x1 = -1`
```
x1:= 1
x2:= $FFFFFFFF
0000 0000 0010 0000 1000 0000 1011 0011
00        20        80        B3
B3 80 20 00
```

- `ADD x1($80000000), x1($80000000), x2(1) => x1 = Max negative value`
```
x1:= $80000000
x2:= 1
0000 0000 0010 0000 1000 0000 1011 0011
00        20        80        B3
B3 80 20 00
```
