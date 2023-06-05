reset
set output 'img/wait_time.png'
set title 'waiting time distribution'
set xlabel 'wait time'
set ylabel 'p'
set xrange [0:20]
set term png enhanced font 'Verdana,10'
set key off
set border 3
set boxwidth 0.5 absolute
set style fill solid 1.0 noborder
rounded(x) = floor(x) + 0.5
set boxwidth 0.9
plot 'data/wait_time.txt' using (rounded($1)):(1) smooth frequency with boxes