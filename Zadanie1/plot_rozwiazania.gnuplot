set datafile separator ","
set term pngcairo size 1200,800
set output "rozwiazania_N1000.png"

set title "Rozwiązania numeryczne dla N=1000"
set xlabel "x"
set ylabel "u(x)"
set grid

plot "thomas_1000.csv" using 1:2 with lines lw 2 lc rgb "blue" title "Thomas", \
     "gauss_seidel_1000.csv" using 1:2 with lines lw 2 lc rgb "red" title "Gauss-Seidel", \
     "full_matrix_1000.csv" using 1:2 with lines lw 2 lc rgb "green" title "Full Matrix"
