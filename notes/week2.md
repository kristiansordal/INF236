# P1
Vise hvorfor den gir dårlig speedup

En prosess per merge operation

merge step 1 takes time 2

step 2 takes time 4
step 3 takes time 8

runtime sum i =1 to log n 2^i

2s = sum i = 1 to k 2^(i+1)

ans S = 2s -s = 2^(k+1) = 2^(log n +1) - 2 = 2 * 2^log(n) - 2 = 2(n - 1)


S(n/2) = n log n /n = logn #SPEEDUP
