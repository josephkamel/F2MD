set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtic auto                        # set xtics automatically
set ytics auto                        # set ytics automatically
set y2tics auto		# set ytics automatically
set title "Absolute Latency Per Attacker" 
set xlabel "Time (Min)"
set ylabel "Detection Latency (s)"
set grid 
show grid 
funcT(x) = x / 60
func3(x,y) = y<=0  ? 0 : 100*x / y

stats "mdaV1.dat" using (lastV1=$8) nooutput
stats "mdaV2.dat" using (lastV2=$8) nooutput
set label sprintf("V1 = %3.5gs \nV2 = %3.5gs",lastV1, lastV2) at graph 0.02,0.95 tc rgb '#00bb00'

plot "mdaV2.dat" using (funcT($2)):($8)  title 'V2 Latency' with linespoints linestyle 2 lw 2 pi 30 ps 0.75 lc rgb "blue",\
"mdaV1.dat" using (funcT($2)):($8)  title 'V1 Latency' with lines lw 2 lt rgb "red"

