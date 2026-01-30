set terminal pngcairo size 800,600 enhanced font 'Arial,12'
set output 'wykres_rownoodlegle_N45.png'

set title "Interpolacja Lagrange'a dla wezlow rownoodleglych (N = 45)"
set xlabel "x"
set ylabel "Wartosc interpolowana"
set grid
set datafile separator ","
set key bottom center box

set xrange [-5:5]
set format y "%.0e"

set style line 1 lc rgb 'black' lt 2 lw 1 
set style line 2 lc rgb '#228B22' lt 1 lw 2 

plot 'wyniki_interpolacji.csv' using 1:2 with lines ls 1 title "Funkcja dokladna", \
     'wyniki_interpolacji.csv' using 1:3 with lines ls 2 title "Interpolacja (Rownoodlegle)"
