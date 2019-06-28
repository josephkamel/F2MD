set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically
set title "On board detection V1"
set xlabel "Time (Min)"
set ylabel "Detected (%)"
set y2label
set grid 
show grid 
funcT(x) = x / 60
func3(x,y) = y<=0  ? 0 : 100*x / y

plot "AppV1.dat" using (funcT($2)):(func3($4,$5)) title 'Cumulative Faulty V1' with lines lw 3 lc rgb "red" , \
 "AppV1Inst.dat" using (funcT($2)):(func3($4,$5)) title 'Instantaneous Faulty V1' ps 0.5 lc rgb "red" , \
 "AppV1.dat" using (funcT($2)):(func3($6,$7)) title 'Cumulative Attacker V1' with lines lc rgb "#009000" lw 3 , \
 "AppV1Inst.dat" using (funcT($2)):(func3($6,$7)) title 'Instantaneous Attacker V1' ps 0.5 lc rgb "#009000"  

