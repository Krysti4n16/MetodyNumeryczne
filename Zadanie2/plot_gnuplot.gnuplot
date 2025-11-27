set datafile separator ","
set terminal pngcairo size 1000,600 enhanced font 'Arial,12'

#wykres 1
set output 'wykres_czasow.png'
set title "Porównanie czasu wykonania uzytych metod" font ",14"
set xlabel "Rozmiar macierzy N"
set ylabel "Czas wykonania [s]"

set logscale xy
set format x "10^{%L}"
set format y "10^{%L}"
set grid xtics ytics mxtics mytics

set key left top box opaque

plot "czas_wykonania.csv" using 1:2 every ::1 with linespoints lw 3 pt 7 ps 1.5 lc rgb "green" title "Sherman-Morrison", \
     "czas_wykonania.csv" using 1:4 every ::1 with linespoints lw 3 pt 9 ps 1.5 lc rgb "red" title "Eliminacja Gaussa", \
     "czas_wykonania.csv" using 1:3 every ::1 with linespoints lw 3 pt 5 ps 1.5 lc rgb "blue" title "Gauss-Seidel"

#wykres 2
set output 'wykres_rozwiazania.png'
set title "Rozwiązanie układu dla N=1000" font ",14"
set xlabel "x_n"
set ylabel "u_n"

unset logscale
unset format
set grid

set key top right box

plot "rozwiazanie_sherman.csv" using 2:3 every ::1 with lines lw 4 lc rgb "green" title "Sherman-Morrison", \
     "rozwiazanie_gauss_seidel.csv" using 2:3 every ::1 with lines lw 2 lc rgb "blue" title "Gauss-Seidel", \
     "rozwiazanie_full.csv" using 2:3 every ::1 with lines lw 2 lc rgb "red" title "Eliminacja Gaussa"

unset output
