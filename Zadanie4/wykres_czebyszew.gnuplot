set terminal pngcairo size 800,600 enhanced font 'Arial,12'
set output 'wykres_czebyszew_N45.png'

set title "Interpolacja Lagrange'a na wezlach Czebyszewa (N = 45)\nNajmniejszy blad aproksymacji"
set xlabel "x"
set ylabel "f(x)"
set grid
set datafile separator ","
set key top right box

set yrange [-0.1:1.1]
set xrange [-5:5]

set style line 1 lc rgb '#0060ad' lt 1 lw 2 # Niebieski (Dokładna)
set style line 2 lc rgb '#dd181f' lt 1 lw 2 # Czerwony (Czebyszew)

plot 'wyniki_interpolacji.csv' using 1:2 with lines ls 1 title "Funkcja dokladna f(x)", \
     'wyniki_interpolacji.csv' using 1:4 with lines ls 2 title "Interpolacja (Czebyszew)"
