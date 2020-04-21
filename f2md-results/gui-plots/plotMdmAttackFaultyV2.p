set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto  			# set ytics automatically
set title "On board detection V2"
set xlabel "Time (Min)"
set ylabel "Detected (%)"
set y2label
set grid 
show grid 
funcT(x) = x / 60
func3(x,y) = y<=0  ? 0 : 100*x / y

plot "AppV2.dat" using (funcT($2)):(func3($4,$5)) title 'Cumulative Faulty V2' with lines lw 3 lc rgb "red" , \
"AppV2Inst.dat" using (funcT($2)):(func3($4,$5)) title 'Instantaneous Faulty V2' ps 0.5 lc rgb "red" , \
"AppV2.dat" using (funcT($2)):(func3($6,$7)) title 'Cumulative Attacker V2' with lines lc rgb "#009000" lw 3 , \
"AppV2Inst.dat" using (funcT($2)):(func3($6,$7)) title 'Instantaneous Attacker V2' ps 0.5 lc rgb "#009000"  

