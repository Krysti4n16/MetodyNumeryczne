set datafile separator ","
set term pngcairo size 1200,800
set output "czasy_algorytmy.png"

set title "Czas wykonania metod w zależności od N"
set xlabel "N"
set ylabel "Czas [s]"
set logscale x 10
set logscale y 10
set grid

plot "czasy.csv" using 1:2 with linespoints lw 2 lc rgb "blue" title "Thomas", \
     "czasy.csv" using 1:3 with linespoints lw 2 lc rgb "red" title "Gauss-Seidel", \
     "czasy.csv" using 1:4 with linespoints lw 2 lc rgb "green" title "Full Matrix"

