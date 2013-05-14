set terminal epslatex
set output "doc/gnuplot/dynamic-time.eps"
set size 3/3., 3/3.
set format xy "$%g$"
set title "$T(n)= O(2*m*n)$"
set xlabel "entrada"
set ylabel "tempo"
plot [0:10] [0:10] 2*x*x
