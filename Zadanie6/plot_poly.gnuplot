P(x) = 243*x**7 - 486*x**6 + 783*x**5 - 990*x**4 + 558*x**3 - 28*x**2 - 72*x + 16

set title "Przebieg wielomianu P(x) w dziedzinie rzeczywistej" font ",14"
set xlabel "x"
set ylabel "P(x)"
set grid
set xzeroaxis lt 2 lw 2

set xrange [-0.8:1.0]
set yrange [-10:10]

set samples 1000 


plot P(x) title "P(x)" with lines lw 2 lc rgb "red", \
     "roots.dat" using 1:($2==0 ? 0 : 1/0) title "Pierwiastki rzeczywiste" with points pt 7 ps 1.5 lc rgb "blue"
