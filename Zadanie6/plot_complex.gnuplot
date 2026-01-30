set title "Rozklad miejsc zerowych na plaszczyznie zespolonej" font ",14"
set xlabel "Czesc Rzeczywista (Re)"
set ylabel "Czesc Urojona (Im)"
set grid
set size square

set xzeroaxis
set yzeroaxis
set xzeroaxis linetype 2 linewidth 1.5
set yzeroaxis linetype 2 linewidth 1.5

set xrange [-1.5:1.5]
set yrange [-2:2]

set style line 1 lc rgb "blue" pt 7 ps 1.5

plot "roots.dat" using 1:2 title "Miejsca zerowe" with points ls 1
